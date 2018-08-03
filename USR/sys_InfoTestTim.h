/**
  ******************************************************************************
  * @file    sys_InfoTestTim.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    28-May-2018
  * @brief   Interface of source file sys_InfoTestTim.c     
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_INFO_TEST_TIM_H
#define _SYS_INFO_TEST_TIM_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
extern volatile  uint32_t  ulHighFrequencyTimerTicks; 
/* function declarations --------------------------------------------------------------------*/
void InfoTestTimeInit(uint32_t TickPriority);

#endif /* end of _SYS_INFO_TEST_TIM_H */





