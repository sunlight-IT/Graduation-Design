#pragma once
#pragma
#include <stdint.h>

#include "gpio.h"
#include "stdbool.h"

#define TEXT_SIZE 200
#define BUF_SIZE  256

// 该结构体存在字节对齐问题，uint16为2字节，要对起到2字节的偏移，就导致head和data_len中间多了一个空字节来补齐data_len需要对齐的字节
typedef struct {
  uint8_t head;
  uint8_t data_len_h;
  uint8_t data_len_l;
  uint8_t word;
  uint8_t parame;
  uint8_t text[TEXT_SIZE];
} cmd_frame_t;

typedef enum {
  kVoiceCompoundWord   = 0x01,
  kStopCompoundWord    = 0x02,
  kPauseCompoundWord   = 0x03,
  kRecoverCompoundWord = 0x04,
  kSeacherStatusWord   = 0x21,
  kBaudRateWord        = 0x31,
  kEnterPowerDownWord  = 0x88,
} CmdWordEnum;

typedef enum {
  kInitSuccess = 0x4A,
  kRecvSuccess = 0x41,
  kRecvFail    = 0x45,
  kRecvQuery   = 0x4E,
  kEmpty       = 0x4F,
} RecvBackEnum;

typedef enum {
  kGB2321 = 0x00,
  kGBK,
  kBIG5,
  kUNICODE,
} CmdParameTextCodeEnum;

typedef enum {
  k9600 = 0x00,
  k19200,
  k38400,
} CmdParameBaudEnum;

void     voice_compound_cmd(uint8_t* txt);
void     voice_baud_cmd(void);
void     voice_inquire_cmd(void);
void     syn6288_inquiry(void);
void     syn_handle_uart(UART_HandleTypeDef* h_uart);
void     syn_recv(void);
uint8_t* get_rx_cache(void);
