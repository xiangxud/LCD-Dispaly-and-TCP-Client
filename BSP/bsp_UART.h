/**
  ******************************************************************************
  * @file    bsp_UART.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   Interface of source file bsp_UART.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_UART_H
#define _BSP_UART_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/

#define   PC_RxLen      1024
#define   SUB_RxLen     1152 


struct MessageType
{
	uint16_t Rx_Len;
	uint8_t  Rx_Buffer[PC_RxLen];
};



/* macro definitions ------------------------------------------------------------------------*/	
/* Expanded Variables -----------------------------------------------------------------------*/
extern UART_HandleTypeDef   hPC_UART;
extern UART_HandleTypeDef   hSUB_UART;

extern DMA_HandleTypeDef    hPC_UART_DMARx;
extern DMA_HandleTypeDef    hPC_UART_DMATx;
extern DMA_HandleTypeDef    hSUB_UART_DMARx;
extern DMA_HandleTypeDef    hSUB_UART_DMATx;

/* function declarations --------------------------------------------------------------------*/
void UART_Init(void);
void UART_Rx_IDLE(UART_HandleTypeDef *huart);

#endif /* end of _BSP_UART_H */



