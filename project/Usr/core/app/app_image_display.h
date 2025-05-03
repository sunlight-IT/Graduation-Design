#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_BLUE,
  } color_t;

void app_image_display();

bool app_trace_picture(void);
void now_target_set(uint32_t color_type);