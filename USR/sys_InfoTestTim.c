/**
  ******************************************************************************
  * @file    sys_InfoTestTim.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    28-May-2018
  * @brief   TIM for freeRTOS information test init	
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "sys_InfoTestTim.h"

/* private type definitions -------------------------------------------------------------------------*/
volatile  uint32_t  ulHighFrequencyTimerTicks = 0ul;
TIM_HandleTypeDef        htim2;

/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables --------------------------------------------------------------------------------*/
#define  ulHighINTERRUPT_FREQUENCY     20000

/* Expanded Variables -------------------------------------------------------------------------------*/
/* private functions prototype ----------------------------------------------------------------------*/

/* function body ------------------------------------------------------------------------------------*/


void InfoTestTimeInit(uint32_t TickPriority)
{
  uint32_t              uwTimclock = 0;
  
  /*Configure the TIM2 IRQ priority */
  HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority ,0); 
  
  /* Enable the TIM2 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM2_IRQn); 
  
  /* Enable TIM2 clock */
  __HAL_RCC_TIM2_CLK_ENABLE();
  
  /* Compute TIM1 clock */
  uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  
  /* Initialize TIM1 */
  htim2.Instance = TIM2;
  
  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  htim2.Init.Period = uwTimclock/100/ulHighINTERRUPT_FREQUENCY-1;
  htim2.Init.Prescaler = 100-1;
  htim2.Init.ClockDivision = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&htim2);

    /* Start the TIM time Base generation in interrupt mode */
   HAL_TIM_Base_Start_IT(&htim2);
	 
	 return;
}

/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/



