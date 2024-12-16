#pragma once
#include <stdbool.h>
#include <stdint.h>

#define PIC_W 120
#define PIC_H 120

#define RGB_SHOW_X 0
#define RGB_SHOW_Y 0
// lcd屏幕显示的图像不能重叠
#define BIN_SHOW_X PIC_W + RGB_SHOW_X + 1
#define BIN_SHOW_Y 0

typedef struct {
  uint8_t r;

  uint8_t g;

  uint8_t b;
} rgb_t;

// 色相(H)、饱和度(S)、明度(L)颜色格式结构体
typedef struct {
  uint8_t h;  // [0,240] 色调

  uint8_t s;  // [0,240] 饱和度

  uint8_t l;  // [0,240] 亮度
} hsl_t;

bool image_get(uint8_t sx, uint8_t sy, uint16_t width, uint16_t height);
void trace_picture(void);
void image_display(void);

uint16_t get_picture_data(uint16_t x, uint16_t y);
uint16_t get_picture_data_bina(uint16_t x, uint16_t y);