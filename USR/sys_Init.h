/**
  ******************************************************************************
  * @file    sys_BSPInit.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   Interface of source file sys_BSPInit.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_INIT_H
#define _SYS_INIT_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_GPIO.h"
#include "bsp_RTC.h"
#include "bsp_Fram.h"
#include "bsp_ATT.h"
#include "bsp_UART.h"
#include "app_Optimset.h"
#include "app_Acquisition.h"
#include "app_Alarm.h"
#include "app_PTEnergy.h"
#include "GUI.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void sys_Init(void);

#endif /* end _SYS_INIT_H */



