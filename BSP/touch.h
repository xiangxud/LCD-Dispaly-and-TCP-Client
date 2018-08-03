#ifndef __TOUCH_H
#define __TOUCH_H

#include "stm32f4xx_hal.h"
#include "fsmc.h"
#include "SSD1963.h"

#define TP_SPI_CS_HIGH()                      HAL_GPIO_WritePin(TP_SPI_CS_GPIO_Port, TP_SPI_CS_Pin, GPIO_PIN_SET)
#define TP_SPI_CS_LOW()                       HAL_GPIO_WritePin(TP_SPI_CS_GPIO_Port, TP_SPI_CS_Pin, GPIO_PIN_RESET)

#define TP_SPI_SCK_HIGH()                     HAL_GPIO_WritePin(TP_SPI_SCK_GPIO_Port, TP_SPI_SCK_Pin, GPIO_PIN_SET)
#define TP_SPI_SCK_LOW()                      HAL_GPIO_WritePin(TP_SPI_SCK_GPIO_Port, TP_SPI_SCK_Pin, GPIO_PIN_RESET)

#define TP_SPI_MOSI_HIGH()                    HAL_GPIO_WritePin(TP_SPI_MOSI_GPIO_Port, TP_SPI_MOSI_Pin, GPIO_PIN_SET)
#define TP_SPI_MOSI_LOW()                     HAL_GPIO_WritePin(TP_SPI_MOSI_GPIO_Port, TP_SPI_MOSI_Pin, GPIO_PIN_RESET)

#define TP_SPI_MISO_READ()                    HAL_GPIO_ReadPin(TP_SPI_MISO_GPIO_Port,TP_SPI_MISO_Pin)
     
#define TP_IRQ_READ()                         HAL_GPIO_ReadPin(TP_IRQ_GPIO_Port,TP_IRQ_Pin)        


#define CMD_RDX                                       0X90  //10010000即用差分方式读X坐标
#define CMD_RDY	                                      0XD0  //11010000即用差分方式读Y坐标 
#define XPT2046_SPIFLASH_ADDR                         (0)


typedef struct TP_Calibration_TypeDef
{
   uint16_t CalFlag;     /*!< 校准标志 */
	 uint16_t CalData_XL;  /*!< X方向左 */
	 uint16_t CalData_XR;  /*!< X方向右 */
	 uint16_t CalData_YT;  /*!< Y方向上 */
	 uint16_t CalData_YB;  /*!< Y方向下 */	 
}TP_Calibration_TypeDef;

extern TP_Calibration_TypeDef   TP_Cal;
extern uint16_t adc_x;
extern uint16_t adc_y;
int  GUI_TOUCH_X_MeasureX(void);
int  GUI_TOUCH_X_MeasureY(void);
void Touch_Init_GPIO(void);
uint8_t XPT2046_Touch_Calibrate(void);

/* 类型定义 ------------------------------------------------------------------*/
#pragma pack(4)     // 4字节对齐
typedef struct XPT2046_Calibration
{	
  int16_t lcd_x[5];
	int16_t lcd_y[5];
  int16_t touch_x[5];
  int16_t touch_y[5];
	int32_t adjust[7];
	float kx;
	float ky;
}XPT2046_Calibration;
#pragma pack()
extern XPT2046_Calibration cal_value;
#endif
