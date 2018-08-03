/**
  ******************************************************************************
  * @file    bsp_IWDG.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   Interface of source file bsp_IWDG.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_IWDG_H
#define _BSP_IWDG_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
extern IWDG_HandleTypeDef   hiwdg;
/* function declarations --------------------------------------------------------------------*/
void IWDG_Init(void);

#endif /* end _SYS_INIT_H */





