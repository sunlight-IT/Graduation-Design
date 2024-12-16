#pragma once

#include <stdint.h>

typedef enum {
  k_pid_servo_x = 1,
  k_pid_servo_y,
} identify;

typedef struct {
  uint16_t P;
  uint16_t I;
  uint16_t D;

  int32_t cumulative_error;
  int32_t last_error;
  int32_t now_error;

} pid_data_block_t;