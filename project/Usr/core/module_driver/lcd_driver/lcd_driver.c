#include "FONT.H"
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

#if FSMC_LCD
static void LCD_Delay(__IO uint32_t nCount) { for (; nCount != 0; nCount--); }

static __inline void LCD_WR_CMD(uint16_t uscmd) { *(__IO uint16_t*)(FSMC_Addr_LCD_CMD) = uscmd; }

static __inline void LCD_WR_DATA(uint16_t usdata) { *(__IO uint16_t*)(FSMC_Addr_LCD_DATA) = usdata; }

static __inline uint16_t LCD_RD_DATA(void) { return (*(__IO uint16_t*)(FSMC_Addr_LCD_DATA)); }

void lcd_write_data_16(uint16_t word) { LCD_WR_DATA(word); }

void lcd_clear(uint16_t color) {
  unsigned int i, m;
  lcd_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);
  for (i = 0; i < lcddev.height; i++) {
    for (m = 0; m < lcddev.width; m++) {
      lcd_write_data_16(color);
    }
  }
}
#else
void LCD_WR_CMD(uint8_t byte) {
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
void LCD_WR_DATA(uint8_t byte) {
  data_8 = byte;
  LCD_CS_DOWN;
  LCD_RS_UP;
  if (HAL_SPI_Transmit(m_spi, &data_8, 1, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
}

void lcd_write_data_16(uint16_t word) {
  data_16 = word;
  LCD_CS_DOWN;
  LCD_RS_UP;
  if (HAL_SPI_Transmit(m_spi, (uint8_t*)&data_16, 2, 10)) {
    ErrorHanding(TAG, "HAL_SPI_Transmit error");
  }
  LCD_CS_UP;
}

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
#endif
//  * FSMC_NOE   :LCD-RD  PD4
//  * FSMC_NWE   :LCD-WR PD5
//  * FSMC_NE1   :LCD-CS  PD7
//  * FSMC_A18   :LCD-DC PD13 RS

void lcd_handle_reg(SPI_HandleTypeDef* hspi) { m_spi = hspi; }

void LCD_RESET(void) {
  LCD_RST_DOWN;
  HAL_Delay(100);
  LCD_RST_UP;
  HAL_Delay(50);
}

void test(void) {
  uint16_t id = 0;
  LCD_WR_CMD(0xD3);
  LCD_RD_DATA();  // empty
  id = LCD_RD_DATA();
  id = LCD_RD_DATA() << 8;
  id |= LCD_RD_DATA();
  ZLOGI(TAG, "id is : %04x", id);
}

void lcd_write_reg(uint8_t reg_addr, uint16_t val) {
  LCD_WR_CMD(reg_addr);
  LCD_WR_DATA(val);
}

void lcd_write_ram_prepare(void) { LCD_WR_CMD(lcddev.wramcmd); }

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
  LCD_WR_CMD(lcddev.setxcmd);
  LCD_WR_DATA(xStar >> 8);
  LCD_WR_DATA(0x00FF & xStar);
  LCD_WR_DATA(xEnd >> 8);
  LCD_WR_DATA(0x00FF & xEnd);

  LCD_WR_CMD(lcddev.setycmd);
  LCD_WR_DATA(yStar >> 8);
  LCD_WR_DATA(0x00FF & yStar);
  LCD_WR_DATA(yEnd >> 8);
  LCD_WR_DATA(0x00FF & yEnd);

  lcd_write_ram_prepare();  // 开始写入GRAM
}

// void lcd_read_identification(void) { LCD_WR_CMD(0x04); }

void lcd_init(void) {
  test();
  // LCD_RESET();
  /*  Power control B (CFh)  */
  LCD_WR_CMD(0xCF);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0xC9);  // C1
  LCD_WR_DATA(0X30);
  /*  Power on sequence control (EDh) */
  LCD_WR_CMD(0xED);
  LCD_WR_DATA(0x64);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0X12);
  LCD_WR_DATA(0X81);
  /*  Driver timing control A (E8h) */
  LCD_WR_CMD(0xE8);
  LCD_WR_DATA(0x85);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x7A);  // 野火是0x78

  /*  Power control A (CBh) */
  LCD_WR_CMD(0xCB);
  LCD_WR_DATA(0x39);
  LCD_WR_DATA(0x2C);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x34);
  LCD_WR_DATA(0x02);

  /* Pump ratio control (F7h) */
  LCD_WR_CMD(0xF7);
  LCD_WR_DATA(0x20);
  /* Driver timing control B */
  LCD_WR_CMD(0xEA);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);

  /* Power Control 1 (C0h) */
  LCD_WR_CMD(0xC0);   // Power control
  LCD_WR_DATA(0x1B);  // VRH[5:0]  野火是0x21

  /* Power Control 2 (C1h) */
  LCD_WR_CMD(0xC1);   // Power control
  LCD_WR_DATA(0x00);  // SAP[2:0];BT[3:0] 01 野火是0x11

  /* VCOM Control 1 (C5h) */
  LCD_WR_CMD(0xC5);   // VCM control
  LCD_WR_DATA(0x30);  // 3F 野火是0x2d
  LCD_WR_DATA(0x30);  // 3C  野火是0x33

  /*  VCOM Control 2 (C7h)  */
  LCD_WR_CMD(0xC7);   // VCM control2
  LCD_WR_DATA(0XB7);  // 野火是0xc0

  /* memory access control set */
  LCD_WR_CMD(0x36);   // Memory Access Control
  LCD_WR_DATA(0x08);  // 野火是0x00

  LCD_WR_CMD(0x3A);
  LCD_WR_DATA(0x55);

  /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
  LCD_WR_CMD(0xB1);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x1A);  // 野火是0x17

  /*  Display Function Control (B6h) */
  LCD_WR_CMD(0xB6);  // Display Function Control
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0xA2);

  /*野火里加进来的*/
  // ILI9341_Write_Cmd(0xF6);
  // ILI9341_Write_Data(0x01);
  // ILI9341_Write_Data(0x30);
  // LCD_WR_CMD(0xF6);
  // LCD_WR_DATA(0x01);
  // LCD_WR_DATA(0x30);
  /*****************/
  /* Enable 3G (F2h) */
  LCD_WR_CMD(0xF2);  // 3Gamma Function Disable
  LCD_WR_DATA(0x00);

  /* Gamma Set (26h) */
  LCD_WR_CMD(0x26);  // Gamma curve selected
  LCD_WR_DATA(0x01);

  /* Positive Gamma Correction */
  LCD_WR_CMD(0xE0);  // Set Gamma
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
  LCD_WR_CMD(0XE1);  // Set Gamma
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

  LCD_WR_CMD(0x2B);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x01);
  LCD_WR_DATA(0x3f);
  LCD_WR_CMD(0x2A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0xef);

  LCD_WR_CMD(0x11);  // Exit Sleep
  HAL_Delay(120);
  LCD_WR_CMD(0x29);  // display on

  lcd_direction(k_two_hundred_seventy);
  LCD_LED_DOWN;
  lcd_clear(RED);
}

