/**
  ******************************************************************************
  * @file    bsp_GPIO.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    19-May-2018
  * @brief   Interface of source file bsp_Fram.c     
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/
#define PC_LED_PIN               GPIO_PIN_3
#define PC_LED_GPIO_PORT         GPIOE
#define ALM_LED_PIN              GPIO_PIN_4
#define ALM_LED_GPIO_PORT        GPIOE
#define RUN_LED_PIN              GPIO_PIN_2
#define RUN_LED_GPIO_PORT        GPIOE

#define PC_LED_ON()              HAL_GPIO_WritePin(PC_LED_GPIO_PORT, PC_LED_PIN, GPIO_PIN_RESET);       
#define PC_LED_OFF()             HAL_GPIO_WritePin(PC_LED_GPIO_PORT, PC_LED_PIN, GPIO_PIN_SET);
#define PC_LED_Twinkle()         HAL_GPIO_TogglePin(PC_LED_GPIO_PORT, PC_LED_PIN);

#define ALM_LED_ON()             HAL_GPIO_WritePin(ALM_LED_GPIO_PORT, ALM_LED_PIN, GPIO_PIN_RESET);
#define ALM_LED_OFF()            HAL_GPIO_WritePin(ALM_LED_GPIO_PORT, ALM_LED_PIN, GPIO_PIN_SET);
#define ALM_LED_Twinkle()        HAL_GPIO_TogglePin(ALM_LED_GPIO_PORT, ALM_LED_PIN);

#define RUN_LED_ON()             HAL_GPIO_WritePin(RUN_LED_GPIO_PORT, RUN_LED_PIN, GPIO_PIN_RESET);
#define RUN_LED_OFF()            HAL_GPIO_WritePin(RUN_LED_GPIO_PORT, RUN_LED_PIN, GPIO_PIN_SET);
#define RUN_LED_Twinkle()        HAL_GPIO_TogglePin(RUN_LED_GPIO_PORT, RUN_LED_PIN);

#define ALM_AURAL_PIN            GPIO_PIN_3
#define ALM_AURAL_GPIO_PORT      GPIOD
#define ALM_VISUAL_PIN           GPIO_PIN_2
#define ALM_VISUAL_GPIO_PORT     GPIOD

#define ALM_AURAL_ON()           HAL_GPIO_WritePin(ALM_AURAL_GPIO_PORT, ALM_AURAL_PIN, GPIO_PIN_SET);
#define ALM_AURAL_OFF()          HAL_GPIO_WritePin(ALM_AURAL_GPIO_PORT, ALM_AURAL_PIN, GPIO_PIN_RESET);
#define ALM_AURAL_Toggle()       HAL_GPIO_TogglePin(ALM_AURAL_GPIO_PORT, ALM_AURAL_PIN);
#define ALM_VISUAL_ON()          HAL_GPIO_WritePin(ALM_VISUAL_GPIO_PORT, ALM_VISUAL_PIN, GPIO_PIN_SET);
#define ALM_VISUAL_OFF()         HAL_GPIO_WritePin(ALM_VISUAL_GPIO_PORT, ALM_VISUAL_PIN, GPIO_PIN_RESET);


#define BUSB_SPD_PIN             GPIO_PIN_5
#define BUSB_SPD_GPIO_PORT       GPIOE
#define BUSA_SPD_PIN             GPIO_PIN_6
#define BUSA_SPD_GPIO_PORT       GPIOE
#define BUSB_QF_PIN              GPIO_PIN_13
#define BUSB_QF_GPIO_PORT        GPIOC
#define BUSA_QF_PIN              GPIO_PIN_0
#define BUSA_QF_GPIO_PORT        GPIOC	

#define BUSA_SPD_State()         HAL_GPIO_ReadPin(BUSA_SPD_GPIO_PORT, BUSA_SPD_PIN)
#define BUSB_SPD_State()         HAL_GPIO_ReadPin(BUSB_SPD_GPIO_PORT, BUSB_SPD_PIN)
#define BUSA_QF_State()          HAL_GPIO_ReadPin(BUSA_QF_GPIO_PORT, BUSA_QF_PIN)
#define BUSB_QF_State()          HAL_GPIO_ReadPin(BUSB_QF_GPIO_PORT, BUSB_QF_PIN)

/* ATT7022E GPIO Define */
#define  DA_SPIx                        SPI2
#define  DA_SPIx_RCC_CLK_ENABLE()       __HAL_RCC_SPI2_CLK_ENABLE()
#define  DA_SPIx_RCC_CLK_DISABLE()      __HAL_RCC_SPI2_CLK_DISABLE()

