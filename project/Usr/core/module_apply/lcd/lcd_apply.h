#pragma once
#include <stdint.h>

#include "core/module_driver/lcd_driver/lcd_drive.h"
void lcd_draw_point(uint16_t x, uint16_t y);
void lcd_draw_point_clor(uint16_t x, uint16_t y, uint16_t clor);

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_draw_cross(uint16_t x1, uint16_t y1, uint16_t length);

void lcd_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* p);

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height);