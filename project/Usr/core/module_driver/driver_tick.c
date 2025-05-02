#include "driver_tick.h"

#include <stdint.h>

#include "component/MultiButton/multi_button.h"
#include "component/component.h"
#include "stm32f1xx.h"
#include "tim.h"
// volatile unsigned long g_SystemTicks;
volatile uint8_t tims1;
volatile uint8_t tims2;
// 定时器2中断服务函数

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  tims1++;
  if (tims1 > 5) {
    tims1 = 0;
    // LOGI("button time");
    button_ticks();
  }
}

// void TIM2_IRQHandler(void) {
//   if (TIM_GetITStatus(TIM2, TIM_IT_UPDATE) != RESET) {
//     // 清除中断标志位
//     TIM_ClearITPendingBit(TIM2, TIM_IT_UPDATE);

//     // tims2++;
//     //  if (tims2 > 10) {
//     //    tims2 = 0;
//     //    buzzer_proc(&Buzzer);
//     //  }
//     //  g_SystemTicks ++;
//   }
// }