void lcd_set_cursor(uint16_t Xpos, uint16_t Ypos) { lcd_set_windows(Xpos, Ypos, Xpos, Ypos); }

uint16_t lcd_read_point_rgb(uint16_t x, uint16_t y) {
  uint16_t r = 0, g = 0, b = 0;
  if (x > lcddev.width || y > lcddev.height) return 0;
  lcd_set_cursor(x, y);

  LCD_WR_CMD(0x0e);
  r = LCD_RD_DATA();
}

void lcd_show_char(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t num, uint8_t size, uint8_t mode) {
  uint8_t  temp;
  uint8_t  pos, t;
  uint16_t colortemp = POINT_COLOR;

  num = num - ' ';                                        // 得到偏移后的值
  lcd_set_windows(x, y, x + size / 2 - 1, y + size - 1);  // 设置单个文字显示窗口
  if (!mode)                                              // 非叠加方式
  {
    for (pos = 0; pos < size; pos++) {
      if (size == 12)
        temp = asc2_1206[num][pos];  // 调用1206字体
      else
        temp = asc2_1608[num][pos];  // 调用1608字体
      for (t = 0; t < size / 2; t++) {
        if (temp & 0x01)
          lcd_write_data_16(fc);
        else
          lcd_write_data_16(bc);
        temp >>= 1;
      }
    }
  } else  // 叠加方式
  {
    for (pos = 0; pos < size; pos++) {
      if (size == 12)
        temp = asc2_1206[num][pos];  // 调用1206字体
      else
        temp = asc2_1608[num][pos];  // 调用1608字体
      for (t = 0; t < size / 2; t++) {
        POINT_COLOR = fc;
        if (temp & 0x01) lcd_draw_point(x + t, y + pos);  // 画一个点
        temp >>= 1;
      }
    }
  }
  POINT_COLOR = colortemp;
  lcd_set_windows(0, 0, lcddev.width - 1, lcddev.height - 1);  // 恢复窗口为全屏
}

