#ifndef _SERVO_APPLY_H_
#define _SERVO_APPLY_H_

#define ACTUAL_IMAGE_LEN      (50)                                           //
#define ACTUAL_IMAGE_WIDTH    (50)                                           //
#define CAMERA_IMAGE_DISTANCE (50)                                           // 摄像头到实际图像的垂直距离
#define TFT_AND_ACTUAL_RATE   (ACTUAL_IMAGE_LEN / (float)(120 - 1))          // TFT两像素和实际图像大小的比例
#define CAL_PARAMER           (TFT_AND_ACTUAL_RATE / CAMERA_IMAGE_DISTANCE)  // 计算实际tan的参数

#define MID_ANGLR 900

void rate_translate_angle(void);
void servo_pid_calculate(void);

#endif