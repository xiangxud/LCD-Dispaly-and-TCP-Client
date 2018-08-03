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

struct Acq_BusVariableTypeDef           Acq_BusVar_st_a[2];     /*!< ��·ĸ�߲ɼ�����*/
struct Ana_BusVariableTypeDef           Ana_BusVar_st_a[2];      /*!< ��·ĸ�߷�������*/
union  un1   X;

struct Optimset_TypeDef                 Optimset_st;                  /*!< �������ñ���    */

uint8_t ucBranchState_a[MAXBranchNum];         /*!< ֧·״̬ */                                         
uint8_t ucBranchSet_a[MAXBranchNum];           /*!< ����֧·�������ã����120·��1bit��ʾ1· */

/* private macro definitions ------------------------------------------------------------------------*/	
/* private variables ---------------------------------------------------------------------------------*/
/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/











	/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/





