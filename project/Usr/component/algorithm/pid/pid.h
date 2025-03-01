#pragma once

#include <stdint.h>

typedef enum {
  k_pid_servo_x = 0,
  k_pid_servo_y,
} identify;

typedef struct {
  float P;
  float I;
  float D;

  float cumulative_error;
  float now_error;
  float previous_error;
  float last_error;

  float out;

} pid_data_block_t;

int32_t pid_position(int32_t error, identify id);
int32_t pid_increment(int32_t error, identify id);