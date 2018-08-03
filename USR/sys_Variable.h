/**
  ******************************************************************************
  * @file    sys_Variable.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    18-May-2018
  * @brief   this file gives all globle Variable interface of the project   
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _SYS_VARIABLE_H
#define _SYS_VARIABLE_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* type definitions -------------------------------------------------------------------------*/
/**
  * @}
  */

/** @addtogroup Global_variable_structures
  * @{
  */   

/** 
  * @brief Collected busbar variable structure  
  */
#pragma pack(1)
struct Acq_BusVariableTypeDef
{
   float fUa;                /*!< A�����ѹ       */
	 float fUb;                /*!< B�����ѹ       */
	 float fUc;                /*!< C�����ѹ	      */
	 float fUab;               /*!< AB���ߵ�ѹ      */
   float fUbc;               /*!< BC���ߵ�ѹ      */
   float fUca;               /*!< CA���ߵ�ѹ      */	
	 float fIa;                /*!< A�����         */
	 float fIb;                /*!< B�����         */
	 float fIc;                /*!< C�����          */
	 float fPFa;               /*!< A�๦������      */
	 float fPFb;               /*!< B�๦������      */
	 float fPFc;               /*!< C�๦������      */
	 float fPFt;               /*!< ���๦������     */
	 float fPa;                /*!< A���й�����      */
	 float fPb;                /*!< B���й�����      */
	 float fPc;                /*!< C���й�����      */
	 float fPt;                /*!< �����й�����     */
	 float fQa;                /*!< A���޹�����      */
	 float fQb;                /*!< B���޹�����      */
	 float fQc;                /*!< C���޹�����      */
	 float fQt;                /*!< �����޹�����     */
	 uint32_t uiEPa;           /*!< A���й�����      */
   uint32_t uiEPb;           /*!< B���й�����      */
   uint32_t uiEPc;           /*!< C���й�����      */
   uint32_t uiEPt;           /*!< �����й�����     */
   float fEQa;               /*!< A���޹�����      */
   float fEQb;               /*!< B���޹�����      */
   float fEQc;               /*!< C���޹�����      */
   float fEQt;               /*!< �����޹�����     */
   float fF;                 /*!< Ƶ��             */
   float fVTHDa;             /*!< A���ѹг�������� */
   float fVTHDb;             /*!< B���ѹг�������� */
   float fVTHDc;             /*!< C���ѹг�������� */
   float fITHDa;             /*!< A�����г�������� */
   float fITHDb;             /*!< B�����г�������� */
   float fITHDc;	           /*!< C�����г�������� */
};

/** 
  * @brief Analyzed busbar variable structure
  */
#pragma pack(1)
struct Ana_BusVariableTypeDef
{
	 float fMaxIa;             /*!< A��������      */
	 float fMaxIb;             /*!< B��������      */
	 float fMaxIc;             /*!< C��������      */
	 float fReqIa;             /*!< A���������      */
	 float fReqIb;             /*!< B���������      */
	 float fReqIc;             /*!< C���������      */
	 float fMaxReqIa;          /*!< A������������  */
	 float fMaxReqIb;          /*!< B������������  */
	 float fMaxReqIc;          /*!< C������������  */
	 float fReqPa;             /*!< A���й�����      */
	 float fReqPb;             /*!< B���й�����      */
	 float fReqPc;             /*!< C���й�����      */
	 float fReqPt;             /*!< �����й�����      */
   float fMaxReqPa;          /*!< A������й�����  */
   float fMaxReqPb;          /*!< B������й�����  */
   float fMaxReqPc;          /*!< C������й�����  */
   float fMaxReqPt;	         /*!< ��������й�����  */
};


/**
  * @}
  */

/** @addtogroup Status variable
  * @{
  */  

/** 
  * @brief Busbar status structure 
  */
