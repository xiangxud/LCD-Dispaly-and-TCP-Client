#ifndef __SSD1963_H
#define __SSD1963_H

#include "stm32f4xx_hal.h"


void LCD_Init(void);
void LCD_Clear(unsigned int color);
void LCD_DrawLine(uint16_t usX1,uint16_t usY1,uint16_t usX2,uint16_t usY2,uint16_t usColor);
void LCD_DrawCross(uint16_t x,uint16_t y);
uint16_t SSD1963_Read_Data(void);

/******************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
ԭ��ͼѡ�� FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6000 0000
RAM����ַ = 0X6002 0000 = 0X6000 0000+(1<<(16+1))
�����·���ʱѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
*******************************************************************************/

/******************************* SSD1963 ��ʾ���� FSMC �������� ***************/
#define FSMC_LCD_CMD                   ((uint32_t)0x60000000)	     //FSMC_Bank1_NORSRAM1����LCD��������ĵ�ַ
#define FSMC_LCD_DATA                  ((uint32_t)0x60020000)      //FSMC_Bank1_NORSRAM1����LCD���ݲ����ĵ�ַ    
#define BACKGROUND		                 WHITE     //Ĭ�ϱ�����ɫ


#define BLUE                          (uint16_t)0x001F	   //��ɫ 
#define GREEN                         (uint16_t)0x07E0	   //��ɫ 
#define RED                           (uint16_t)0xF800	   //��ɫ 
#define CYAN                          (uint16_t)0x07FF	   //����ɫ����ɫ
#define MAGENTA                       (uint16_t)0xF81F	   //����ɫ�����ɫ
#define YELLOW                        (uint16_t)0xFFE0	   //��ɫ 
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
#define WHITE                         (uint16_t)0xFFFF	   //��ɫ
#define LIGHTGRAY                     (uint16_t)0xD69A
#define GRAY                          (uint16_t)0x8410	   //��ɫ 
#define DARKGRAY                      (uint16_t)0x4208
#define BLACK                         (uint16_t)0x0000	   //��ɫ 
#define BROWN                         (uint16_t)0xA145
#define ORANGE                        (uint16_t)0xFD20



#define HDP                479                //Һ�����ֱ��ʳ�
#define HT                 525                //ˮƽ������ = (HT + 1) ������ //
#define HPS                40                 //ˮƽͬ��������ʼλ��= (HPS + 1) ������
#define LPS                6                  //ˮƽ��ʾʱ����ʼλ�� = LPS ������
#define HPW                39                   //ˮƽͬ�������� = (HPW + 1) pixels

#define VDP                271                //Һ�����ֱ��ʿ�
#define VT                 288                //��ֱ���� = (VT + 1) ��
#define VPS                12                 //��ֱͬ�����忪ʼλ�� = VPS ��
#define FPS                8                  //��ֱ��ʾʱ�ο�ʼλ�� = FPS ��
#define VPW                11                 //��ֱͬ�������� = (VPW + 1) ��


#define SSD1963_RESET_LOW()                 HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET);
#define SSD1963_RESET_HIGH()                HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET);

void SSD1963_Write_Cmd(uint16_t Cmd);
void SSD1963_Write_Data(uint16_t Data);
uint16_t SSD1963_Read_Data(void);

#endif
