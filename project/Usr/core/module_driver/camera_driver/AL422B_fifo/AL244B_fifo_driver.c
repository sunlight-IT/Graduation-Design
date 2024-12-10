#include "AL244B_fifo_driver.h"

#include "component/component.h"

#define TAG "ALL2448_FIFO"

static uint8_t n_vsync;

void ov7725_fifo_init(void) {
  FIFO_OE_L;
  FIFO_WE_H;
}

void    clear_vsync(void) { n_vsync = 0; }
uint8_t get_vsync(void) { return n_vsync; }

// exti callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (n_vsync == 0) {
    FIFO_WRST_L;
    FIFO_WE_H;
    n_vsync = 1;
    FIFO_WRST_H;
  } else if (n_vsync == 1) {
    FIFO_WE_L;
    n_vsync = 2;
  }
}
