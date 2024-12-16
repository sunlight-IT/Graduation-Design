#include "servo_driver.h"

#include "component/component.h"

#define TAG "servo_driver"

static TIM_HandleTypeDef *m_tim;

void servo_handle_reg(TIM_HandleTypeDef *h_tim) { m_tim = h_tim; }

void duty_output(uint32_t duty) {
  uint32_t duty_temp = duty;
  if (duty_temp <= DUTY_MIN)
    duty_temp = DUTY_MIN;
  else if (duty_temp >= DUTY_MAX)
    duty_temp = DUTY_MAX;
  __HAL_TIM_SetCompare(m_tim, TIM_CHANNEL_1, duty_temp);
}

void servo_angle_row(uint16_t angle) {
  float    angle_tmp = 1.0f * angle;
  uint32_t duty      = 0;
  if (angle_tmp > ANGLE_MAX)
    angle_tmp = ANGLE_MAX;
  else if (angle_tmp < ANGLE_MIN)
    angle_tmp = ANGLE_MIN;

  duty = angle_tmp / ANGLE_MAX * (SERVO_DUTY_MAX - SERVO_DUTY_MIN) + SERVO_DUTY_MIN;
  duty_output(duty);
}