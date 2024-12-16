#include "component/component.h"
#include "lcd_drive.h"
#include "lcd_gpio.h"

#define TAG "LCD_DRIVER"

static SPI_HandleTypeDef* m_spi;
static HAL_StatusTypeDef  info;
static uint8_t            buf_spi_send[BUF_SIZE];
static uint8_t            buf_spi_receive[BUF_SIZE];
static _lcd_dev           lcddev;
static uint8_t            data_8;
static uint16_t           data_16;

static uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
static uint16_t DeviceCode;

void lcd_handle_reg(SPI_HandleTypeDef* hspi) { m_spi = hspi; }

void LCD_RESET(void) {
  LCD_RST_DOWN;
  HAL_Delay(100);
  LCD_RST_UP;
  HAL_Delay(50);
}

void LCD_WR_REG(uint8_t byte) {
  data_8 = byte;
  LCD_CS_DOWN;
  LCD_RS_DOWN;
  if (HAL_SPI_Transmit(m_spi, &data_8, 1, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
}

uint8_t LCD_RD_DATA(void) {
  uint8_t data = 0;
  LCD_CS_DOWN;
  if (HAL_SPI_Receive(m_spi, &data, 1, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
  return data;
}

void test(void) {
  uint16_t id;
  LCD_WR_REG(0xD3);
  id = LCD_RD_DATA();  // empty
  id = LCD_RD_DATA();
  id = LCD_RD_DATA() << 8;
  id |= LCD_RD_DATA();
  ZLOGI(TAG, "id is : %04x", id);
}

void LCD_WR_DATA(uint8_t byte) {
  data_8 = byte;
  LCD_CS_DOWN;
  LCD_RS_UP;
  if (HAL_SPI_Transmit(m_spi, &data_8, 1, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
}

void lcd_write_reg(uint8_t reg_addr, uint16_t val) {
  LCD_WR_REG(reg_addr);
  LCD_WR_DATA(val);
}

void lcd_write_ram_prepare(void) { LCD_WR_REG(lcddev.wramcmd); }

void lcd_write_data_16(uint16_t word) {
  data_16 = word;
  LCD_CS_DOWN;
  LCD_RS_UP;
  if (HAL_SPI_Transmit(m_spi, (uint8_t*)&data_16, 2, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
}

void lcd_direction(uint8_t direction) {
  lcddev.setxcmd = 0x2A;
  lcddev.setycmd = 0x2B;
  lcddev.wramcmd = 0x2C;
  switch (direction) {
    case 0:
      lcddev.width  = LCD_W;
      lcddev.height = LCD_H;
      lcd_write_reg(0x36, (1 << 3) | (0 << 6) | (0 << 7));
      break;
    case 1:
      lcddev.width  = LCD_H;
      lcddev.height = LCD_W;
      lcd_write_reg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
      break;
    case 2:
      lcddev.width  = LCD_W;
      lcddev.height = LCD_H;
      lcd_write_reg(0x36, (1 << 3) | (1 << 6) | (1 << 7));  // BGR==1,MY==0,MX==0,MV==0
      break;
    case 3:
      lcddev.width  = LCD_H;
      lcddev.height = LCD_W;
      lcd_write_reg(0x36, (1 << 3) | (1 << 7) | (1 << 5));  // BGR==1,MY==1,MX==0,MV==1
      break;
    default:
      break;
  }
}

void lcd_set_windows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd) {
  LCD_WR_REG(lcddev.setxcmd);
  LCD_WR_DATA(xStar >> 8);
  LCD_WR_DATA(0x00FF & xStar);
  LCD_WR_DATA(xEnd >> 8);
  LCD_WR_DATA(0x00FF & xEnd);

  LCD_WR_REG(lcddev.setycmd);
  LCD_WR_DATA(yStar >> 8);
  LCD_WR_DATA(0x00FF & yStar);
  LCD_WR_DATA(yEnd >> 8);
  LCD_WR_DATA(0x00FF & yEnd);

  lcd_write_ram_prepare();  // 开始写入GRAM
}

// void lcd_read_identification(void) { LCD_WR_REG(0x04); }

void lcd_clear(uint16_t color) {
  unsigned int i, m;
  lcd_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
  LCD_CS_DOWN;
  LCD_RS_UP;
  for (i = 0; i < lcddev.height; i++) {
    for (m = 0; m < lcddev.width; m++) {
      lcd_write_data_16(color);
    }
  }
  LCD_CS_UP;
}

void lcd_init(void) {
  LCD_RESET();
  /*  Power control B (CFh)  */
  LCD_WR_REG(0xCF);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0xC9);  // C1
  LCD_WR_DATA(0X30);
  /*  Power on sequence control (EDh) */
  LCD_WR_REG(0xED);
  LCD_WR_DATA(0x64);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0X12);
  LCD_WR_DATA(0X81);
  /*  Driver timing control A (E8h) */
  LCD_WR_REG(0xE8);
  LCD_WR_DATA(0x85);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x7A);  // 野火是0x78

  /*  Power control A (CBh) */
  LCD_WR_REG(0xCB);
  LCD_WR_DATA(0x39);
  LCD_WR_DATA(0x2C);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x34);
  LCD_WR_DATA(0x02);

  /* Pump ratio control (F7h) */
  LCD_WR_REG(0xF7);
  LCD_WR_DATA(0x20);
  /* Driver timing control B */
  LCD_WR_REG(0xEA);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);

  /* Power Control 1 (C0h) */
  LCD_WR_REG(0xC0);   // Power control
  LCD_WR_DATA(0x1B);  // VRH[5:0]  野火是0x21

  /* Power Control 2 (C1h) */
  LCD_WR_REG(0xC1);   // Power control
  LCD_WR_DATA(0x00);  // SAP[2:0];BT[3:0] 01 野火是0x11

  /* VCOM Control 1 (C5h) */
  LCD_WR_REG(0xC5);   // VCM control
  LCD_WR_DATA(0x30);  // 3F 野火是0x2d
  LCD_WR_DATA(0x30);  // 3C  野火是0x33

  /*  VCOM Control 2 (C7h)  */
  LCD_WR_REG(0xC7);   // VCM control2
  LCD_WR_DATA(0XB7);  // 野火是0xc0

  /* memory access control set */
  LCD_WR_REG(0x36);   // Memory Access Control
  LCD_WR_DATA(0x08);  // 野火是0x00

  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x55);

  /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
  LCD_WR_REG(0xB1);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x1A);  // 野火是0x17

  /*  Display Function Control (B6h) */
  LCD_WR_REG(0xB6);  // Display Function Control
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0xA2);

  /*野火里加进来的*/
  // ILI9341_Write_Cmd(0xF6);
  // ILI9341_Write_Data(0x01);
  // ILI9341_Write_Data(0x30);
  // LCD_WR_REG(0xF6);
  // LCD_WR_DATA(0x01);
  // LCD_WR_DATA(0x30);
  /*****************/
  /* Enable 3G (F2h) */
  LCD_WR_REG(0xF2);  // 3Gamma Function Disable
  LCD_WR_DATA(0x00);

  /* Gamma Set (26h) */
  LCD_WR_REG(0x26);  // Gamma curve selected
  LCD_WR_DATA(0x01);

  /* Positive Gamma Correction */
  LCD_WR_REG(0xE0);  // Set Gamma
  LCD_WR_DATA(0x0F);
  LCD_WR_DATA(0x2A);
  LCD_WR_DATA(0x28);
  LCD_WR_DATA(0x08);
  LCD_WR_DATA(0x0E);
  LCD_WR_DATA(0x08);
  LCD_WR_DATA(0x54);
  LCD_WR_DATA(0XA9);
  LCD_WR_DATA(0x43);
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0x0F);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);

  /* Negative Gamma Correction (E1h) */
  LCD_WR_REG(0XE1);  // Set Gamma
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x15);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x07);
  LCD_WR_DATA(0x11);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x2B);
  LCD_WR_DATA(0x56);
  LCD_WR_DATA(0x3C);
  LCD_WR_DATA(0x05);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x0F);
  LCD_WR_DATA(0x3F);
  LCD_WR_DATA(0x3F);
  LCD_WR_DATA(0x0F);

  LCD_WR_REG(0x2B);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x01);
  LCD_WR_DATA(0x3f);
  LCD_WR_REG(0x2A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0xef);

  LCD_WR_REG(0x11);  // Exit Sleep
  HAL_Delay(120);
  LCD_WR_REG(0x29);  // display on

  lcd_direction(k_two_hundred_seventy);
  LCD_LED_UP;
  lcd_clear(BLACK);
}

void lcd_set_cursor(uint16_t Xpos, uint16_t Ypos) { lcd_set_windows(Xpos, Ypos, Xpos, Ypos); }

uint16_t lcd_read_point_rgb(uint16_t x, uint16_t y) {
  uint16_t r = 0, g = 0, b = 0;
  if (x > lcddev.width || y > lcddev.height) return 0;
  lcd_set_cursor(x, y);

  LCD_WR_REG(0x0e);
  r = LCD_RD_DATA();
}