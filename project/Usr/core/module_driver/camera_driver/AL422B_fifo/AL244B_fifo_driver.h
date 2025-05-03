#pragma once
#include <stdbool.h>

#include "fifo_gpio.h"

#define CAMERA_WIDTH  120
#define CAMERA_HEIGHT 120

#define FIFO_RD_PREPARE \
  do {                  \
    FIFO_RRST_L;        \
    FIFO_RCLK_L;        \
    FIFO_RCLK_H;        \
    FIFO_RRST_H;        \
    FIFO_RCLK_L;        \
    FIFO_RCLK_H;        \
  } while (0)

// RCLK 一个周期完成一个字节数据
#define FIFO_READ(RGB565)                            \
  do {                                               \
    RGB565 = 0;                                      \
    FIFO_RCLK_L;                                     \
    RGB565 = (OV7725_DATA_GPIO_Port->IDR & 0x00ff);  \
    RGB565 <<= 8;                                    \
    FIFO_RCLK_H;                                     \
    FIFO_RCLK_L;                                     \
    RGB565 |= (OV7725_DATA_GPIO_Port->IDR) & 0x00ff; \
    FIFO_RCLK_H;                                     \
  } while (0)

typedef uint8_t (*pic_data)[CAMERA_HEIGHT];

void ov7725_fifo_init(void);

void clear_vsync(void);

void vsync_exit(void);

uint16_t get_picture_data(uint16_t x, uint16_t y);
pic_data get_picture(void);
void     pic_recv(void);
uint8_t  get_vsync(void);
bool     get_pic_state(void);
void     clear_pic_state(void);
