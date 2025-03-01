#ifndef _FSM_MIDDLE_H_
#define _FSM_MIDDLE_H_

#include "core/module_driver/camera_driver/ov7725/ov_7725.h"

typedef enum {
  fsm_err      = -1,
  fsm_cpl      = 0,
  fsm_on_going = 1,
} fsm_t;

fsm_t image_process(ov7725_mode_param_t* camera_mode);

#endif