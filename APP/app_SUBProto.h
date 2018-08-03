/**
  ******************************************************************************
  * @file    app_SUBProto.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_SUBProto.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_SUBPROTO_H
#define _APP_SUBPROTO_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
enum BranchStateTypeDef
{
   eActiveON    =0x00,        /*!< 启用 正常 */
	 eActiveOFF   =0x01,        /*!< 启用 故障 */
	 eReactiveON  =0x02,        /*!< 未启用 闭合 */
	 eReativeOFF  =0x03,        /*!< 未启用 断开 */	 
};


/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
extern __IO uint8_t IOChangeFlag;
/* function declarations --------------------------------------------------------------------*/
void SUB_Transmit(void );
uint8_t SUB_Receive(uint8_t* pData, uint16_t ucLen );
void  SUB_BranchState(void);


#endif /* end of _APP_SUBPROTO_H */



