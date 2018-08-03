/**
  ******************************************************************************
  * @file    app_PCProto.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_PCProto.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_PCPROTO_H
#define _APP_PCPROTO_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/	
#define  FramFrm_CID1        0x2C

/* Expanded Variables -----------------------------------------------------------------------*/
extern uint8_t  ucPCProto_DataFlag;

/* function declarations --------------------------------------------------------------------*/

uint8_t gPCProtoHandle(const uint8_t *pRxData, const uint16_t len);

#endif /* end of _APP_PCPROTO_H */



