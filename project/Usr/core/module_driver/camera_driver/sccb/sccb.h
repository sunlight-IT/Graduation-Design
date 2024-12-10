#pragma once

#include "gpio.h"
#include "stdbool.h"

#define GPIO_TYPE GPIOC
#define SDIO_CLK  GPIO_PIN_6
#define SDIO_DAT  GPIO_PIN_7

#define SDIO_C(x) HAL_GPIO_WritePin(GPIO_TYPE, SDIO_CLK, (x))
#define SDIO_D(x) HAL_GPIO_WritePin(GPIO_TYPE, SDIO_DAT, (x))

#define SDIO_D_IN()           \
  {                           \
    GPIOC->CRL &= 0X0FFFFFFF; \
    GPIOC->CRL |= 0X80000000; \
  }
#define SDIO_D_OUT()          \
  {                           \
    GPIOC->CRL &= 0X0FFFFFFF; \
    GPIOC->CRL |= 0X30000000; \
  }

#define SDIO_D_READ HAL_GPIO_ReadPin(GPIO_TYPE, SDIO_DAT)

#define ADDR_OV7725 0x42

bool sccb_write_byte(uint8_t write_addr, uint8_t byte);
bool sccb_read_byte(uint8_t read_addr, uint8_t *read_buf, uint16_t len);