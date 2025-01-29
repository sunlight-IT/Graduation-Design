#include "servo_driver.h"

#include "component/component.h"

#define TAG "servo_driver"

static TIM_HandleTypeDef *m_tim[2];

void servo_x_handle_reg(TIM_HandleTypeDef *h_tim) { m_tim[0] = h_tim; }
void servo_y_handle_reg(TIM_HandleTypeDef *h_tim) { m_tim[1] = h_tim; }

void duty_output(uint32_t duty, int option) {
  uint32_t duty_temp = duty;
  if (duty_temp <= DUTY_MIN)
    duty_temp = DUTY_MIN;
  else if (duty_temp >= DUTY_MAX)
    duty_temp = DUTY_MAX;
  if (option == k_servo_x)
    __HAL_TIM_SetCompare(m_tim[option], TIM_CHANNEL_1, duty_temp);
  else
    __HAL_TIM_SetCompare(m_tim[1], TIM_CHANNEL_1, duty_temp);
}

void servo_angle_row(uint16_t angle, int option) {
  float    angle_tmp = 1.0f * angle;
  uint32_t duty      = 0;
  if (angle_tmp > ANGLE_MAX)
    angle_tmp = ANGLE_MAX;
  else if (angle_tmp < ANGLE_MIN)
    angle_tmp = ANGLE_MIN;

  duty = angle_tmp / ANGLE_MAX * (SERVO_DUTY_MAX - SERVO_DUTY_MIN) + SERVO_DUTY_MIN;
  duty_output(duty, option);
}