void lcd_show_string(uint16_t x, uint16_t y, uint8_t size, uint8_t* p, uint8_t mode) {
  while ((*p <= '~') && (*p >= ' '))  // 判断是不是非法字符!
  {
    if (x > (lcddev.width - 1) || y > (lcddev.height - 1)) return;
    lcd_show_char(x, y, POINT_COLOR, BACK_COLOR, *p, size, mode);
    x += size / 2;
    p++;
  }
}

static uint32_t mypow(uint8_t m, uint8_t n) {
  uint32_t result = 1;
  while (n--) result *= m;
  return result;
}

void lcd_show_num(uint16_t x, uint16_t y, int32_t num, uint8_t len, uint8_t size) {
  uint8_t t, temp;
  uint8_t enshow = 0;
  uint8_t signaled_x;
  if (num < 0) {
    num        = -num;
    signaled_x = x + (size / 2);
    lcd_show_char(x, y, POINT_COLOR, BACK_COLOR, '-', size, 0);

  } else {
    signaled_x = x;
  }
  for (t = 0; t < len; t++) {
    temp = (num / mypow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1)) {
      if (temp == 0) {
        lcd_show_char(signaled_x + (size / 2) * t, y, POINT_COLOR, BACK_COLOR, ' ', size, 0);
        continue;
      } else
        enshow = 1;
    }
    lcd_show_char(signaled_x + (size / 2) * t, y, POINT_COLOR, BACK_COLOR, temp + '0', size, 0);
  }
}

void lcd_draw_point(uint16_t x, uint16_t y) {
  lcd_set_cursor(x, y);  // 设置光标位置
  lcd_write_data_16(POINT_COLOR);
}

void lcd_draw_point_clor(uint16_t x, uint16_t y, uint16_t clor) {
  lcd_set_cursor(x, y);  // 设置光标位置
  lcd_write_data_16(clor);
}

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint16_t t;
  int      xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int      incx, incy, uRow, uCol;

  delta_x = x2 - x1;  // 计算坐标增量
  delta_y = y2 - y1;
  uRow    = x1;
  uCol    = y1;
  if (delta_x > 0)
    incx = 1;  // 设置单步方向
  else if (delta_x == 0)
    incx = 0;  // 垂直线
  else {
    incx    = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;  // 水平线
  else {
    incy    = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
    distance = delta_x;  // 选取基本增量坐标轴
  else
    distance = delta_y;
  for (t = 0; t <= distance + 1; t++)  // 画线输出
  {
    lcd_draw_point(uRow, uCol);  // 画点
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      uCol += incy;
    }
  }
}

void lcd_draw_cross(uint16_t x1, uint16_t y1, uint16_t length) {
  int16_t uRow, uCol;
  uRow = x1 + length;
  lcd_draw_line(x1, y1, uRow, y1);
  uRow = ((x1 - length) < 0) ? 0 : x1 - length;
  lcd_draw_line(x1, y1, uRow, y1);
  uCol = y1 + length;
  lcd_draw_line(x1, y1, x1, uCol);
  uCol = ((y1 - length) < 0) ? 0 : y1 - length;
  lcd_draw_line(x1, y1, x1, uCol);
}

// 画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height) {
  uint16_t diagonal_point_x1 = x1 + width / 2;
  uint16_t diagonal_point_y1 = y1 + height / 2;
  uint16_t diagonal_point_x2 = x1 - width / 2;
  uint16_t diagonal_point_y2 = y1 - height / 2;
  lcd_draw_line(diagonal_point_x1, diagonal_point_y1, diagonal_point_x1, diagonal_point_y2);
  lcd_draw_line(diagonal_point_x1, diagonal_point_y1, diagonal_point_x2, diagonal_point_y1);
  lcd_draw_line(diagonal_point_x2, diagonal_point_y2, diagonal_point_x1, diagonal_point_y2);
  lcd_draw_line(diagonal_point_x2, diagonal_point_y2, diagonal_point_x2, diagonal_point_y1);
}

void lcd_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* p) {
  uint8_t picH, picL;
  lcd_set_windows(x, y, x + width, y + width);
  for (int i = 0; i < width * height; i++) {
    picL = *(p + i * 2);
    picH = *(p + i * 2 + 1);
    lcd_write_data_16(picH << 8 | picL);
  }
}
