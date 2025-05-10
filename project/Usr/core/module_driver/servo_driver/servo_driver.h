#pragma once

#include <stdint.h>

#include "stm32f1xx_hal.h"

#define SERVO_DUTY_MIN 25   // 0.5ms
#define SERVO_DUTY_MAX 100  // 2ms

#define ANGLE_X_MAX ((float)1600)
#define ANGLE_X_MIN ((float)1000)
#define ANGLE_Y_MAX ((float)1350)
#define ANGLE_Y_MIN ((float)1100)
#define DUTY_MAX    1000
#define DUTY_MIN    10

typedef enum {
  k_servo_x = 0,
  k_servo_y,
} ServoOptionEnum;

void servo_x_handle_reg(TIM_HandleTypeDef *h_tim);
void servo_y_handle_reg(TIM_HandleTypeDef *h_tim);

void servo_angle_row(uint16_t angle, int option);

void servo_test(void);
void servo_unblock_type(void);