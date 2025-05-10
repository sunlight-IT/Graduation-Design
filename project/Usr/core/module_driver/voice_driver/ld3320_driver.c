#include "ld3320_driver.h"

#include "component/component.h"

#define TAG "LD3320_driver"

#define CMD_DATA_CFG(cmd, code, show) {(cmd), (code), (show)}

// #define _NOP \
//   _nop();    \
//   _nop();    \
//   _nop();

// static void LD_Delay();

static bool               ars_irq;
static int32_t            asr_status = k_asr_none;
static SPI_HandleTypeDef* m_spi;
static uint8_t            nLD_Mode = LD_MODE_IDLE;

// ��������޸Ķ�Ӧ������ָ��
static const voidce_cmd_code_t voice_cmd_data[] = {
    CMD_DATA_CFG("xiao jie", k_code_cmd, "�յ�"),  //
    // CMD_DATA_CFG("dai ma ce shi", k_code_dmcs, "������Գɹ�"),  //
    // CMD_DATA_CFG("ce shi wan bi", k_code_cswb, "�������"),      //

    // CMD_DATA_CFG("bei jing", k_code_1, "����"),   //
    // CMD_DATA_CFG("shang hai", k_code_2, "�Ϻ�"),  //
    // CMD_DATA_CFG("kai deng", k_code_3, "����"),   //
    // CMD_DATA_CFG("guan deng", k_code_4, "�ص�"),  //
    CMD_DATA_CFG("ni hao", k_code_dmcs, "������Գɹ�"),  //
    CMD_DATA_CFG("ni hao", k_code_cswb, "�������"),    //

    CMD_DATA_CFG("ni hao", k_code_1, "����"),  //
    CMD_DATA_CFG("ni hao", k_code_2, "�Ϻ�"),   //
    CMD_DATA_CFG("ni hao", k_code_3, "����"),  //
    CMD_DATA_CFG("ni hao", k_code_4, "�ص�"),   //
};

static const uint8_t CMD_DATA_NUM = sizeof(voice_cmd_data) / sizeof(voice_cmd_data[0]);

void ld3320_handle_reg(SPI_HandleTypeDef* h_spi) { m_spi = h_spi; }

static void LD3320_WR_REG(uint8_t byte) {
  uint8_t data = byte;
  uint8_t i;
  // LD_Delay();
  if (HAL_SPI_Transmit(m_spi, &data, 1, 100)) {
    LOGE("HAL_SPI_Transmit error");
  }
}
static uint8_t LD3320_RD_REG(void) {
  uint8_t           byte = 0;
  uint8_t           i, temp;
  HAL_StatusTypeDef status;

  if ((status = HAL_SPI_Receive(m_spi, &temp, 1, 100))) {
    LOGE("HAL_SPI_Receive error");
  }

  return temp;
}

static void ld3320_write_reg(uint8_t addr, uint8_t data) {
  VO_CS_L;
  LD3320_WR_REG(WR_CMD);
  LD3320_WR_REG(addr);
  LD3320_WR_REG(data);
  VO_CS_H;
}

static uint8_t ld3320_read_reg(uint8_t addr) {
  uint8_t data = 0;
  VO_CS_L;
  LD3320_WR_REG(RD_CMD);
  LD3320_WR_REG(addr);
  data = LD3320_RD_REG();
  VO_CS_H;
  return data;
}

void ld3320_reset(void) {
  VO_RST_H;
  HAL_Delay(5);
  VO_RST_L;
  HAL_Delay(5);
  VO_RST_H;
  HAL_Delay(5);

  VO_CS_L;
  HAL_Delay(5);
  VO_CS_H;
  HAL_Delay(5);
}

