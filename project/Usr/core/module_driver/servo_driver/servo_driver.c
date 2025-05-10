#include "servo_driver.h"

#include "component/algorithm/pid/pid.h"
#include "component/component.h"
#include "core/module_driver/lcd_driver/lcd_drive.h"
#include "core/module_driver/servo_driver/servo_driver.h"

#define TAG "servo_driver"

static TIM_HandleTypeDef *m_tim[2];

static int flages = 1;

void servo_x_handle_reg(TIM_HandleTypeDef *h_tim) {
  m_tim[0] = h_tim;
  HAL_TIM_PWM_Start(m_tim[0], TIM_CHANNEL_1);
}
void servo_y_handle_reg(TIM_HandleTypeDef *h_tim) {
  m_tim[1] = h_tim;
  HAL_TIM_PWM_Start(m_tim[1], TIM_CHANNEL_1);
}

void duty_output(float duty, int option) {
  float duty_temp = duty;
  if (duty_temp <= DUTY_MIN)
    duty_temp = DUTY_MIN;
  else if (duty_temp >= DUTY_MAX)
    duty_temp = DUTY_MAX;
  if (option == k_servo_x)
    __HAL_TIM_SetCompare(m_tim[option], TIM_CHANNEL_1, duty_temp);
  else
    __HAL_TIM_SetCompare(m_tim[option], TIM_CHANNEL_1, duty_temp);
}

void servo_angle_row(uint16_t angle, int option) {
  float angle_tmp = angle;
  float duty      = 0;

  switch (option) {
    case k_servo_x:
      if (angle_tmp >= ANGLE_X_MAX)
        angle_tmp = ANGLE_X_MAX;
      else if (angle_tmp <= ANGLE_X_MIN)
        angle_tmp = ANGLE_X_MIN;

      duty = angle_tmp / ANGLE_X_MAX * (SERVO_DUTY_MAX - SERVO_DUTY_MIN) + SERVO_DUTY_MIN;
      duty_output(duty, option);
      break;
    case k_servo_y:
      if (angle_tmp >= ANGLE_Y_MAX)
        angle_tmp = ANGLE_Y_MAX;
      else if (angle_tmp <= ANGLE_Y_MIN)
        angle_tmp = ANGLE_Y_MIN;

      duty = angle_tmp / ANGLE_Y_MAX * (SERVO_DUTY_MAX - SERVO_DUTY_MIN) + SERVO_DUTY_MIN;
      duty_output(duty, option);
      /* code */
      break;
  }
}

// block type
void servo_test(void) {
  if (flages) {
    for (int i = 1000; i < 1600; i++) {
      servo_angle_row(i, 0);
      // servo_angle_row(i, 1);
      HAL_Delay(1);
    }
    flages = 0;
  } else {
    for (int i = 1600; i > 1000; i--) {
      servo_angle_row(i, 0);
      // servo_angle_row(i, 1);
      HAL_Delay(1);
    }
    flages = 1;
  }

  // servo_angle_row(1000, 0);
  // servo_angle_row(1000, 1);
  // HAL_Delay(2000);
  // servo_angle_row(1600, 0);
  // servo_angle_row(1350, 1);
  // HAL_Delay(2000);
}

// unblock type
static uint8_t angle;

void servo_unblock_type(void) {
  if (flages) {
    servo_angle_row(angle, 0);
    servo_angle_row(angle, 1);
    angle++;
    if (angle >= 160) flages = 0;
  } else {
    servo_angle_row(angle, 0);
    servo_angle_row(angle, 1);
    angle--;
    if (angle <= 10) flages = 1;
  }
}
