/**
  ******************************************************************************
  * @file    bsp_UART.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   UART configuration 
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -----------------------------------------------------------------------------------------*/
#include "bsp_UART.h"
#include "sys_PPriority.h"
#include "sys_Tasks.h"

/* private type definitions -------------------------------------------------------------------------*/

struct MessageType  PCMsg;
struct MessageType  SUBMsg;

/* private macro definitions ------------------------------------------------------------------------*/	

#define PC_UARTx                   USART1
#define PC_UART_TX_Pin             GPIO_PIN_9
#define PC_UART_TX_GPIO_Port       GPIOA
#define PC_UART_RX_Pin             GPIO_PIN_10
#define PC_UART_RX_GPIO_Port       GPIOA
#define PC_DMARx_STREAM            DMA2_Stream2 
#define PC_DMATx_STREAM            DMA2_Stream7
#define PC_DMA_CHANNEL             DMA_CHANNEL_4
#define PC_UART_IRQn               USART1_IRQn
#define PC_DMATx_IRQn              DMA2_Stream7_IRQn


#define SUB_UARTx                  USART3
#define SUB_UART_TX_Pin            GPIO_PIN_10
#define SUB_UART_TX_GPIO_Port      GPIOC
#define SUB_UART_RX_Pin            GPIO_PIN_11
#define SUB_UART_RX_GPIO_Port      GPIOC
#define SUB_DMARx_STREAM           DMA1_Stream1 
#define SUB_DMATx_STREAM           DMA1_Stream3
#define SUB_DMA_CHANNEL            DMA_CHANNEL_4
#define SUB_UART_IRQn              USART3_IRQn
#define SUB_DMATx_IRQn             DMA1_Stream3_IRQn


/* private variables ---------------------------------------------------------------------------------*/
UART_HandleTypeDef   hPC_UART;
UART_HandleTypeDef   hSUB_UART;

DMA_HandleTypeDef    hPC_UART_DMARx;
DMA_HandleTypeDef    hPC_UART_DMATx;
DMA_HandleTypeDef    hSUB_UART_DMARx;
DMA_HandleTypeDef    hSUB_UART_DMATx;

/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/