void ld3320_init_cmd(void) {
  ld3320_read_reg(0x06);
  ld3320_write_reg(0x17, 0x35);

  HAL_Delay(10);
  ld3320_read_reg(0x06);

  ld3320_write_reg(0x89, 0x03);
  HAL_Delay(5);
  ld3320_write_reg(0xCF, 0x43);
  HAL_Delay(5);
  ld3320_write_reg(0xCB, 0x02);

  /*PLL setting*/
  ld3320_write_reg(0x11, LD_PLL_11);
  if (nLD_Mode == LD_MODE_MP3) {
    ld3320_write_reg(0x1E, 0x00);
    ld3320_write_reg(0x19, LD_PLL_MP3_19);
    ld3320_write_reg(0x1B, LD_PLL_MP3_1B);
    ld3320_write_reg(0x1D, LD_PLL_MP3_1D);
  } else {
    ld3320_write_reg(0x1E, 0x00);
    ld3320_write_reg(0x19, LD_PLL_ASR_19);
    ld3320_write_reg(0x1B, LD_PLL_ASR_1B);
    ld3320_write_reg(0x1D, LD_PLL_ASR_1D);
  }
  HAL_Delay(5);

  ld3320_write_reg(0xCD, 0x04);
  ld3320_write_reg(0x17, 0x4c);
  HAL_Delay(5);
  ld3320_write_reg(0xB9, 0x00);
  ld3320_write_reg(0xCF, 0x4F);
  ld3320_write_reg(0x6F, 0xFF);
}

void ld3320_init_asr(void) {
  nLD_Mode = LD_MODE_ASR_RUN;
  ld3320_init_cmd();

  ld3320_write_reg(0xBD, 0x00);
  ld3320_write_reg(0x17, 0x48);
  HAL_Delay(5);

  ld3320_write_reg(0x3C, 0x80);
  ld3320_write_reg(0x3E, 0x07);
  ld3320_write_reg(0x38, 0xff);
  ld3320_write_reg(0x3A, 0x07);

  ld3320_write_reg(0x40, 0);
  ld3320_write_reg(0x42, 8);
  ld3320_write_reg(0x44, 0);
  ld3320_write_reg(0x46, 8);
  HAL_Delay(5);
}

bool ld3320_check_asr_busy_b2(void) {
  uint8_t reg_val = 1;
  for (size_t i = 0; i < 5; i++) {
    reg_val = ld3320_read_reg(0xb2);
    if (0x21 == reg_val) {
      return true;
    }
    HAL_Delay(20);
  }
  return false;
}

bool ld3320_asr_run(void) {
  ld3320_write_reg(0x35, MIC_VOL);
  ld3320_write_reg(0x1C, 0x09);
  ld3320_write_reg(0xBD, 0x20);
  ld3320_write_reg(0x08, 0x01);
  HAL_Delay(5);
  ld3320_write_reg(0x08, 0x00);
  HAL_Delay(5);

  if (!ld3320_check_asr_busy_b2()) {
    return false;
  }

  ld3320_write_reg(0xB2, 0xff);
  ld3320_write_reg(0x37, 0x06);
  HAL_Delay(5);
  ld3320_write_reg(0x37, 0x06);
  HAL_Delay(5);
  ld3320_write_reg(0x1C, 0x0b);
  ld3320_write_reg(0x29, 0x10);
  ld3320_write_reg(0xBD, 0x00);
  return true;
}

bool ld3320_asr_add_fixed(void) {
  uint8_t asr_add_len = 0;
  for (size_t index = 0; index < CMD_DATA_NUM; index++) {
    if (!ld3320_check_asr_busy_b2()) {
      LOGW("ld3320 busy");
      return false;
    }

    ld3320_write_reg(0xc1, voice_cmd_data[index].code);
    ld3320_write_reg(0xc3, 0);
    ld3320_write_reg(0x08, 0x04);
    HAL_Delay(1);
    ld3320_write_reg(0x08, 0x00);
    HAL_Delay(1);

    // һ��һ���ֽ�д�� 0xb9�Ĵ�����
    for (asr_add_len = 0; asr_add_len < CODE_LEN; asr_add_len++) {
      if (voice_cmd_data[index].cmd[asr_add_len] == 0) break;
      ld3320_write_reg(0x5, voice_cmd_data[index].cmd[asr_add_len]);
      // LOGI( "%s", ld3320_read_reg(0x05));
    }
    // LOGI( "%d %s", voice_cmd_data[index].code, voice_cmd_data[index].cmd);
    ld3320_write_reg(0xb9, asr_add_len);
    ld3320_write_reg(0xb2, 0xff);
    ld3320_write_reg(0x37, 0x04);
  }
  return true;
}

