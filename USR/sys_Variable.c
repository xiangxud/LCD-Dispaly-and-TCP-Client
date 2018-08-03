/**
  ******************************************************************************
  * @file    sys_Variable.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    18-May-2018
  * @brief   this file defines all globle Variable of the project   
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "sys_Variable.h"

/* private type definitions -------------------------------------------------------------------------*/

struct Acq_BusVariableTypeDef           Acq_BusVar_st_a[2];     /*!< 两路母线采集变量*/
struct Ana_BusVariableTypeDef           Ana_BusVar_st_a[2];      /*!< 两路母线分析变量*/
union  un1   X;

struct Optimset_TypeDef                 Optimset_st;                  /*!< 参数设置变量    */

uint8_t ucBranchState_a[MAXBranchNum];         /*!< 支路状态 */                                         
uint8_t ucBranchSet_a[MAXBranchNum];           /*!< 单相支路开关设置，最大120路，1bit表示1路 */

/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/











	/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/





