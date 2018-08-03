#ifndef __SSD1963_H
#define __SSD1963_H

#include "stm32f4xx_hal.h"


void LCD_Init(void);
void LCD_Clear(unsigned int color);
void LCD_DrawLine(uint16_t usX1,uint16_t usY1,uint16_t usX2,uint16_t usY2,uint16_t usColor);
void LCD_DrawCross(uint16_t x,uint16_t y);
uint16_t SSD1963_Read_Data(void);

/******************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
原理图选择 FSMC_A16 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6000 0000
RAM基地址 = 0X6002 0000 = 0X6000 0000+(1<<(16+1))
如果电路设计时选择不同的地址线时，地址要重新计算  
*******************************************************************************/

/******************************* SSD1963 显示屏的 FSMC 参数定义 ***************/
#define FSMC_LCD_CMD                   ((uint32_t)0x60000000)	     //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define FSMC_LCD_DATA                  ((uint32_t)0x60020000)      //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址    
#define BACKGROUND		                 WHITE     //默认背景颜色


#define BLUE                          (uint16_t)0x001F	   //蓝色 
#define GREEN                         (uint16_t)0x07E0	   //绿色 
#define RED                           (uint16_t)0xF800	   //红色 
#define CYAN                          (uint16_t)0x07FF	   //蓝绿色，青色
#define MAGENTA                       (uint16_t)0xF81F	   //红紫色，洋红色
#define YELLOW                        (uint16_t)0xFFE0	   //黄色 
#define LIGHTBLUE                     (uint16_t)0x841F
#define LIGHTGREEN                    (uint16_t)0x87F0
#define LIGHTRED                      (uint16_t)0xFC10
#define LIGHTCYAN                     (uint16_t)0x87FF
#define LIGHTMAGENTA                  (uint16_t)0xFC1F
#define LIGHTYELLOW                   (uint16_t)0xFFF0
#define DARKBLUE                      (uint16_t)0x0010
#define DARKGREEN                     (uint16_t)0x0400
#define DARKRED                       (uint16_t)0x8000
#define DARKCYAN                      (uint16_t)0x0410
#define DARKMAGENTA                   (uint16_t)0x8010
#define DARKYELLOW                    (uint16_t)0x8400
#define WHITE                         (uint16_t)0xFFFF	   //白色
#define LIGHTGRAY                     (uint16_t)0xD69A
#define GRAY                          (uint16_t)0x8410	   //灰色 
#define DARKGRAY                      (uint16_t)0x4208
#define BLACK                         (uint16_t)0x0000	   //黑色 
#define BROWN                         (uint16_t)0xA145
#define ORANGE                        (uint16_t)0xFD20



#define HDP                479                //液晶屏分辨率长
#define HT                 525                //水平总周期 = (HT + 1) 个像素 //
#define HPS                40                 //水平同步脉冲起始位置= (HPS + 1) 个像素
#define LPS                6                  //水平显示时段起始位置 = LPS 个像素
#define HPW                39                   //水平同步脉冲宽度 = (HPW + 1) pixels

#define VDP                271                //液晶屏分辨率宽
#define VT                 288                //垂直总数 = (VT + 1) 行
#define VPS                12                 //垂直同步脉冲开始位置 = VPS 行
#define FPS                8                  //垂直显示时段开始位置 = FPS 行
#define VPW                11                 //垂直同步脉冲宽度 = (VPW + 1) 行


#define SSD1963_RESET_LOW()                 HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET);
#define SSD1963_RESET_HIGH()                HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET);

void SSD1963_Write_Cmd(uint16_t Cmd);
void SSD1963_Write_Data(uint16_t Data);
uint16_t SSD1963_Read_Data(void);

#endif
