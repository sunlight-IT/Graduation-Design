#pragma once
#include <stdint.h>

#include "stm32f1xx_hal.h"
#define BUF_SIZE 256

// 定义LCD的尺�?
#define LCD_W 240
#define LCD_H 320

// LCD重�?�参数集
typedef struct {
  uint16_t width;    // LCD 宽度
  uint16_t height;   // LCD 高度
  uint16_t id;       // LCD ID
  uint8_t  dir;      // �?屏还�?竖屏控制�?0，竖屏；1，横屏�?
  uint16_t wramcmd;  // 开始写gram指令
  uint16_t setxcmd;  // 设置x坐标指令
  uint16_t setycmd;  // 设置y坐标指令
} _lcd_dev;

typedef enum {
  k_zero = 0,
  k_ninety,              // 90
  k_hundred_eighty,      // 180
  k_two_hundred_seventy  // 270
} lcd_direct_degress_t;

// 画笔颜色
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40  // 棕色
#define BRRED   0XFC07  // 棕红�?
#define GRAY    0X8430  // 灰色
// GUI颜色

#define DARKBLUE  0X01CF  // 深蓝�?
#define LIGHTBLUE 0X7D7C  // 浅蓝�?
#define GRAYBLUE  0X5458  // 灰蓝�?
// 以上三色为PANEL的�?�色

#define LIGHTGREEN 0X841F  // 浅绿�?
#define LIGHTGRAY  0XEF5B  // 浅灰�?(PANNEL)
#define LGRAY      0XC618  // 浅灰�?(PANNEL),窗体背景�?

#define LGRAYBLUE 0XA651  // 浅灰蓝色(�?间层颜色)
#define LBBLUE    0X2B12  // 浅�?�蓝�?(选择条目的反�?)

void lcd_init(void);
void lcd_handle_reg(SPI_HandleTypeDef* hspi);
void lcd_set_cursor(uint16_t Xpos, uint16_t Ypos);
void lcd_set_windows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void lcd_write_data_16(uint16_t word);
// void lcd_draw_point(uint16_t x, uint16_t y);
void lcd_write_ram_prepare(void);
void lcd_direction(uint8_t direction);

void lcd_show_char(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode);
void lcd_show_string(uint16_t x, uint16_t y, uint8_t size, uint8_t* p, uint8_t mode);
void lcd_show_num(uint16_t x, uint16_t y, int32_t num, uint8_t len, uint8_t size);

void lcd_draw_point(uint16_t x, uint16_t y);
void lcd_draw_point_clor(uint16_t x, uint16_t y, uint16_t clor);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_draw_cross(uint16_t x1, uint16_t y1, uint16_t length);
void lcd_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* p);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height);

void test(void);

void LCD_RESET();