/**
  *@brief  UART GPIO Pin congiguration
	*@param  *uartHandle
	*@retval NONE
	*@note   callback by HAL driver
	*/
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
   GPIO_InitTypeDef   GPIO_InitStruct;
	
	 if(uartHandle->Instance == PC_UARTx)
	 {
		    __HAL_RCC_USART1_CLK_ENABLE();
		 
		 		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
				GPIO_InitStruct.Pull = GPIO_PULLUP;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		 
				GPIO_InitStruct.Pin = PC_UART_TX_Pin;
				HAL_GPIO_Init(PC_UART_TX_GPIO_Port, &GPIO_InitStruct);
		    GPIO_InitStruct.Pin = PC_UART_RX_Pin;
				HAL_GPIO_Init(PC_UART_RX_GPIO_Port, &GPIO_InitStruct);
		 
		    /* 开启DMA2时钟 */
		    __HAL_RCC_DMA2_CLK_ENABLE();
		 
		    hPC_UART_DMARx.Instance = PC_DMARx_STREAM;
				hPC_UART_DMARx.Init.Channel = PC_DMA_CHANNEL;
				hPC_UART_DMARx.Init.Direction = DMA_PERIPH_TO_MEMORY;
				hPC_UART_DMARx.Init.PeriphInc = DMA_PINC_DISABLE;
				hPC_UART_DMARx.Init.MemInc = DMA_MINC_ENABLE;
				hPC_UART_DMARx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
				hPC_UART_DMARx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
				hPC_UART_DMARx.Init.Mode = DMA_NORMAL;
				hPC_UART_DMARx.Init.Priority = DMA_PRIORITY_LOW;
				hPC_UART_DMARx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;	
        HAL_DMA_Init(&hPC_UART_DMARx);	
        
        __HAL_LINKDMA(uartHandle, hdmarx, hPC_UART_DMARx);

        hPC_UART_DMATx.Instance = PC_DMATx_STREAM;
				hPC_UART_DMATx.Init.Channel = PC_DMA_CHANNEL;
        hPC_UART_DMATx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hPC_UART_DMATx.Init.PeriphInc = DMA_PINC_DISABLE;
        hPC_UART_DMATx.Init.MemInc = DMA_MINC_ENABLE;
        hPC_UART_DMATx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hPC_UART_DMATx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hPC_UART_DMATx.Init.Mode = DMA_NORMAL;
        hPC_UART_DMATx.Init.Priority = DMA_PRIORITY_LOW;
        hPC_UART_DMATx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
				HAL_DMA_Init(&hPC_UART_DMATx);	
        
        __HAL_LINKDMA(uartHandle, hdmatx, hPC_UART_DMATx);
				
				HAL_NVIC_SetPriority(PC_UART_IRQn, PC_RxPriority, 0);
				HAL_NVIC_EnableIRQ(PC_UART_IRQn);
				
				HAL_NVIC_SetPriority(PC_DMATx_IRQn, PC_TxPriority, 0);
				HAL_NVIC_EnableIRQ(PC_DMATx_IRQn);
	 }
	 else if(uartHandle->Instance == SUB_UARTx)
	 {
		 
		    __HAL_RCC_USART3_CLK_ENABLE();
		 
	    	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
				GPIO_InitStruct.Pull = GPIO_PULLUP;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		 
				GPIO_InitStruct.Pin = SUB_UART_TX_Pin;
				HAL_GPIO_Init(SUB_UART_TX_GPIO_Port, &GPIO_InitStruct);
		    GPIO_InitStruct.Pin = SUB_UART_RX_Pin;
				HAL_GPIO_Init(SUB_UART_RX_GPIO_Port, &GPIO_InitStruct);
		 
		    /* 开启DMA1时钟 */
		    __HAL_RCC_DMA1_CLK_ENABLE();
		 
		    hSUB_UART_DMARx.Instance = SUB_DMARx_STREAM;
				hSUB_UART_DMARx.Init.Channel = SUB_DMA_CHANNEL;
				hSUB_UART_DMARx.Init.Direction = DMA_PERIPH_TO_MEMORY;
				hSUB_UART_DMARx.Init.PeriphInc = DMA_PINC_DISABLE;
				hSUB_UART_DMARx.Init.MemInc = DMA_MINC_ENABLE;
				hSUB_UART_DMARx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
				hSUB_UART_DMARx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
				hSUB_UART_DMARx.Init.Mode = DMA_NORMAL;
				hSUB_UART_DMARx.Init.Priority = DMA_PRIORITY_LOW;
				hSUB_UART_DMARx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;	
        HAL_DMA_Init(&hSUB_UART_DMARx);	
        
        __HAL_LINKDMA(uartHandle, hdmarx, hSUB_UART_DMARx);

        hSUB_UART_DMATx.Instance = SUB_DMATx_STREAM;
				hSUB_UART_DMATx.Init.Channel = SUB_DMA_CHANNEL;
        hSUB_UART_DMATx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hSUB_UART_DMATx.Init.PeriphInc = DMA_PINC_DISABLE;
        hSUB_UART_DMATx.Init.MemInc = DMA_MINC_ENABLE;
        hSUB_UART_DMATx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hSUB_UART_DMATx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hSUB_UART_DMATx.Init.Mode = DMA_NORMAL;
        hSUB_UART_DMATx.Init.Priority = DMA_PRIORITY_LOW;
        hSUB_UART_DMATx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
				HAL_DMA_Init(&hSUB_UART_DMATx);	
        
        __HAL_LINKDMA(uartHandle, hdmatx, hSUB_UART_DMATx);
				
				HAL_NVIC_SetPriority(SUB_UART_IRQn, SUB_RxPriority, 0);
				HAL_NVIC_EnableIRQ(SUB_UART_IRQn);
				
				HAL_NVIC_SetPriority(SUB_DMATx_IRQn, SUB_TxPriority, 0);
				HAL_NVIC_EnableIRQ(SUB_DMATx_IRQn);
	 }
}


