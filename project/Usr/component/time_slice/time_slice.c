#include "time_slice.h"

TIME_DATA_T image_data;
TIME_DATA_T image_show;
TIME_DATA_T servo_ctrl;

void apply_slice(void) {
  image_data.timeimg_num >= 1 ? (image_data.state = true, image_data.timeimg_num = 0) : image_data.timeimg_num++;
  image_show.timeimg_num >= 10 ? (image_show.state = true, image_show.timeimg_num = 0) : image_show.timeimg_num++;
  servo_ctrl.timeimg_num >= 5 ? (servo_ctrl.state = true, servo_ctrl.timeimg_num = 0) : servo_ctrl.timeimg_num++;
}