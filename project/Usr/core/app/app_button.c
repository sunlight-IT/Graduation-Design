#include "app_button.h"

#include "Core/module_driver/driver_button.h"
#include "component/MultiButton/multi_button.h"
#include "component/component.h"

static Button    button[3];
static UI_ACTION Action;

void Callback_Button_Top_Handler(void) {  //
  Action = UI_ACTION_UP;
  LOGI("test top button");
}

void Callback_Button_MID_Handler(void) {  //
  Action = UI_ACTION_ENTER;
  LOGI("test mid button");
}

void Callback_Button_DOWN_Handler(void) {  //
  Action = UI_ACTION_DOWN;
  LOGI("test down button");
}

void APP_Button_SetUp(void) {
  button_init(&button[0], read_button, 0, BUTTON_TOP_ID);
  button_init(&button[1], read_button, 0, BUTTON_MID_ID);
  button_init(&button[2], read_button, 0, BUTTON_DOWN_ID);

  button_attach(&button[0], SINGLE_CLICK, Callback_Button_Top_Handler);
  button_attach(&button[0], DOUBLE_CLICK, Callback_Button_Top_Handler);

  button_attach(&button[1], SINGLE_CLICK, Callback_Button_MID_Handler);
  button_attach(&button[1], DOUBLE_CLICK, Callback_Button_MID_Handler);

  button_attach(&button[2], SINGLE_CLICK, Callback_Button_DOWN_Handler);
  button_attach(&button[2], DOUBLE_CLICK, Callback_Button_DOWN_Handler);

  button_start(&button[0]);
  button_start(&button[1]);
  button_start(&button[2]);
}

UI_ACTION Button_Scan(void) {
  switch (Action) {
    case UI_ACTION_UP:  // 向上
      Action = UI_ACTION_NONE;
      return UI_ACTION_UP;
      break;
    case UI_ACTION_DOWN:  // 向下
      Action = UI_ACTION_NONE;
      return UI_ACTION_DOWN;
      break;
    case UI_ACTION_ENTER:  // 进入或选择
      Action = UI_ACTION_NONE;
      return UI_ACTION_ENTER;
      break;
  }

  return UI_ACTION_NONE;
}
