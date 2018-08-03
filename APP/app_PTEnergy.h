/**
  ******************************************************************************
  * @file    app_PTEnergy.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_PTEnergy.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_PTEnergy_H
#define _APP_PTEnergy_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_RTC.h"
#include "bsp_Fram.h"
#include "sys_FramAlloc.h"
#include "sys_Variable.h"

/* type definitions -------------------------------------------------------------------------*/
#pragma pack(1)
typedef struct PTEnergyTypeDef 
{
    uint16_t  Time;
	  uint32_t  EP[8];
}PTEnergyTypeDef;

/* macro definitions ------------------------------------------------------------------------*/	
#define  FMOffset_PTE              (6)  /* 历史记录在历史电能存储块中的偏移地址 */

/* Expanded Variables -----------------------------------------------------------------------*/
extern uint16_t usPTEBaseTime;           /*!< 时基 */
/* function declarations --------------------------------------------------------------------*/
void gPTE_BaseTime(void);
void gPTE_Init(void);
uint16_t gPTE_FTime(void);
uint8_t gPTE_GetIndex(void);
void gPTE_Save(void);
void gPTE_Query(uint16_t usTimeS, uint16_t usTimeE, PTEnergyTypeDef *pPTEnergy);


#endif /* end of _APP_PTEnergy_H */



