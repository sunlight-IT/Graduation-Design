#include "oled_driver.h"

#include "u8g2.h"

#define SCL(x)                               \
  HAL_GPIO_WritePin(IIC_PORT, IIC_SCL, (x)); \
  HAL_Delay_Us(10)
#define SDA(x)                               \
  HAL_GPIO_WritePin(IIC_PORT, IIC_SDA, (x)); \
  HAL_Delay_Us(10)

#define SDA_RD() GPIO_ReadInputDataBit(IIC_PORT, IIC_SDA)

void HAL_Delay_Us(uint16_t nus) {
  uint32_t count = nus * (SystemCoreClock / 72);
  while (count--);
}

void HAL_Delay_Ms(uint32_t xms) {
  while (xms--) {
    HAL_Delay_Us(1000);
  }
}

static void my_iic_start(void) {
  SDA(1);
  SCL(1);
  SDA(0);
  SCL(0);
}

static void my_iic_stop(void) {
  SDA(0);
  SCL(1);
  SDA(1);
}

static void my_iic_send_byte(uint8_t byte) {
  uint8_t i;
  uint8_t temp = 0;
  for (i = 0; i < 8; i++) {
    temp = byte & (0x80 >> i);

    if (temp) {
      SDA(1);
    } else {
      SDA(0);
    }
    SCL(1);
    SDA(0);
  }
}

uint8_t my_iic_recv_byte(void) {
  uint8_t i, byte = 0x00;
  SDA(1);
  for (i = 0; i < 8; i++) {
    SCL(1);
    if (SDA_RD() == 1) {
      byte |= (0x80 >> i);
    }
    HAL_Delay_Us(10);
    SCL(0);
  }
  return byte;
}

static void my_iic_send_ack(uint8_t ack) {
  if (ack) {
    SDA(1);
  } else {
    SDA(0);
  }
  SCL(1);
  SCL(0);
}

static uint8_t my_iic_recv_ack(void) {
  uint8_t ack;
  SDA(1);
  SCL(1);
  ack = SDA_RD();
  SCL(0);
  return ack;
}

static uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch (msg) {
    case U8X8_MSG_DELAY_100NANO:  // delay arg_int * 100 nano seconds
      __NOP();
      break;
    case U8X8_MSG_DELAY_10MICRO:  // delay arg_int * 10 micro seconds
      for (uint16_t n = 0; n < 320; n++) {
        __NOP();
      }
      break;
    case U8X8_MSG_DELAY_MILLI:  // delay arg_int * 1 milli second
      //__NOP();
      HAL_Delay(1);  // HAL_Delay_Ms(1);
      break;
    case U8X8_MSG_DELAY_I2C:  // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      // HAL_Delay_Us(5);
      //__NOP();
      break;                       // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK:  // arg_int=0: Output low at I2C clock pin
      if (arg_int == 1) {
        HAL_GPIO_WritePin(IIC_PORT, IIC_SCL, SET);
        // 0x40000000UL + 0x00010000UL + 0x00001800UL
      } else if (arg_int == 0) {
        HAL_GPIO_WritePin(IIC_PORT, IIC_SCL, RESET);
      }
      break;                      // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
      if (arg_int == 1) {
        HAL_GPIO_WritePin(IIC_PORT, IIC_SDA, SET);
      } else if (arg_int == 0) {
        HAL_GPIO_WritePin(IIC_PORT, IIC_SDA, RESET);
      }
      break;  // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);  // default return value
      break;
  }
  return 1;
}

extern u8g2_t u8g2;

void display_init(void) {
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearBuffer(&u8g2);

  // while (1) {
  // u8g2_SendBuffer(&u8g2);
  // u8g2_DrawBox(&u8g2, 0, 0, 20, 20);
  // u8g2_DrawBox(&u8g2, 20, 20, 20, 20);
  // u8g2_SendBuffer(&u8g2);
  // u8g2_DrawFrame(&u8g2, 10, 40, 20, 20);
  // u8g2_SendBuffer(&u8g2);
  // u8g2_SetFont(&u8g2, u8g2_font_DigitalDiscoThin_tf);
  // u8g2_DrawStr(&u8g2, 30, 10, "free");
  // u8g2_SendBuffer(&u8g2);

  // HAL_Delay(1000);
  //}
}