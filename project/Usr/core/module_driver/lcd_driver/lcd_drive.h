#pragma once
#include <stdint.h>

#include "stm32f1xx_hal.h"

#define FSMC_LCD 1  // 1:浣跨�?FSMC鎺ュ彛锛�?0:浣跨�?SPI鎺ュ�?

#define BUF_SIZE 256

#define __CONNECT(a, b) a##b
#define CONNECT(a, b)   __CONNECT(a, b)

#if FSMC_LCD
#define FSMC_BANK1_BASE               ((uint32_t)0x60000000)
#define FSMC_BANK1_NORSRAM_OFFSET_NE1 ((uint32_t)0x00 << 24)
#define FSMC_BANK1_NORSRAM_OFFSET_NE2 ((uint32_t)0x04 << 24)
#define FSMC_BANK1_NORSRAM_OFFSET_NE3 ((uint32_t)0x08 << 24)
#define FSMC_BANK1_NORSRAM_OFFSET_NE4 ((uint32_t)0x0C << 24)

#define FSMC_BANK_NORSRAM4 ((FSMC_BANK1_BASE) | FSMC_BANK1_NORSRAM_OFFSET_NE4)
// FSMC_Bank1_NORSRAM
#define FSMC_Addr_LCD_CMD (FSMC_BANK_NORSRAM4 | (((uint32_t)0x00) << 11))

// FSMC_Bank1_NORSRAM
#define FSMC_Addr_LCD_DATA (FSMC_BANK_NORSRAM4 | (((uint32_t)0x01) << 11))
#endif

// 瀹氫箟LCD鐨勫昂锟�??
#define LCD_W 240
#define LCD_H 320

// LCD閲嶏�??锟藉�?鏁伴�?
typedef struct {
  uint16_t width;    // LCD 瀹藉�?
  uint16_t height;   // LCD 楂樺�?
  uint16_t id;       // LCD ID
  uint8_t  dir;      // 锟�?灞忚繕锟�??绔栧睆鎺у埗锟�??0锛岀�?灞忥�?1锛屾í灞忥�??
  uint16_t wramcmd;  // �?�?�?�?啓gram鎸囦�?
  uint16_t setxcmd;  // 璁剧疆x鍧愭爣鎸囦护
  uint16_t setycmd;  // 璁剧疆y鍧愭爣鎸囦护
} _lcd_dev;

typedef enum {
  k_zero = 0,
  k_ninety,              // 90
  k_hundred_eighty,      // 180
  k_two_hundred_seventy  // 270
} lcd_direct_degress_t;

// 鐢荤�?棰滆�?
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
#define BROWN   0XBC40  // �?曡�??
#define BRRED   0XFC07  // �?曠�?�锟�??
#define GRAY    0X8430  // 鐏拌�?
// GUI棰滆�?

#define DARKBLUE  0X01CF  // 娣辫摑锟�??
#define LIGHTBLUE 0X7D7C  // 娴呰摑锟�??
#define GRAYBLUE  0X5458  // 鐏拌摑锟�??
// 浠ヤ笂涓夎�?�涓篜ANEL鐨勶�??锟借�??

#define LIGHTGREEN 0X841F  // 娴呯豢锟�??
#define LIGHTGRAY  0XEF5B  // 娴呯伆锟�??(PANNEL)
#define LGRAY      0XC618  // 娴呯伆锟�??(PANNEL),绐椾綋鑳屾櫙锟�?

#define LGRAYBLUE 0XA651  // 娴呯伆钃濊�??(锟�?闂村眰棰滆�??)
#define LBBLUE    0X2B12  // 娴咃�??锟借摑锟�?(�?夋�?�鏉＄洰鐨勫弽锟�??)

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