#pragma pack(1)
struct BusStatus_TypeDef
{
	uint8_t  bQF:1;               /*!< ����״̬   */
	uint8_t     :3;               /*!< Ԥ��       */
	uint8_t  bSPD:1;              /*!< ����״̬   */
	uint8_t      :3;              /*!< Ԥ��       */
};
   
/**
  * @}
  */

/** @addtogroup optimset structures
  * @{
  */

/** 
  * @brief  Setup structure 
  */
#pragma pack(1)
struct Setup_TypeDef
{
	uint8_t  bBusMode:4;     /*!< ĸ��ģʽ��1--S�� 2--D�� 4--H�� 8--Y */
	uint8_t  bQF:1;          /*!< ֧·���ؼ�⹦�� */
  uint8_t  bSPD:1;         /*!< ĸ�߷��׼�⹦�� */
  uint8_t  temp:2;	
};


union un1
{
  struct BusStatus_TypeDef    BusStatus_st_a[2];            /*!< ��·ĸ��״̬    */
  uint8_t ucBusStatus[2];
};


/** 
  * @brief optimset structure 
  */
#pragma pack(1)
struct Optimset_TypeDef
{
	  uint8_t  ucComAddr;              /*!< ����ͨ�ŵ�ַ          */
	  uint16_t usComBaud;              /*!< ��������ͨ�Ų�����     */
		uint8_t  ucDHCPEn;               /*!< ʹ�ܶ�̬IP */
	  uint8_t  ucIPAddr_a[4];          /*!< ������̫��ͨ�ŵ�ַ     */
	  uint8_t  ucIPGateway_a[4];       /*!< ��̫��ͨ������     */
	  uint8_t  ucIPMask_a[4];          /*!< ��̫��ͨ���������� */
		uint8_t  usDstIPAddr_a[4];       /*!< Ŀ����̫��ͨ�ŵ�ַ     */
	  uint16_t usDstIPPort;            /*!< Ŀ����̫��ͨ�Ŷ˿�     */
	  union
		{
			struct Setup_TypeDef   Setup_st; /*!< ϵͳ����            */
      uint8_t ucSetup;		
		}Setup;		
	  uint16_t usRateI[2];             /*!< ĸ�߶����          */
	  uint16_t usCTR[2];               /*!< ĸ�ߵ������          */
	  uint8_t  ucBranchN_a[2];         /*!< ֧·��                */
	  uint16_t usOVThr;                /*!< ��ѹ��ֵ              */
	  uint16_t usUVThr;                /*!< Ƿѹ��ֵ              */
    uint8_t  ucOCThrFst;             /*!< һ��������ֵ���ٷֱȣ� */
    uint8_t  ucOCThrSec;             /*!< ����������ֵ���ٷֱȣ� */	
	  uint16_t usOPThr[2];             /*!< ��������              */
	  uint16_t usOFThr;                /*!< Ƶ������              */
	  uint16_t usUFThr;                /*!< Ƶ������              */
};

/* macro definitions ------------------------------------------------------------------------*/	
#define  MAXBranchNum         (144)
#define  BusModeS             (1)
#define  BusModeD             (2)
#define  BusModeH             (4)
#define  BusModeY             (8)

/* Expanded Variables -----------------------------------------------------------------------*/
extern struct Acq_BusVariableTypeDef      Acq_BusVar_st_a[2];
extern struct Ana_BusVariableTypeDef      Ana_BusVar_st_a[2];

extern union un1                          X;                             /*!< ��·ĸ��״̬    */

extern struct Optimset_TypeDef            Optimset_st;                  /*!< �������ñ���    */

extern uint8_t ucBranchState_a[MAXBranchNum ];          /*!< ֧·״̬ */                                         
extern uint8_t ucBranchSet_a[MAXBranchNum ];            /*!< ֧·�������ã����144·��1bit��ʾ1· */

/* function declarations --------------------------------------------------------------------*/




#endif /* end of _SYS_VARIABLE_H */




