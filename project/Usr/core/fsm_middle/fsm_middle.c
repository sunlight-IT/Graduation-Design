#include "fsm_middle.h"

#include "core/module_driver/camera_driver/ov7725/ov_7725.h"
#include "core/module_middle/image_display/image_display.h"

#define GET_IMAGE_DATA_RESET_FSM() \
  do {                             \
    s_State = START;               \
  } while (0)

#define TRACE_PICTURE_RESET_FSM() \
  do {                            \
    s_State = START;              \
  } while (0)

fsm_t image_process(ov7725_mode_param_t* camera_mode) {
  static enum {
    START = 0,
    GET_IMAGE_DATA,
    TRACE_PICTURE,
  } s_State = {START};

  switch (s_State) {
    case START:
      s_State = GET_IMAGE_DATA;
    case GET_IMAGE_DATA:
      if (!image_get(camera_mode->lcd_sx,     //
                     camera_mode->lcd_sy,     //
                     camera_mode->cam_width,  //
                     camera_mode->cam_height)) {
        GET_IMAGE_DATA_RESET_FSM();
        return fsm_on_going;
      }

      s_State = TRACE_PICTURE;
    case TRACE_PICTURE:
      if (!trace_picture()) {
        s_State = GET_IMAGE_DATA;
      }
      TRACE_PICTURE_RESET_FSM();
      return fsm_cpl;
  }
}