/**
  ******************************************************************************
  * @file    app_TPCal.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   Interface of source file app_TPCal.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_TPCAL_H
#define _APP_TPCAL_H

/* Includes ---------------------------------------------------------------------------------*/
#include "bsp_Fram.h"
#include "touch.h"
#include "SSD1963.h"
#include "sys_FramAlloc.h"

/* type definitions -------------------------------------------------------------------------*/
typedef struct TP_Calibration_TypeDef
{
   uint16_t CalFlag;     /*!< 校准标志 */
	 uint32_t CalData_XL;  /*!< X方向左 */
	 uint32_t CalData_XR;  /*!< X方向右 */
	 uint32_t CalData_YT;  /*!< Y方向上 */
	 uint32_t CalData_YB;  /*!< Y方向下 */	 
}TP_Calibration_TypeDef;

extern TP_Calibration_TypeDef   TP_Cal;
/* macro definitions ------------------------------------------------------------------------*/
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
uint8_t TPCal_Calibrate(void);

#endif /* end _APP_TPCAL_H */



