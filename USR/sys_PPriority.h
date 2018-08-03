/**
  ******************************************************************************
  * @file    sys_PPriority.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    26-May-2018
  * @brief   defines periph priority  in enum type
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_PPRIORITY_H
#define _SYS_PPRIORITY_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* type definitions -------------------------------------------------------------------------*/
enum PreiphPriority
{
	  PC_RxPriority    = 3,
	  PC_TxPriority    = 3,
	  SUB_RxPriority   = 4,
	  SUB_TxPriority   = 4,
	  USB_HostPriority = 5,
};
/* macro definitions ------------------------------------------------------------------------*/	
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/




#endif /* end of _SYS_PPRIORITY_H */



