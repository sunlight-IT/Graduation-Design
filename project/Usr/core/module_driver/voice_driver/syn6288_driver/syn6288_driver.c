#include "syn6288_driver.h"

#include "component/component.h"
#include "string.h"

#define TAG                  "syn6288 driver"
#define DEBUG                1
#define UART2_TIMEOUT        300
#define UART2_RECV_WAIT_TIME 200

static uint8_t set_parame(uint8_t high, uint8_t low);
static uint8_t calculate_ecc(uint8_t* cmd, uint8_t data_len);

static UART_HandleTypeDef* m_uart;
static cmd_frame_t         cmd_frame_cache;
static uint8_t             cmd_buf[64];

static uint8_t  rx_buf[BUF_SIZE];
static uint8_t  rx_cache;
static uint8_t  recv_back;
static uint8_t  size;
static uint32_t ticktime;

void syn_handle_uart(UART_HandleTypeDef* h_uart) {
  m_uart = h_uart;
  size   = 0;
  HAL_UART_Receive_IT(m_uart, &rx_cache, 1);
}

uint8_t* get_rx_cache(void) { return &rx_cache; }

void syn_recv(void) {
  if (size >= BUF_SIZE) {
    LOGE("rxbuf overflow %d", size);
    return;
  }

  // rx_buf[size] = rx_cache;
  // rx_cache     = 0;
  // ++size;

  HAL_UART_Receive_IT(m_uart, &rx_cache, 1);
  ticktime = HAL_GetTick();
  LOGI("%02x", rx_cache);
}

static uint8_t cmd_frame_pack_txt(uint8_t word, uint8_t background_music, uint8_t txt_code, const uint8_t* txt) {
  uint8_t cmd_pack_len = 0;
  uint8_t str_len      = 0;
  uint8_t data_len     = 0;
  bool    param_status = false;
  if (NULL != txt) {
    str_len = strlen(txt);
    if (str_len > 200) {
      LOGE("txt strlen %d > 200", str_len);
      return;
    }
  }

  if (kSeacherStatusWord != word) {
    data_len     = (1 + 1 + str_len + 1);
    param_status = true;
  } else {
    data_len     = (1 + 1 + str_len);
    param_status = false;
  }

  cmd_buf[cmd_pack_len++] = 0xFD;
  cmd_buf[cmd_pack_len++] = 0;
  cmd_buf[cmd_pack_len++] = data_len;
  cmd_buf[cmd_pack_len++] = word;

  if (param_status == true) {
    cmd_buf[cmd_pack_len++] = set_parame(background_music, txt_code);
  }

  if (str_len != 0) {
    memcpy((cmd_buf + cmd_pack_len), txt, str_len);
    cmd_pack_len += str_len;
  }

  cmd_buf[cmd_pack_len] = calculate_ecc(cmd_buf, cmd_pack_len);
  ++cmd_pack_len;
#if DEBUG
  for (int i = 0; i < cmd_pack_len; i++) {
    LOGI("%02x", *(cmd_buf + i));
  }
#endif
  if (HAL_UART_Transmit_DMA(m_uart, cmd_buf, cmd_pack_len)) {
    LOGE("send error");
  }

  ticktime = HAL_GetTick();
}

static void cmd_frame_send_other(uint8_t word) {
  switch (word) {
    case kBaudRateWord:
      break;
  }
}

void recv_back_process(void) {
  switch (recv_back) {
    case kInitSuccess:
      LOGI("Init success %02x", recv_back);
      break;
    case kRecvSuccess:
      LOGI("Receive sucess %02x", recv_back);
      break;
    case kRecvFail:
      LOGE("Receive fail %02x", recv_back);
      break;
    case kRecvQuery:
      LOGI("query status %02x", recv_back);
      break;
    case kEmpty:
      LOGI("chip idle %02x", recv_back);
      break;
    default:
      LOGE("Receive error %02x", recv_back);
      break;
  }
}

bool syn_recv_ack(void) {
  while (1) {
    if (size != 0 && has_pass_time(ticktime) > UART2_RECV_WAIT_TIME) {
      size      = 0;
      recv_back = rx_buf[0];
      for (int i = 0; i < 10; i++) LOGI("recv_back %02x", rx_buf[i]);
      memset(rx_buf, 0, 10);
      return true;
    } else if (has_pass_time(ticktime) > UART2_TIMEOUT) {
      LOGE("Time out");
      return false;
    }
  }
}

void voice_compound_cmd(uint8_t* txt) {
  cmd_frame_pack_txt((uint8_t)kVoiceCompoundWord, 0, 0, txt);
  // if (syn_recv_ack()) {
  //   recv_back_process();
  // }
}

void voice_baud_cmd(void) {
  cmd_frame_pack_txt((uint8_t)kBaudRateWord, 0, 0, NULL);
  // if (syn_recv_ack()) {
  //   recv_back_process();
  // }
}

void voice_inquire_cmd(void) {
  cmd_frame_pack_txt((uint8_t)kSeacherStatusWord, 0, 0, NULL);

  // if (syn_recv_ack()) {
  //   recv_back_process();
  // }
}

void syn6288_inquiry(void) {
  uint8_t frame_len = cmd_frame_pack_txt(kSeacherStatusWord, 0, 0, NULL);
  if (HAL_UART_Transmit(m_uart, (uint8_t*)(&cmd_frame_cache), frame_len, 10)) {
    LOGE("send error");
  }
  // if (syn_recv_ack()) {
  //   recv_back_process();
  // }
}

uint8_t set_parame(uint8_t high, uint8_t low) {
  uint8_t parame = ((high & 0x1f) << 3);
  parame |= low & 0x08;
  return parame;
}

uint8_t calculate_ecc(uint8_t* cmd, uint8_t data_len) {
  uint8_t ecc = 0;
  for (int i = 0; i < data_len; i++) {
    // LOGI("ECC is %02x, cmd is %02x", ecc, (*(cmd + i)));
    ecc = ecc ^ (*(cmd + i));
  }
  LOGI("ECC is %02x", ecc);
  return ecc;
}