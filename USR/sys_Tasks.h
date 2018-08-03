/**
  ******************************************************************************
  * @file    sys_Tasks.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    28-May-2018
  * @brief   Interface of source file sys_Tasks.c     
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_TASKS_H
#define _SYS_TASKS_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
extern osThreadId Data_TaskHandle;
extern osThreadId IAP_TaskHandle;
extern osThreadId ETH_TaskHandle;
extern osThreadId PC_TaskHandle;
extern osThreadId SUB_TaskHandle;
extern osThreadId OPT_TaskHandle;

extern osMessageQId  PC_QueueHandle;
extern osMessageQId  SUB_QueueHandle;

/* function declarations --------------------------------------------------------------------*/
void MX_FREERTOS_Init(void);



#endif /* end of _SYS_TASKS_H */




