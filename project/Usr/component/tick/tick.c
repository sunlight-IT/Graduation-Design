#include "tick.h"

#include "stm32f1xx_hal.h"

uint32_t has_pass_time(uint32_t last_time)
{
    int32_t pass_time;
    pass_time = HAL_GetTick() - last_time;
    if (pass_time >= 0)
        return pass_time;
    else
        return UINT32_MAX - last_time - pass_time;
}