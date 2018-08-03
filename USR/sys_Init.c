/**
  ******************************************************************************
  * @file    sys_Init.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   all user bsp init  
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "sys_Init.h"
#include "sys_FramAlloc.h"

/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/


/* function body -------------------------------------------------------------------------------------*/


void sys_Init(void)
{				
	  /* IO外设初始化 */
   	GPIO_Init();
	
	  /* RTC初始化 */
	  RTC_Init();
	
	  /* I2C初始化 */
	  Fram_GPIO_init();
	
	  /* 上位机、下位机串口初始化 */
	  UART_Init();

    /* 参数初始化 */
		Opt_Init();

    /* 告警初始化 */
    AlmRelate_Init();

    /* 电能累加初始化 */
    Acq_EnergyInit();
		
		/* 历史电能初始化 */
		gPTE_Init();
		return;
}








/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************************************/