#define  DA_SPI_MISO_PIN                GPIO_PIN_2
#define  DA_SPI_MISO_GPIO_PORT               GPIOC
#define  DA_SPI_MOSI_PIN                GPIO_PIN_3
#define  DA_SPI_MOSI_GPIO_PORT               GPIOC
#define  DA_SPI_SCK_PIN                 GPIO_PIN_10
#define  DA_SPI_SCK_GPIO_PORT                GPIOB

#define  DA_ATT1_CS_PIN                 GPIO_PIN_8
#define  DA_ATT1_CS_GPIO_PORT                GPIOB
#define  DA_ATT2_CS_PIN                 GPIO_PIN_9
#define  DA_ATT2_CS_GPIO_PORT                GPIOB

#define  DA_ATT1_RESET_PIN              GPIO_PIN_0
#define  DA_ATT1_RESET_GPIO_PORT             GPIOE
#define  DA_ATT2_RESET_PIN              GPIO_PIN_1
#define  DA_ATT2_RESET_GPIO_PORT             GPIOE


#define  DA_ATT1_CS_DISABLE()           HAL_GPIO_WritePin(DA_ATT1_CS_GPIO_PORT, DA_ATT1_CS_PIN,GPIO_PIN_SET)
#define  DA_ATT1_CS_ENABLE()            HAL_GPIO_WritePin(DA_ATT1_CS_GPIO_PORT, DA_ATT1_CS_PIN,GPIO_PIN_RESET)
#define  DA_ATT2_CS_DISABLE()           HAL_GPIO_WritePin(DA_ATT2_CS_GPIO_PORT, DA_ATT2_CS_PIN,GPIO_PIN_SET)
#define  DA_ATT2_CS_ENABLE()            HAL_GPIO_WritePin(DA_ATT2_CS_GPIO_PORT, DA_ATT2_CS_PIN,GPIO_PIN_RESET)


#define  DA_ATT1_RESET_DISABLE()        HAL_GPIO_WritePin(DA_ATT1_RESET_GPIO_PORT, DA_ATT1_RESET_PIN,GPIO_PIN_SET)
#define  DA_ATT1_RESET_ENABLE()         HAL_GPIO_WritePin(DA_ATT1_RESET_GPIO_PORT, DA_ATT1_RESET_PIN,GPIO_PIN_RESET)
#define  DA_ATT2_RESET_DISABLE()        HAL_GPIO_WritePin(DA_ATT2_RESET_GPIO_PORT, DA_ATT2_RESET_PIN,GPIO_PIN_SET)
#define  DA_ATT2_RESET_ENABLE()         HAL_GPIO_WritePin(DA_ATT2_RESET_GPIO_PORT, DA_ATT2_RESET_PIN,GPIO_PIN_RESET)

/* XPT2046 GPIO Define */
#define  XPT2046_SPIx                   SPI3

#define  XPT2046_SPIx_RCC_CLK_ENABLE()  __HAL_RCC_SPI3_CLK_ENABLE()
#define  XPT2046_SPIx_RCC_CLK_DISABLE() __HAL_RCC_SPI3_CLK_DISABLE()

#define  XPT2046_SCK_PIN                GPIO_PIN_3
#define  XPT2046_SCK_GPIO_PORT               GPIOB	
#define  XPT2046_CS_PIN                 GPIO_PIN_5
#define  XPT2046_CS_GPIO_PORT                GPIOB
#define  XPT2046_MOSI_PIN               GPIO_PIN_12
#define  XPT2046_MOSI_GPIO_PORT              GPIOC
#define  XPT2046_BUSY_PIN               GPIO_PIN_6
#define  XPT2046_BUSY_GPIO_PORT              GPIOD
#define  XPT2046_MISO_PIN               GPIO_PIN_4
#define  XPT2046_MISO_GPIO_PORT              GPIOB
#define  XPT2046_IRQ_PIN                GPIO_PIN_15
#define  XPT2046_IRQ_GPIO_PORT               GPIOA

#define  XPT2046_CS_DISABLE()     HAL_GPIO_WritePin(XPT2046_CS_GPIO_PORT, XPT2046_CS_PIN,GPIO_PIN_SET)
#define  XPT2046_CS_ENABLE()      HAL_GPIO_WritePin(XPT2046_CS_GPIO_PORT, XPT2046_CS_PIN,GPIO_PIN_RESET)

#define  XPT2046_IRQ_Read()       HAL_GPIO_ReadPin(XPT2046_IRQ_GPIO_PORT, XPT2046_IRQ_PIN )

#define DUMMY_BYTE                  (0xFF)


/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void GPIO_Init(void);





#endif /* end of _BSP_GPIO_H */





