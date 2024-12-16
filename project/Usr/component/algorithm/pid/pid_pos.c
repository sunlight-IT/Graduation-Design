#include "pid_pos.h"

#include "stddef.h"
/*
1. 输入：error  输出 ：out
2. p, i, d
3. 积分 微分
 */

// 应用static如果有多个功能需要调用 pid_position函数，
// 那么上一个调用函数的静态数据就可能被修改，
// 或者之前的数据被用到新功能中
// 解决：添加功能标识符,适用于运行时开辟新空间，不适合死内存
static pid_data_block_t position_x = {
    .P = 0,
    .I = 0,
    .D = 0,
};
static pid_data_block_t position_y = {
    .P = 0,
    .I = 0,
    .D = 0,
};

int32_t pid_position(int32_t error, identify id) {
  int32_t           out = 0;
  pid_data_block_t* pos = NULL;
  if (k_pid_servo_x == id)
    pos = &position_x;
  else if (k_pid_servo_y == id)
    pos = &position_y;

  pos->now_error = error;

  out += pos->P * pos->now_error +                     //
         pos->I * pos->cumulative_error +              //
         pos->D * (pos->now_error - pos->last_error);  //

  pos->last_error = error;
  pos->cumulative_error += error;

  return out;
}