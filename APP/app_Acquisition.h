/**
  ******************************************************************************
  * @file    app_Acquisition.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   Interface of source file app_Acquisition.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_ACQUISITION_H
#define _APP_ACQUISITION_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
/* macro definitions ------------------------------------------------------------------------*/	
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
void Acq_BusVar(void);
void Acq_BusState(void);
void Acq_EnergyInit(void);
void Acq_RTEnergy(void);
void Acq_EnergyClear(void);
void Acq_Req(void);
void Acq_MAXClear(void);

#endif /* end of _APP_ACQUISITION_H */
