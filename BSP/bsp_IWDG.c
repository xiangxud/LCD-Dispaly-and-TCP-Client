/**
  ******************************************************************************
  * @file    bsp_IWDG.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   all user bsp init  
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "bsp_IWDG.h"

/* private type definitions --------------------------------------------------*/
/* private macro definitions -------------------------------------------------*/	
/* private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef   hiwdg;
		
/* Expanded Variables --------------------------------------------------------*/
/* private functions prototype -----------------------------------------------*/
/* function body -------------------------------------------------------------*/
void IWDG_Init(void)
{		
		hiwdg.Instance = IWDG;
		hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
		hiwdg.Init.Reload = 0xfff;
		
		HAL_IWDG_Init(&hiwdg);
}




/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************/