void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
    if(uartHandle->Instance == PC_UARTx)
		{
		   __HAL_RCC_USART1_CLK_DISABLE();
			
			 HAL_GPIO_DeInit(PC_UART_TX_GPIO_Port,PC_UART_TX_Pin);
		   HAL_GPIO_DeInit(PC_UART_RX_GPIO_Port,PC_UART_RX_Pin);
			
			 HAL_DMA_DeInit(uartHandle->hdmarx);
			 HAL_DMA_DeInit(uartHandle->hdmatx);
			
			 HAL_NVIC_DisableIRQ(PC_UART_IRQn);
			 HAL_NVIC_DisableIRQ(PC_DMATx_IRQn);
		}
		else if(uartHandle->Instance == SUB_UARTx)
		{
		   __HAL_RCC_USART3_CLK_DISABLE();
			
			 HAL_GPIO_DeInit(SUB_UART_TX_GPIO_Port,SUB_UART_TX_Pin);
		   HAL_GPIO_DeInit(SUB_UART_RX_GPIO_Port,SUB_UART_RX_Pin);
			
			 HAL_DMA_DeInit(uartHandle->hdmarx);
			 HAL_DMA_DeInit(uartHandle->hdmatx);
			
			 HAL_NVIC_DisableIRQ(SUB_UART_IRQn);
			 HAL_NVIC_DisableIRQ(SUB_DMATx_IRQn); 
		}
}


/**
  *@brief  UART configuration
	*@param  NONE
	*@retval NONE
	*@note   
	*/
void UART_Init(void)
{
	 /* config PC USART */
   hPC_UART.Instance = PC_UARTx;
	 hPC_UART.Init.BaudRate = 38400;
	 hPC_UART.Init.WordLength = UART_WORDLENGTH_8B;
	 hPC_UART.Init.StopBits = UART_STOPBITS_1;
	 hPC_UART.Init.Parity = UART_PARITY_NONE;
	 hPC_UART.Init.Mode = UART_MODE_TX_RX;
	 hPC_UART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	 hPC_UART.Init.OverSampling = UART_OVERSAMPLING_16;
	 HAL_UART_Init(&hPC_UART);
	 
	 __HAL_UART_ENABLE_IT(&hPC_UART, UART_IT_IDLE);
	
	 /* config SUB USART */
	 hSUB_UART.Instance = SUB_UARTx;
	 hSUB_UART.Init.BaudRate = 57600;
	 hSUB_UART.Init.WordLength = UART_WORDLENGTH_8B;
	 hSUB_UART.Init.StopBits = UART_STOPBITS_1;
	 hSUB_UART.Init.Parity = UART_PARITY_NONE;
	 hSUB_UART.Init.Mode = UART_MODE_TX_RX;
	 hSUB_UART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	 hSUB_UART.Init.OverSampling = UART_OVERSAMPLING_16;
	 HAL_UART_Init(&hSUB_UART);
	 
	 __HAL_UART_ENABLE_IT(&hSUB_UART, UART_IT_IDLE);
}	


/* DMA发送完成中断回调函数 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   __HAL_DMA_DISABLE(huart->hdmatx);
}



/* 串口接收空闲中断服务函数 */
void UART_Rx_IDLE(UART_HandleTypeDef *huart)
{
   uint32_t temp;		 
	
	 if( __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
	 {
			__HAL_UART_CLEAR_IDLEFLAG(huart);
		  HAL_UART_DMAStop(huart);
			temp = huart->hdmarx->Instance->NDTR;
		 
		  if(huart->Instance == PC_UARTx)
	    {				  			
					PCMsg.Rx_Len = PC_RxLen - temp;
					HAL_UART_Receive_DMA(huart, PCMsg.Rx_Buffer, PC_RxLen);				
					  
				  osMessagePut(PC_QueueHandle, (uint32_t)&PCMsg, 0);
			}
			else if(huart->Instance == SUB_UARTx)
			{
			    SUBMsg.Rx_Len = SUB_RxLen - temp;
				  HAL_UART_Receive_DMA(huart, SUBMsg.Rx_Buffer, SUB_RxLen);
				
				  osMessagePut(SUB_QueueHandle, (uint32_t)&SUBMsg, 0);
			}
	 }
}

/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/




