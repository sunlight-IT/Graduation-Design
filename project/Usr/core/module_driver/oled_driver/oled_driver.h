#pragma once

#include "gpio.h"

#define IIC_PORT GPIOF
#define IIC_SCL  GPIO_PIN_14
#define IIC_SDA  GPIO_PIN_15

void HAL_Delay_Us(uint16_t nus);

void display_init(void);