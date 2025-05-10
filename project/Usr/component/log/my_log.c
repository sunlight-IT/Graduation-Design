#include "my_log.h"

#include <stdarg.h>

#include "usart.h"

// #ifdef __GNUC__
// #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
// #else
// #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
// #endif

// PUTCHAR_PROTOTYPE
// {
//   HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0x200);
//   return ch;
// }

int _write(int file, char *ptr, int len) {
  for (int i = 0; i < len; i++) {
    uint8_t ch = *ptr++;
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  }
}
void my_log(const char *format, ...) {
  // 初始化日志长度变量，用于存储输出的字符数
  va_list args;            // 可变形参
  va_start(args, format);  // 指向format后面应该地址，第一个可变形参地址
  vprintf(format, args);
  va_end(args);
}
