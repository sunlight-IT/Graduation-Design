#pragma once

int __io_putchar(int ch);

void z_log(const char* fmt, ...);

#define ZLOGI(TAG, fmt, ...) z_log("%08lu INFO : [%-8s] " fmt "\n", HAL_GetTick(), TAG, ##__VA_ARGS__)
#define ZLOGE(TAG, fmt, ...) z_log("%08lu ERROR: [%-8s] " fmt "\n", HAL_GetTick(), TAG, ##__VA_ARGS__)
#define ZLOGW(TAG, fmt, ...) z_log("%08lu WARNING: [%-8s] " fmt "\n", HAL_GetTick(), TAG, ##__VA_ARGS__)