#ifndef _SERVO_APPLY_H_
#define _SERVO_APPLY_H_

#define ACTUAL_IMAGE_LEN      (50)                                           //
#define ACTUAL_IMAGE_WIDTH    (50)                                           //
#define CAMERA_IMAGE_DISTANCE (50)                                           // ����ͷ��ʵ��ͼ��Ĵ�ֱ����
#define TFT_AND_ACTUAL_RATE   (ACTUAL_IMAGE_LEN / (float)(120 - 1))          // TFT�����غ�ʵ��ͼ���С�ı���
#define CAL_PARAMER           (TFT_AND_ACTUAL_RATE / CAMERA_IMAGE_DISTANCE)  // ����ʵ��tan�Ĳ���

#define MID_ANGLR 900

void rate_translate_angle(void);
void servo_pid_calculate(void);

#endif