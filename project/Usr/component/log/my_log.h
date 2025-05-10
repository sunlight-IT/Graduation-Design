#pragma once
#include <stdio.h>

#include "stm32f1xx.h"

void my_log(const char *format, ...);

#define LOGI(fmt, ...) my_log("[%08lu] [INFO] <%-20s> : " fmt " \r\n", HAL_GetTick(), __func__, ##__VA_ARGS__);
#define LOGW(fmt, ...) my_log("[%08lu] [WARN] <%-20s> : " fmt " \r\n", HAL_GetTick(), __func__, ##__VA_ARGS__);
#define LOGE(fmt, ...) my_log("[%08lu] [EROR] <%-20s> : " fmt " \r\n", HAL_GetTick(), __func__, ##__VA_ARGS__);

#define vofa_LOGI(TYPE, fmt, ...) my_log(" %s:" fmt "\n", TYPE, ##__VA_ARGS__)
