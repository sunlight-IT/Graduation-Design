#pragma once

#include <stdbool.h>

#include "gpio.h"

#define VOICE_CS       VOICE_CS_Pin
#define VOICE_CS_PORT  VOICE_CS_GPIO_Port
#define VOICE_RST      VOICE_RST_Pin
#define VOICE_RST_PORT VOICE_RST_GPIO_Port
#define VOICE_IRQ      VOICE_IRQ_Pin
#define VOICE_IRQ_PORT VOICE_IRQ_GPIO_Port

#define VO_CS_H VOICE_CS_PORT->BSRR = VOICE_CS
#define VO_CS_L VOICE_CS_PORT->BRR = VOICE_CS

#define VO_RST_H VOICE_RST_PORT->BSRR = VOICE_RST
#define VO_RST_L VOICE_RST_PORT->BRR = VOICE_RST

#define CODE_LEN 20
#define CODE_NUM 50

#define WR_CMD 0x04
#define RD_CMD 0x05

//	以下三个状态定义用来记录程序是在运行ASR识别还是在运行MP3播放
#define LD_MODE_IDLE    0x00
#define LD_MODE_ASR_RUN 0x08
#define LD_MODE_MP3     0x40

#define CLK_IN        24 /* 用户注意修改输入的晶振时钟大小 */
#define LD_PLL_11     (uint8_t)((CLK_IN / 2.0) - 1)
#define LD_PLL_MP3_19 0x0f
#define LD_PLL_MP3_1B 0x18
#define LD_PLL_MP3_1D (uint8_t)(((90.0 * ((LD_PLL_11) + 1)) / (CLK_IN)) - 1)

#define LD_PLL_ASR_19 (uint8_t)(CLK_IN * 32.0 / (LD_PLL_11 + 1) - 0.51)
#define LD_PLL_ASR_1B 0x48
#define LD_PLL_ASR_1D 0x1f

#define MIC_VOL 0x43  // 咪头增益

// typedef enum {
//   k_REG_FIFO_DATA       = 0x01,
//   k_REG_FIFO_IQR        = 0x02,
//   k_REG_FIFO_EXT        = 0x05,
//   k_REG_FIFO_STATUS     = 0x06,
//   k_REG_FIFO_CLEAR      = 0x08,
//   k_REG_CLOCK_FREQ_1    = 0x11,
//   k_REG_SOFT_DSP        = 0x17,
//   k_REG_CLOCK_FREQ_2    = 0x19,
//   k_REG_CLOCK_FREQ_3    = 0x1b,
//   k_REG_ADC             = 0x1c,
//   k_REG_CLOCK_FREQ_4    = 0x1d,
//   k_REG_ADC_CTRL        = 0x1e,
//   k_REG_FIFO_DATA_UPP_L = 0x20,
//   k_REG_FIFO_DATA_UPP_H = 0x21,
//   k_REG_FIFO_DATA_LOW_L = 0x22,
//   k_REG_FIFO_DATA_LOW_H = 0x20,

//   k_REG_FIFO_DATA_MCU_WATER_L = 0x24,
//   k_REG_FIFO_DATA_MCU_WATER_H = 0x25,
//   k_REG_FIFO_DATA_MCU_WATER_L = 0x26,
//   k_REG_FIFO_DATA_MCU_WATER_H = 0x27,
//   k_REG_IQR                   = 0x29,
//   k_REG_IQR_REQUST            = 0x2b,
//   k_REG_MP3                   = 0x33,
//   k_REG_ADC_GAIN              = 0x35,
//   k_REG_

// } LD3320_REG;

typedef enum {
  k_asr_none      = 0x00,
  k_asr_runing    = 0x01,
  k_asr_foundok   = 0x10,
  k_asr_foundzero = 0x11,
  k_asr_error     = 0x31,
} e_asr_status;

typedef enum {
  k_code_cmd  = 0x00,  //
  k_code_dmcs = 0x01,  // 代码测试
  k_code_cswb = 0x02,  // 测试完毕
  k_code_1    = 0x03,  //
  k_code_2    = 0x04,  //
  k_code_3    = 0x05,  //
  k_code_4    = 0x06,  //
  k_code_5    = 0x07,  //
  k_code_6    = 0x08,  //

  k_code_max = 0xff,
} asr_code_num;

typedef struct {
  uint8_t cmd[CODE_LEN];
  uint8_t code;
  uint8_t chinese_show[CODE_LEN];
} voidce_cmd_code_t;

void ld3320_handle_reg(SPI_HandleTypeDef* h_spi);
void ld3320_reset(void);

bool asr_recognize(void);
bool ld3320_asr_add_fixed(void);
bool ld3320_asr_run(void);

void ld3320_test(void);

void set_asr_status(int32_t status);

uint8_t  get_asr_result(void);
int32_t  get_asr_status(void);
uint8_t* get_voice_cmd_data_chinese(int code);

void ld3320_exit(void);
