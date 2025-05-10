#include "driver_button.h"

#include "gpio.h"

#define BUTTON_PORT GPIOG

#define BUTTON_TOP_PIN  KEY0_Pin
#define BUTTON_MID_PIN  KEY1_Pin
#define BUTTON_DOWN_PIN KEY2_Pin

uint8_t read_button(button_id id) {
  switch (id) {
    case BUTTON_TOP_ID:
      return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_TOP_PIN);
      break;
    case BUTTON_MID_ID:
      return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_MID_PIN);
      break;
    case BUTTON_DOWN_ID:
      return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_DOWN_PIN);
      break;
    default:
      break;
  }
}