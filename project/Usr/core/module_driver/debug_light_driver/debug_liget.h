#pragma once

#include "gpio.h"

#define DEBUG_LIGHT_MODE   GPIOB
#define DEBUG_LIGHT_PIN    GPIO_PIN_5
#define DEBUG_LIGHT_TOGGLE HAL_GPIO_TogglePin(DEBUG_LIGHT_MODE, DEBUG_LIGHT_PIN)

#define DEBUG_LIGHT_BLINK 1000

void debug_light(void);