#pragma once
#include <stdint.h>
void gui_draw_point(uint16_t x, uint16_t y, uint16_t color);

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void gui_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* p);