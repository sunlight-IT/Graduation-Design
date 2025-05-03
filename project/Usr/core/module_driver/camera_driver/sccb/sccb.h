#pragma once

#include "gpio.h"
#include "stdbool.h"

#define SDIO_CLK           SDIO_C_Pin
#define SDIO_DAT           SDIO_D_Pin
#define SDIO_CLK_GPIO_Port SDIO_D_GPIO_Port

#define SDIO_C(x) HAL_GPIO_WritePin(SDIO_CLK_GPIO_Port, SDIO_CLK, (x))
#define SDIO_D(x) HAL_GPIO_WritePin(SDIO_CLK_GPIO_Port, SDIO_DAT, (x))

#define SDIO_D_IN()                        \
  {                                        \
    SDIO_CLK_GPIO_Port->CRL &= 0XFF0FFFFF; \
    SDIO_CLK_GPIO_Port->CRL |= 0X00800000; \
  }
#define SDIO_D_OUT()                       \
  {                                        \
    SDIO_CLK_GPIO_Port->CRL &= 0XFF0FFFFF; \
    SDIO_CLK_GPIO_Port->CRL |= 0X00300000; \
  }

#define SDIO_D_READ HAL_GPIO_ReadPin(SDIO_CLK_GPIO_Port, SDIO_DAT)

#define ADDR_OV7725 0x42

bool sccb_write_byte(uint8_t write_addr, uint8_t byte);
bool sccb_read_byte(uint8_t read_addr, uint8_t *read_buf, uint16_t len);