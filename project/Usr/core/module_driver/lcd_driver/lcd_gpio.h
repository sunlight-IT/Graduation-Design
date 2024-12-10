#pragma once

#include "gpio.h"

#define LCD_MODE GPIOB
#define LCD_CS   GPIO_PIN_11
#define LCD_RS   GPIO_PIN_10
#define LCD_RST  GPIO_PIN_12
#define LCD_LED  GPIO_PIN_9

//-----------------LCD端口定义----------------
// #define GPIO_TYPE GPIOB  // GPIO组类型
// // #define LED       9      // 背光控制引脚        PB9
// #define LCD_CS  11  // 片选引脚            PB11
// #define LCD_RS  10  // 寄存器/数据选择引脚 PB10
// #define LCD_RST 12  // 复位引脚            PB12

// QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
// #define LCD_LED PBout(LED)  // LCD背光    		 PB9
// 如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
// 以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！

// GPIO置位（拉高）
// #define LCD_CS_SET  GPIO_TYPE->BSRR = 1 << LCD_CS   // 片选端口  	PB11
// #define LCD_RS_SET  GPIO_TYPE->BSRR = 1 << LCD_RS   // 数据/命令  PB10
// #define LCD_RST_SET GPIO_TYPE->BSRR = 1 << LCD_RST  // 复位			  PB12

// // GPIO复位（拉低）
// #define LCD_CS_CLR  GPIO_TYPE->BRR = 1 << LCD_CS   // 片选端口  	PB11
// #define LCD_RS_CLR  GPIO_TYPE->BRR = 1 << LCD_RS   // 数据/命令  PB10
// #define LCD_RST_CLR GPIO_TYPE->BRR = 1 << LCD_RST  // 复位			  PB12

#define LCD_CS_DOWN  LCD_MODE->BRR = LCD_CS
#define LCD_RS_DOWN  LCD_MODE->BRR = LCD_RS
#define LCD_RST_DOWN LCD_MODE->BRR = LCD_RST
// #define LCD_LED_DOWN HAL_GPIO_WritePin(GPIO_TYPE, LCD_LED, RESET)

#define LCD_CS_UP  LCD_MODE->BSRR = LCD_CS
#define LCD_RS_UP  LCD_MODE->BSRR = LCD_RS
#define LCD_RST_UP LCD_MODE->BSRR = LCD_RST
// #define LCD_LED_UP HAL_GPIO_WritePin(LCD_MODE, LCD_LED, SET)

// #define LCD_CS_DOWN  HAL_GPIO_WritePin(LCD_MODE, LCD_CS, RESET)
// #define LCD_RS_DOWN  HAL_GPIO_WritePin(LCD_MODE, LCD_RS, RESET)
// #define LCD_RST_DOWN HAL_GPIO_WritePin(LCD_MODE, LCD_RST, RESET)
#define LCD_LED_DOWN HAL_GPIO_WritePin(GPIO_TYPE, LCD_LED, RESET)

// #define LCD_CS_UP  HAL_GPIO_WritePin(LCD_MODE, LCD_CS, SET)
// #define LCD_RS_UP  HAL_GPIO_WritePin(LCD_MODE, LCD_RS, SET)
// #define LCD_RST_UP HAL_GPIO_WritePin(LCD_MODE, LCD_RST, SET)
#define LCD_LED_UP HAL_GPIO_WritePin(LCD_MODE, LCD_LED, SET)
