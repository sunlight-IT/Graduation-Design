#include "error_handing.h"

#include "component/component.h"
#include "stm32f1xx_hal.h"

void ErrorHanding(char* local, char* info) {
  // while (1) {
  ZLOGE(local, "%s", info);
  // HAL_Delay(1000);
  //}
}