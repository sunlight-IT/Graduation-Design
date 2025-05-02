#include "driver_button.h"

#include "gpio.h"

#define BUTTON_PORT GPIOE
// #define BUTTON_MID_PORT  GPIOE
// #define BUTTON_DOWN_PORT GPIOE

#define BUTTON_TOP_PIN  GPIO_PIN_10
#define BUTTON_MID_PIN  GPIO_PIN_11
#define BUTTON_DOWN_PIN GPIO_PIN_12

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