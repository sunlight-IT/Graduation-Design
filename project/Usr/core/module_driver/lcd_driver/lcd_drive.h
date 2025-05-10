#pragma once
#include <stdint.h>

#include "stm32f1xx_hal.h"

#define FSMC_LCD 1  // 1:娴ｈ法鏁?FSMC閹恒儱褰涢敍锟?0:娴ｈ法鏁?SPI閹恒儱褰?

#define BUF_SIZE 256

#define __CONNECT(a, b) a##b
#define CONNECT(a, b)   __CONNECT(a, b)

#if FSMC_LCD
#define FSMC_BANK1_BASE               ((uint32_t)0x60000000)  // FSMC基地址
#define FSMC_BANK1_NORSRAM_OFFSET_NE1 ((uint32_t)0x00 << 24)  // 不同片选控制地址
#define FSMC_BANK1_NORSRAM_OFFSET_NE2 ((uint32_t)0x04 << 24)
#define FSMC_BANK1_NORSRAM_OFFSET_NE3 ((uint32_t)0x08 << 24)
#define FSMC_BANK1_NORSRAM_OFFSET_NE4 ((uint32_t)0x0C << 24)

#define FSMC_BANK_NORSRAM4 ((FSMC_BANK1_BASE) | FSMC_BANK1_NORSRAM_OFFSET_NE4)  // 选择NE4片选控制
// FSMC_Bank1_NORSRAM
#define FSMC_Addr_LCD_CMD (FSMC_BANK_NORSRAM4 | (((uint32_t)0x00) << 11))

// FSMC_Bank1_NORSRAM
#define FSMC_Addr_LCD_DATA (FSMC_BANK_NORSRAM4 | (((uint32_t)0x01) << 11))
#endif

// 鐎规矮绠烲CD閻ㄥ嫬鏄傞敓锟??
#define LCD_W 240
#define LCD_H 320

// LCD闁插稄鎷??閿熻棄寮?閺佷即娉?
typedef struct {
  uint16_t width;    // LCD 鐎硅棄瀹?
  uint16_t height;   // LCD 妤傛ê瀹?
  uint16_t id;       // LCD ID
  uint8_t  dir;      // 閿燂拷?鐏炲繗绻曢敓锟??缁旀牕鐫嗛幒褍鍩楅敓锟??0閿涘瞼鐝?鐏炲骏绱?1閿涘本铆鐏炲骏鎷??
  uint16_t wramcmd;  // 瀵?鈧?婵?瀣?鍟揼ram閹稿洣鎶?
  uint16_t setxcmd;  // 鐠佸墽鐤唜閸ф劖鐖ｉ幐鍥︽姢
  uint16_t setycmd;  // 鐠佸墽鐤唝閸ф劖鐖ｉ幐鍥︽姢
} _lcd_dev;

typedef enum {
  k_zero = 0,
  k_ninety,              // 90
  k_hundred_eighty,      // 180
  k_two_hundred_seventy  // 270
} lcd_direct_degress_t;

// 閻㈣崵鐟?妫版粏澹?
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
#define BROWN   0XBC40  // 濡?鏇″??
#define BRRED   0XFC07  // 濡?鏇犲?╅敓锟??
#define GRAY    0X8430  // 閻忔媽澹?
// GUI妫版粏澹?

#define DARKBLUE  0X01CF  // 濞ｈ精鎽戦敓锟??
#define LIGHTBLUE 0X7D7C  // 濞村懓鎽戦敓锟??
#define GRAYBLUE  0X5458  // 閻忔媽鎽戦敓锟??
// 娴犮儰绗傛稉澶庡?婃稉绡淎NEL閻ㄥ嫸鎷??閿熷€熷??

#define LIGHTGREEN 0X841F  // 濞村懐璞㈤敓锟??
#define LIGHTGRAY  0XEF5B  // 濞村懐浼嗛敓锟??(PANNEL)
#define LGRAY      0XC618  // 濞村懐浼嗛敓锟??(PANNEL),缁愭ぞ缍嬮懗灞炬珯閿燂拷?

#define LGRAYBLUE 0XA651  // 濞村懐浼嗛拑婵婂??(閿燂拷?闂傛潙鐪版０婊嗗??)
#define LBBLUE    0X2B12  // 濞村拑鎷??閿熷€熸憫閿燂拷?(闁?澶嬪?ㄩ弶锛勬窗閻ㄥ嫬寮介敓锟??)

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
