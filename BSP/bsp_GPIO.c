/**
  ******************************************************************************
  * @file    bsp_GPIO.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   this file configs GPIO clock and I/O PIN    
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -----------------------------------------------------------------------------------------*/
#include "bsp_GPIO.h"
#include "fsmc.h"	
/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/

/**
  *@brief  GPIO init
  *@param  NONE
  *@retval NONE
  *@note   此处打开所有用到的GPIO端口的时钟
  */
void GPIO_Init(void)
{
    GPIO_InitTypeDef      GPIO_InitStruct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
   
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    GPIO_InitStruct.Pin = PC_LED_PIN;
    HAL_GPIO_Init(PC_LED_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ALM_LED_PIN;
    HAL_GPIO_Init(ALM_LED_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = RUN_LED_PIN;
    HAL_GPIO_Init(RUN_LED_GPIO_PORT, &GPIO_InitStruct);
	
	  GPIO_InitStruct.Pin = ALM_AURAL_PIN;
    HAL_GPIO_Init(ALM_AURAL_GPIO_PORT, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = ALM_VISUAL_PIN;
    HAL_GPIO_Init(ALM_VISUAL_GPIO_PORT, &GPIO_InitStruct);
		
		PC_LED_OFF();
		RUN_LED_OFF();
		ALM_LED_OFF();
		ALM_AURAL_OFF();
		ALM_VISUAL_OFF();
    
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    GPIO_InitStruct.Pin = BUSA_QF_PIN;
    HAL_GPIO_Init(BUSA_QF_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BUSB_QF_PIN;
    HAL_GPIO_Init(BUSB_QF_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BUSA_SPD_PIN;
    HAL_GPIO_Init(BUSA_SPD_GPIO_PORT, &GPIO_InitStruct);
   
    GPIO_InitStruct.Pin = BUSB_SPD_PIN;
    HAL_GPIO_Init(BUSB_SPD_GPIO_PORT, &GPIO_InitStruct); 

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = LCD_BL_CTR_Pin|TP_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
}



/**
  *@breif ATT GPIO pins init
	*@param NOEN
	*@retval NONE
	*@note Callback by HAl driver
	*/
void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
   GPIO_InitTypeDef     GPIO_InitStruct;
	
	 if(spiHandle->Instance == DA_SPIx)
	 {		 
		  DA_SPIx_RCC_CLK_ENABLE();
		 
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		 
	    GPIO_InitStruct.Pin = DA_SPI_SCK_PIN;
		  HAL_GPIO_Init(DA_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
		 
		 	GPIO_InitStruct.Pin = DA_SPI_MISO_PIN;
		  HAL_GPIO_Init(DA_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
		
		  GPIO_InitStruct.Pin = DA_SPI_MOSI_PIN;
		  HAL_GPIO_Init(DA_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
		 
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		 
		  GPIO_InitStruct.Pin = DA_ATT1_CS_PIN;
		  HAL_GPIO_Init(DA_ATT1_CS_GPIO_PORT, &GPIO_InitStruct);
		  DA_ATT1_CS_DISABLE();
		 
		  GPIO_InitStruct.Pin = DA_ATT2_CS_PIN;
      HAL_GPIO_Init(DA_ATT2_CS_GPIO_PORT, &GPIO_InitStruct);	
      DA_ATT2_CS_DISABLE();	

		  GPIO_InitStruct.Pin = DA_ATT1_RESET_PIN;
		  HAL_GPIO_Init(DA_ATT1_RESET_GPIO_PORT, &GPIO_InitStruct);
		  DA_ATT1_RESET_ENABLE();
		 
		  GPIO_InitStruct.Pin = DA_ATT2_RESET_PIN;
      HAL_GPIO_Init(DA_ATT2_RESET_GPIO_PORT, &GPIO_InitStruct);	
      DA_ATT2_RESET_ENABLE();	
	 }
	 else if(spiHandle->Instance == XPT2046_SPIx)
	 {
	   	DA_SPIx_RCC_CLK_ENABLE();
		 
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		 
	    GPIO_InitStruct.Pin = XPT2046_SCK_PIN;
		  HAL_GPIO_Init(XPT2046_SCK_GPIO_PORT, &GPIO_InitStruct);
		 
		 	GPIO_InitStruct.Pin = XPT2046_MISO_PIN;
		  HAL_GPIO_Init(XPT2046_MISO_GPIO_PORT, &GPIO_InitStruct);
		
		  GPIO_InitStruct.Pin = XPT2046_MOSI_PIN;
		  HAL_GPIO_Init(XPT2046_MOSI_GPIO_PORT, &GPIO_InitStruct);

		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		 
		  GPIO_InitStruct.Pin = XPT2046_CS_PIN;
		  HAL_GPIO_Init(XPT2046_CS_GPIO_PORT, &GPIO_InitStruct);
		  DA_ATT1_CS_DISABLE();

		  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

      GPIO_InitStruct.Pin = XPT2046_BUSY_PIN;
		  HAL_GPIO_Init(XPT2046_BUSY_GPIO_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = XPT2046_IRQ_PIN;
		  HAL_GPIO_Init(XPT2046_IRQ_GPIO_PORT, &GPIO_InitStruct);				
	 }
}

/**
  *@breif ATT GPIO pins deinit
	*@param NOEN
	*@retval NONE
	*@note Callback by HAl driver
	*/
void HAL_SPI_MspDeinit(SPI_HandleTypeDef *spiHandle)
{
   if(spiHandle->Instance == DA_SPIx)
	 {
	     DA_SPIx_RCC_CLK_DISABLE();
		   HAL_GPIO_DeInit(DA_SPI_SCK_GPIO_PORT, DA_SPI_MISO_PIN);
		   HAL_GPIO_DeInit(DA_SPI_MISO_GPIO_PORT,DA_SPI_MISO_PIN);
		   HAL_GPIO_DeInit(DA_SPI_MOSI_GPIO_PORT, DA_SPI_MOSI_PIN);
	 }
	 else if(spiHandle->Instance == XPT2046_SPIx)
	 {
	     DA_SPIx_RCC_CLK_DISABLE();
		   HAL_GPIO_DeInit(XPT2046_SCK_GPIO_PORT, XPT2046_MISO_PIN);
		   HAL_GPIO_DeInit(XPT2046_MISO_GPIO_PORT, XPT2046_MISO_PIN);
		   HAL_GPIO_DeInit(XPT2046_MOSI_GPIO_PORT, XPT2046_MOSI_PIN);
	 }
}

	
/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/




