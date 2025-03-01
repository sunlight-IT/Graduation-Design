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

void duty_output(uint32_t duty, int option) {
  uint32_t duty_temp = duty;
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
  float    angle_tmp = 0.1 * angle;
  uint32_t duty      = 0;
  if (angle_tmp >= ANGLE_MAX)
    angle_tmp = ANGLE_MAX;
  else if (angle_tmp <= ANGLE_MIN)
    angle_tmp = ANGLE_MIN;

  duty = angle_tmp / ANGLE_MAX * (SERVO_DUTY_MAX - SERVO_DUTY_MIN) + SERVO_DUTY_MIN;
  duty_output(duty, option);
}

// block type
void servo_test(void) {
  if (flages) {
    for (int i = 10; i < 170; i++) {
      servo_angle_row(i, 0);
      servo_angle_row(i, 1);
      HAL_Delay(10);
    }
    flages = 0;
  } else {
    for (int i = 170; i > 10; i--) {
      servo_angle_row(i, 0);
      servo_angle_row(i, 1);
      HAL_Delay(10);
    }
    flages = 1;
  }
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
