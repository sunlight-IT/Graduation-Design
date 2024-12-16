#pragma once
#include "fifo_gpio.h"

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
#define FIFO_READ(RGB565)                                 \
  do {                                                    \
    RGB565 = 0;                                           \
    FIFO_RCLK_L;                                          \
    RGB565 = (OV7725_DATA_GPIO_Port->IDR) & 0xff00;       \
    FIFO_RCLK_H;                                          \
    FIFO_RCLK_L;                                          \
    RGB565 |= (OV7725_DATA_GPIO_Port->IDR >> 8) & 0x00ff; \
    FIFO_RCLK_H;                                          \
  } while (0)

void    ov7725_fifo_init(void);
void    clear_vsync(void);
uint8_t get_vsync(void);
