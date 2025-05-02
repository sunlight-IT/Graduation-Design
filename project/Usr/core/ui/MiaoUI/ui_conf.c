/*
 * This file is part of the MiaoUI Library.
 *
 * Copyright (c) 2025, JFeng-Z, <2834294740@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Created on: 2025-02-08
 */
#include "ui_conf.h"

#include "core_ui/ui.h"
#include "display/dispDriver.h"
#include "images/image.h"
#include "version.h"
#include "widget/custom.h"

/*******************MY_self********************/
#include "Core/app/app_schedule.h"
/***************************************/

/* Page */

ui_page_t Home_Page, System_Page;

/*item */
ui_item_t HomeHead_Item, SystemHead_Item, System_Item, Github_Item;
ui_item_t Contrast_Item;

/*******************MY_self********************/


typedef struct {
  const char *name;
  uint32_t  color_id;
}color_element_info_t;

color_element_info_t color_element_info[3] = {
  {"Red", COLOR_RED},
  {"Yellow", COLOR_YELLOW},
  {"Blue", COLOR_BLUE},
};

ui_item_t Color_Item[3];
ui_item_t Open_Item;
/***************************************/

/**
 * 在此建立所需显示或更改的数据
 * 无参数
 * 无返回值
 */
void Create_Parameter(ui_t *ui) {
  static int       Contrast = 255;
  static ui_data_t Contrast_data;
  Contrast_data.name         = "Contrast";
  Contrast_data.ptr          = &Contrast;
  Contrast_data.function     = Disp_SetContrast;
  Contrast_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
  Contrast_data.dataType     = UI_DATA_INT;
  Contrast_data.actionType   = UI_DATA_ACTION_RW;
  Contrast_data.max          = 255;
  Contrast_data.min          = 0;
  Contrast_data.step         = 5;
  static ui_element_t Contrast_element;
  Contrast_element.data = &Contrast_data;
  Create_element(&Contrast_Item, &Contrast_element);

  /*******************MY_self********************/
 

  static int       ColorRed = 0;
  static ui_data_t ColorRed_data;
  ColorRed_data.name       = color_element_info[COLOR_RED].name;
  ColorRed_data.ptr        = &ColorRed;
  ColorRed_data.dataType   = UI_DATA_SWITCH;
  ColorRed_data.actionType = UI_DATA_ACTION_RW;
  static ui_element_t Color_element;
  Color_element.data = &ColorRed_data;
  Create_element(&Color_Item[COLOR_RED], &Color_element);
  Color_Item[COLOR_RED].id = color_element_info[COLOR_RED].color_id;

  static int       ColorYellow = 0;
  static ui_data_t ColorYellow_data;
  ColorYellow_data.name       = color_element_info[COLOR_YELLOW].name;
  ColorYellow_data.ptr        = &ColorYellow;
  ColorYellow_data.dataType   = UI_DATA_SWITCH;
  ColorYellow_data.actionType = UI_DATA_ACTION_RW;
  static ui_element_t ColorYellow_element;
  ColorYellow_element.data = &ColorYellow_data;
  Create_element(&Color_Item[COLOR_YELLOW], &ColorYellow_element);
  Color_Item[COLOR_YELLOW].id = color_element_info[COLOR_YELLOW].color_id;

  static int       ColorBlue = 0;
  static ui_data_t ColorBlue_data;
  ColorBlue_data.name       = color_element_info[COLOR_BLUE].name;
  ColorBlue_data.ptr        = &ColorBlue;
  ColorBlue_data.dataType   = UI_DATA_SWITCH;
  ColorBlue_data.actionType = UI_DATA_ACTION_RW;
  static ui_element_t ColorBlue_element;
  ColorBlue_element.data = &ColorBlue_data;
  Create_element(&Color_Item[COLOR_BLUE], &ColorBlue_element);
  Color_Item[COLOR_BLUE].id = color_element_info[COLOR_BLUE].color_id;

  // static int       OpenStart = 0;
  // static ui_data_t OpenStart_data;
  // OpenStart_data.name         = "OpenStart";
  // OpenStart_data.function     = trace_picture;
  // OpenStart_data.functionType = UI_ITEM_LOOP_FUNCTION;
  // OpenStart_data.ptr          = &OpenStart;
  // OpenStart_data.dataType     = UI_DATA_SWITCH;
  // OpenStart_data.actionType   = UI_DATA_ACTION_RW;
  // static ui_element_t OpenStart_element;
  // OpenStart_element.data = &OpenStart_data;
  // Create_element(&Open_Item, &OpenStart_element);
}

/**
 * 在此建立所需显示或更改的文本
 * 无参数
 * 无返回值
 */
void Create_Text(ui_t *ui) {
  static ui_text_t github_text;
  github_text.font      = UI_FONT;
  github_text.fontHight = UI_FONT_HIGHT;
  github_text.fontWidth = UI_FONT_WIDTH;
  github_text.ptr       = VERSION_PROJECT_LINK;
  static ui_element_t github_element;
  github_element.text = &github_text;
  Create_element(&Github_Item, &github_element);
}

/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
void Create_MenuTree(ui_t *ui) {
  // AddPage("[HomePage]", &Home_Page, UI_PAGE_ICON);
  // AddItem("[HomePage]", UI_ITEM_ONCE_FUNCTION, logo_allArray[0], &HomeHead_Item, &Home_Page, NULL, Draw_Home);
  // AddItem(" +System", UI_ITEM_PARENTS, logo_allArray[1], &System_Item, &Home_Page, &System_Page, NULL);
  // AddPage("[System]", &System_Page, UI_PAGE_TEXT);
  // AddItem("[System]", UI_ITEM_RETURN, NULL, &SystemHead_Item, &System_Page, &Home_Page, NULL);
  // AddItem(" -Contrast", UI_ITEM_DATA, NULL, &Contrast_Item, &System_Page, NULL, NULL);
  // AddItem(" -Red", UI_ITEM_DATA, NULL, &Color_Item[0], &System_Page, NULL, NULL);
  // AddItem(" -Yellow", UI_ITEM_DATA, NULL, &Color_Item[1], &System_Page, NULL, NULL);
  // AddItem(" -Blue", UI_ITEM_DATA, NULL, &Color_Item[2], &System_Page, NULL, NULL);
  // AddItem(" -Github", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Home_Page, NULL, NULL);
  // AddPage("[HomePage]", &Home_Page, UI_PAGE_TEXT);
  // AddItem(" -Contrast", UI_ITEM_DATA, NULL, &Contrast_Item, &System_Page, NULL, NULL);

  AddPage("[HomePage]", &Home_Page, UI_PAGE_TEXT);
  AddItem("[START]", UI_ITEM_LOOP_FUNCTION, &Open_Item, &Open_Item, &Home_Page, NULL, app_schedule);
  AddItem(" -Red", UI_ITEM_DATA, NULL, &Color_Item[0], &Home_Page, NULL, app_clore_choose);
  AddItem(" -Yellow", UI_ITEM_DATA, NULL, &Color_Item[1], &Home_Page, NULL, app_clore_choose);
  AddItem(" -Blue", UI_ITEM_DATA, NULL, &Color_Item[2], &Home_Page, NULL, app_clore_choose);
}

void MiaoUi_Setup(ui_t *ui) {
  Create_UI(ui, &Open_Item);  // 创建UI, 必须给定一个头项目
  // Draw_Home(ui);
}
