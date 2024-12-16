#include "pid_pos.h"

#include "stddef.h"
/*
1. ���룺error  ��� ��out
2. p, i, d
3. ���� ΢��
 */

// Ӧ��static����ж��������Ҫ���� pid_position������
// ��ô��һ�����ú����ľ�̬���ݾͿ��ܱ��޸ģ�
// ����֮ǰ�����ݱ��õ��¹�����
// �������ӹ��ܱ�ʶ��,����������ʱ�����¿ռ䣬���ʺ����ڴ�
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