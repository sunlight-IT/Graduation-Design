#include "syn6288_driver.h"

#include "component/component.h"
#include "string.h"

#define TAG           "syn6288 driver"
#define DEBUG         1
#define UART2_TIMEOUT 100

static uint8_t set_parame(uint8_t high, uint8_t low);
static uint8_t calculate_ecc(const uint8_t* cmd, uint8_t data_len);

static UART_HandleTypeDef* m_uart;
static cmd_frame_t         cmd_frame_cache;

static uint8_t  rx_buf[BUF_SIZE];
static uint8_t  rx_cache;
static uint8_t  recv_back;
static uint8_t  size;
static uint32_t ticktime;

void syn_handle_uart(UART_HandleTypeDef* h_uart) {
  m_uart = h_uart;
  HAL_UART_Receive_IT(m_uart, &rx_cache, 1);
}

uint8_t* get_rx_cache(void) { return &rx_cache; }

void syn_recv(void) {
  if (size >= BUF_SIZE) {
    ZLOGE(TAG, "rxbuf overflow %d", size);
    return;
  }

  rx_buf[size] = rx_cache;
  size++;
  ticktime = HAL_GetTick();
}

// 发送串口数据必须是一块连续的存储空间，因为发送的串口指针只能按顺序递增
static uint8_t cmd_frame_pack_txt(uint8_t word, uint8_t background_music, uint8_t txt_code, const uint8_t* txt) {
  uint8_t* p_cmd_frame   = (uint8_t*)&cmd_frame_cache;
  uint16_t cmd_frame_len = 0;
  uint8_t  str_len       = strlen(txt);
  if (str_len > 200) {
    ZLOGE(TAG, "txt strlen %d > 200", str_len);
    return;
  }

  cmd_frame_cache.head       = 0xfd;
  cmd_frame_cache.data_len_h = 0;
  cmd_frame_cache.data_len_l = (1 + 1 + str_len + 1);  // word + parame + str_len + ecc
  cmd_frame_cache.word       = word;

  cmd_frame_cache.parame = set_parame(background_music, txt_code);
  strncpy(cmd_frame_cache.text, txt, str_len);
  cmd_frame_len                 = 3 + cmd_frame_cache.data_len_l;
  cmd_frame_cache.text[str_len] = calculate_ecc((uint8_t*)&cmd_frame_cache, cmd_frame_len - 1);

#if DEBUG
  for (int i = 0; i < cmd_frame_len; i++) {
    ZLOGI(TAG, "%02x", ((uint8_t*)(&cmd_frame_cache))[i]);
  }
#endif

  return cmd_frame_len;  // head + str_len + data_len
}

static void cmd_frame_send_txt(uint8_t word, uint8_t background_music, uint8_t txt_code, const uint8_t* txt) {
  uint8_t frame_len = cmd_frame_pack_txt(word, background_music, txt_code, txt);
  if (HAL_UART_Transmit(m_uart, (uint8_t*)(&cmd_frame_cache), frame_len, 100)) {
    ZLOGE(TAG, "send error");
  }
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
      ZLOGI(TAG, "Init success %d", recv_back);
      break;
    case kRecvSuccess:
      ZLOGI(TAG, "Receive sucess %d", recv_back);
      break;
    case kRecvFail:
      ZLOGE(TAG, "Receive fail %d", recv_back);
      break;
    case kRecvQuery:
      ZLOGI(TAG, "query status %d", recv_back);
      break;
    case kEmpty:
      ZLOGI(TAG, "chip empty %d", recv_back);
      break;
    default:
      ZLOGE(TAG, "Receive error %d", recv_back);
      break;
  }
}

bool syn_recv_ack(void) {
  while (1) {
    if (size != 0 && has_pass_time(ticktime) < UART2_TIMEOUT) {
      size      = 0;
      recv_back = rx_buf[0];
      return true;
    } else if (has_pass_time(ticktime) > UART2_TIMEOUT) {
      ZLOGE(TAG, "Time out");
      return false;
    }
  }
}

void voice_compound_cmd(uint8_t* txt) {
  cmd_frame_send_txt((uint8_t)kVoiceCompoundWord, 0, 0, txt);
  if (syn_recv_ack()) {
    recv_back_process();
  }
}

void syn6288_inquiry(void) {
  uint8_t frame_len = cmd_frame_pack_txt(kSeacherStatusWord, 0, 0, NULL);
  if (HAL_UART_Transmit(m_uart, (uint8_t*)(&cmd_frame_cache), frame_len, 10)) {
    ZLOGE(TAG, "send error");
  }
  if (syn_recv_ack()) {
    recv_back_process();
  }
}

uint8_t set_parame(uint8_t high, uint8_t low) {
  uint8_t parame = (high << 4);
  parame |= low;
}

uint8_t calculate_ecc(const uint8_t* cmd, uint8_t data_len) {
  uint8_t ecc = 0;
  for (int i = 0; i < data_len; i++) {
    ecc ^= cmd[i];
  }
  return ecc;
}