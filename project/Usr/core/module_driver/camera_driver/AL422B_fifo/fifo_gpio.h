#pragma once

#include "gpio.h"

/*
RE 引脚和 WEN 引脚为一体， WEN高电平时写使能，WEN 低电平时 读使能
*/

#define OV7725_DATA_GPIO_Port GPIOF

// #define FIFO_OE(x)   HAL_GPIO_WritePin(OV7725_OE_GPIO_Port, OV7725_OE_Pin, x)
// #define FIFO_WRST(x) HAL_GPIO_WritePin(OV7725_WRST_GPIO_Port, OV7725_WRST_Pin, x)
// #define FIFO_RRST(x) HAL_GPIO_WritePin(OV7725_RRST_GPIO_Port, OV7725_RRST_Pin, x)
// #define FIFO_RCLK(x) HAL_GPIO_WritePin(OV7725_RCLK_GPIO_Port, OV7725_RCLK_Pin, x)
// #define FIFO_WEN(x)  HAL_GPIO_WritePin(OV7725_WEN_GPIO_Port, OV7725_WEN_Pin, x)

#define FIFO_OE_H OV7725_OE_GPIO_Port->BSRR = OV7725_OE_Pin
#define FIFO_OE_L OV7725_OE_GPIO_Port->BRR = OV7725_OE_Pin /*拉低使FIFO输出使能*/

#define FIFO_WRST_H OV7725_WRST_GPIO_Port->BSRR = OV7725_WRST_Pin /*拉高允许FIFO写(数据from摄像头)指针运动 */
#define FIFO_WRST_L OV7725_WRST_GPIO_Port->BRR = OV7725_WRST_Pin  /*拉低使FIFO写(数据from摄像头)指针复位*/

#define FIFO_RRST_H OV7725_RRST_GPIO_Port->BSRR = OV7725_RRST_Pin /*拉高允许FIFO读(数据从FIFO输出)指针运动 */
#define FIFO_RRST_L OV7725_RRST_GPIO_Port->BRR = OV7725_RRST_Pin  /*拉低使FIFO读(数据从FIFO输出)指针复位 */

#define FIFO_RCLK_H OV7725_RCLK_GPIO_Port->BSRR = OV7725_RCLK_Pin
#define FIFO_RCLK_L OV7725_RCLK_GPIO_Port->BRR = OV7725_RCLK_Pin /*FIFO输出数据时钟*/

#define FIFO_WE_H OV7725_WEN_GPIO_Port->BSRR = OV7725_WEN_Pin /*拉高使FIFO写允许*/
#define FIFO_WE_L OV7725_WEN_GPIO_Port->BRR = OV7725_WEN_Pin