bool asr_recognize(void) {
  for (size_t i = 0; i < 5; i++)  //	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
  {
    ld3320_init_asr();
    HAL_Delay(5);
    if (!ld3320_asr_add_fixed()) {
      ld3320_reset();  //	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
      HAL_Delay(5);    //	���ӳ�ʼ����ʼ����ASRʶ������
      continue;
    }
    HAL_Delay(5);
    if (!ld3320_asr_run()) {
      ld3320_reset();  //	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
      HAL_Delay(5);    //	���ӳ�ʼ����ʼ����ASRʶ������
      continue;
    }
    return true;  //	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
  }
  return false;
}

uint8_t get_asr_result(void) {
  uint8_t resualt = ld3320_read_reg(0xc5);
  return resualt;
}

void set_asr_status(int32_t status) { asr_status = (e_asr_status)status; }

int32_t get_asr_status(void) { return asr_status; }

uint8_t* get_voice_cmd_data_chinese(int code) {
  int code_tmp = code;
  for (size_t i = 0; i < CMD_DATA_NUM; i++) {
    if (voice_cmd_data[i].cmd == code_tmp)  //
      return voice_cmd_data[i].chinese_show;
  }
  return NULL;
}

void process_int(void) {
  uint8_t reg_val;
  uint8_t asr_res_count;
  reg_val = ld3320_read_reg(0x2b);
  ld3320_write_reg(0x29, 0);
  ld3320_write_reg(0x02, 0);
  if ((reg_val & 0x10) && ld3320_read_reg(0xb2) == 0x21 && ld3320_read_reg(0xbf) == 0x35) /*ʶ��ɹ�*/
  {
    asr_res_count = ld3320_read_reg(0xba);

    if (asr_res_count > 0 && asr_res_count <= 4) {
      asr_status = k_asr_foundok;
    } else {
      asr_status = k_asr_foundzero;
      LOGI("count = %d", asr_res_count);
      LOGI("asr res count error");
    }
  } /*û��ʶ����*/
  else {
    asr_status = k_asr_foundzero;
    LOGI("empty");
  }

  ld3320_write_reg(0x2b, 0);
  ld3320_write_reg(0x1C, 0); /*д0:ADC������*/
  ld3320_write_reg(0x29, 0);
  ld3320_write_reg(0x02, 0);
  ld3320_write_reg(0x2B, 0);
  ld3320_write_reg(0xBA, 0);
  ld3320_write_reg(0xBC, 0);
  ld3320_write_reg(0x08, 1); /*���FIFO_DATA*/
  ld3320_write_reg(0x08, 0); /*���FIFO_DATA�� �ٴ�д0*/
  // LOGI( "process sucess");
}

void ld3320_test(void) {
  ld3320_reset();
  LOGI("%02x", ld3320_read_reg(0x06));
  LOGI("%02x", ld3320_read_reg(0x06));
  LOGI("%02x", ld3320_read_reg(0x35));
  LOGI("%02x", ld3320_read_reg(0xb3));

  ld3320_read_reg(0x6);
  ld3320_write_reg(0x35, 0x33);
  ld3320_write_reg(0x1b, 0x55);
  ld3320_write_reg(0xb3, 0xaa);
  LOGI("%02x", ld3320_read_reg(0x35));
  LOGI("%02x", ld3320_read_reg(0x1b));
  LOGI("%02x", ld3320_read_reg(0xb3));
}

void ld3320_exit(void) { process_int(); }
// void LD_Delay(void) {
//   uint32_t i = 72;
//   while (i--) {
//     /// __NOP;
//   }
// }