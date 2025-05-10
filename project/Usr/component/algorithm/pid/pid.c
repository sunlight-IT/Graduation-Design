#include "pid.h"

#include "stddef.h"
/*
1. ���룺error  ��� ��out
2. p, i, d
3. ���� ΢��
 */

// Ӧ��static����ж��������Ҫ���� pid_position������
// ��ô��һ�����ú����ľ�̬���ݾͿ��ܱ��޸ģ�
// ����֮ǰ�����ݱ��õ��¹�����
// ��������ӹ��ܱ�ʶ��,����������ʱ�����¿ռ䣬���ʺ����ڴ�
static pid_data_block_t position_x = {
    // .P                = 1,
    // .I                = 0.4,
    // .D                = 0.3,
    .P = 0.1, .I = 0.3, .D = 0.05, .now_error = 0, .previous_error = 0, .last_error = 0, .cumulative_error = 0, .out = 0,
};
static pid_data_block_t position_y = {
    // .P                = 1,
    // .I                = 0.4,
    // .D                = 0.3,
    .P = 0.1, .I = 0.3, .D = 0.05, .now_error = 0, .last_error = 0, .previous_error = 0, .cumulative_error = 0, .out = 0,
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