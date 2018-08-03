/**
  ******************************************************************************
  * @file    app_Optimset.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_Optimset.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_OPTIMSET_H
#define _APP_OPTIMSET_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/	
#define  SoftVersion     "V3.1.1.0717"
/* Expanded Variables -----------------------------------------------------------------------*/
extern uint16_t UsrPassword;
/* function declarations --------------------------------------------------------------------*/
void Opt_Save(void);
void Opt_PWSave(void);
void Opt_BranchSet_Save(uint8_t Set, uint8_t Num);
void Opt_Init(void);

#endif /* end of _APP_OPTIMSET_H */



