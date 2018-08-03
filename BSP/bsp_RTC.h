/**
  ******************************************************************************
  * @file    bsp_RTC.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   Interface of source file bsp_RTC.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_RTC_H
#define _BSP_RTC_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/	
/* Expanded Variables -----------------------------------------------------------------------*/
extern RTC_HandleTypeDef   hrtc;
/* function declarations --------------------------------------------------------------------*/
void RTC_Init(void);

#endif /* end of _BSP_RTC_H */



