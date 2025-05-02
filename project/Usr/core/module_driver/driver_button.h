#pragma once

#include <stdint.h>

typedef enum {
  BUTTON_TOP_ID,  //
  BUTTON_MID_ID,  //
  BUTTON_DOWN_ID
} button_id;

uint8_t read_button(button_id id);