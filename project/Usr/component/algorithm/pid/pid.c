#include "pid.h"

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
    .P                = 1,
    .I                = 0.4,
    .D                = 0.3,
    .now_error        = 0,
    .previous_error   = 0,
    .last_error       = 0,
    .cumulative_error = 0,
    .out              = 0,
};
static pid_data_block_t position_y = {
    .P                = 1,
    .I                = 0.4,
    .D                = 0.3,
    .now_error        = 0,
    .last_error       = 0,
    .previous_error   = 0,
    .cumulative_error = 0,
    .out              = 0,
};

int32_t pid_position(int32_t error, identify id) {
  int32_t           out = 0;
  pid_data_block_t* pos = NULL;
  if (k_pid_servo_x == id)
    pos = &position_x;
  else if (k_pid_servo_y == id)
    pos = &position_y;

  pos->now_error = error;

  pos->out = pos->P * pos->now_error +                     //
             pos->I * pos->cumulative_error +              //
             pos->D * (pos->now_error - pos->last_error);  //

  pos->last_error = error;
  pos->cumulative_error += error;

  return out;
}

int32_t pid_increment(int32_t error, identify id) {
  pid_data_block_t* incre = NULL;
  if (k_pid_servo_x == id)
    incre = &position_x;
  else if (k_pid_servo_y == id)
    incre = &position_y;

  incre->now_error = 1.0 * error;

  incre->out += incre->P * (incre->now_error - incre->previous_error) +                             //
                incre->I * incre->now_error +                                                       //
                incre->D * (incre->now_error - 2 * incre->previous_error + incre->previous_error);  //

  incre->last_error     = incre->previous_error;
  incre->previous_error = incre->now_error;
  return (int32_t)incre->out;
}