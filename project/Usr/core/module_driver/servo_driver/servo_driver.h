#pragma once

#include <stdint.h>

#include "stm32f1xx_hal.h"

#define SERVO_DUTY_MIN 25   // 0.5ms
#define SERVO_DUTY_MAX 100  // 2ms

#define ANGLE_MAX 180
#define ANGLE_MIN 0
#define DUTY_MAX  1000
#define DUTY_MIN  10

typedef enum {
  k_servo_x = 0,
  k_servo_y,
} ServoOptionEnum;

void servo_x_handle_reg(TIM_HandleTypeDef *h_tim);
void servo_y_handle_reg(TIM_HandleTypeDef *h_tim);

void servo_angle_row(uint16_t angle, int option);