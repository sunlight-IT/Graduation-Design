#include "gui.h"

#include "core/module_driver/lcd_driver/lcd_drive.h"

void gui_draw_point(uint16_t x, uint16_t y, uint16_t color) {
  lcd_set_cursor(x, y);  // 设置光标位置
  lcd_write_data_16(color);
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

void gui_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* p) {
  uint8_t picH, picL;
  lcd_set_windows(x, y, x + width, y + width);
  for (int i = 0; i < width * height; i++) {
    picL = *(p + i * 2);
    picH = *(p + i * 2 + 1);
    lcd_write_data_16(picH << 8 | picL);
  }
}