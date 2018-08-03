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
   float fUa;                /*!< A相相电压       */
	 float fUb;                /*!< B相相电压       */
	 float fUc;                /*!< C相相电压	      */
	 float fUab;               /*!< AB相线电压      */
   float fUbc;               /*!< BC相线电压      */
   float fUca;               /*!< CA相线电压      */	
	 float fIa;                /*!< A相电流         */
	 float fIb;                /*!< B相电流         */
	 float fIc;                /*!< C相电流          */
	 float fPFa;               /*!< A相功率因数      */
	 float fPFb;               /*!< B相功率因数      */
	 float fPFc;               /*!< C相功率因数      */
	 float fPFt;               /*!< 合相功率因数     */
	 float fPa;                /*!< A相有功功率      */
	 float fPb;                /*!< B相有功功率      */
	 float fPc;                /*!< C相有功功率      */
	 float fPt;                /*!< 合相有功功率     */
	 float fQa;                /*!< A相无功功率      */
	 float fQb;                /*!< B相无功功率      */
	 float fQc;                /*!< C相无功功率      */
	 float fQt;                /*!< 合相无功功率     */
	 uint32_t uiEPa;           /*!< A相有功电能      */
   uint32_t uiEPb;           /*!< B相有功电能      */
   uint32_t uiEPc;           /*!< C相有功电能      */
   uint32_t uiEPt;           /*!< 合相有功电能     */
   float fEQa;               /*!< A相无功电能      */
   float fEQb;               /*!< B相无功电能      */
   float fEQc;               /*!< C相无功电能      */
   float fEQt;               /*!< 合相无功电能     */
   float fF;                 /*!< 频率             */
   float fVTHDa;             /*!< A相电压谐波畸变率 */
   float fVTHDb;             /*!< B相电压谐波畸变率 */
   float fVTHDc;             /*!< C相电压谐波畸变率 */
   float fITHDa;             /*!< A相电流谐波畸变率 */
   float fITHDb;             /*!< B相电流谐波畸变率 */
   float fITHDc;	           /*!< C相电流谐波畸变率 */
};

/** 
  * @brief Analyzed busbar variable structure
  */
#pragma pack(1)
struct Ana_BusVariableTypeDef
{
	 float fMaxIa;             /*!< A相最大电流      */
	 float fMaxIb;             /*!< B相最大电流      */
	 float fMaxIc;             /*!< C相最大电流      */
	 float fReqIa;             /*!< A相电流需量      */
	 float fReqIb;             /*!< B相电流需量      */
	 float fReqIc;             /*!< C相电流需量      */
	 float fMaxReqIa;          /*!< A相最大电流需量  */
	 float fMaxReqIb;          /*!< B相最大电流需量  */
	 float fMaxReqIc;          /*!< C相最大电流需量  */
	 float fReqPa;             /*!< A相有功需量      */
	 float fReqPb;             /*!< B相有功需量      */
	 float fReqPc;             /*!< C相有功需量      */
	 float fReqPt;             /*!< 合相有功需量      */
   float fMaxReqPa;          /*!< A相最大有功需量  */
   float fMaxReqPb;          /*!< B相最大有功需量  */
   float fMaxReqPc;          /*!< C相最大有功需量  */
   float fMaxReqPt;	         /*!< 合相最大有功需量  */
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
	uint8_t  bQF:1;               /*!< 开关状态   */
	uint8_t     :3;               /*!< 预留       */
	uint8_t  bSPD:1;              /*!< 防雷状态   */
	uint8_t      :3;              /*!< 预留       */
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
	uint8_t  bBusMode:4;     /*!< 母线模式：1--S， 2--D， 4--H， 8--Y */
	uint8_t  bQF:1;          /*!< 支路开关监测功能 */
  uint8_t  bSPD:1;         /*!< 母线防雷监测功能 */
  uint8_t  temp:2;	
};


union un1
{
  struct BusStatus_TypeDef    BusStatus_st_a[2];            /*!< 两路母线状态    */
  uint8_t ucBusStatus[2];
};


/** 
  * @brief optimset structure 
  */
#pragma pack(1)
struct Optimset_TypeDef
{
	  uint8_t  ucComAddr;              /*!< 本机通信地址          */
	  uint16_t usComBaud;              /*!< 本机串口通信波特率     */
		uint8_t  ucDHCPEn;               /*!< 使能动态IP */
	  uint8_t  ucIPAddr_a[4];          /*!< 本机以太网通信地址     */
	  uint8_t  ucIPGateway_a[4];       /*!< 以太网通信网关     */
	  uint8_t  ucIPMask_a[4];          /*!< 以太网通信子网掩码 */
		uint8_t  usDstIPAddr_a[4];       /*!< 目标以太网通信地址     */
	  uint16_t usDstIPPort;            /*!< 目标以太网通信端口     */
	  union
		{
			struct Setup_TypeDef   Setup_st; /*!< 系统配置            */
      uint8_t ucSetup;		
		}Setup;		
	  uint16_t usRateI[2];             /*!< 母线额定电流          */
	  uint16_t usCTR[2];               /*!< 母线电流变比          */
	  uint8_t  ucBranchN_a[2];         /*!< 支路数                */
	  uint16_t usOVThr;                /*!< 过压阈值              */
	  uint16_t usUVThr;                /*!< 欠压阈值              */
    uint8_t  ucOCThrFst;             /*!< 一级过流阈值（百分比） */
    uint8_t  ucOCThrSec;             /*!< 二级过流阈值（百分比） */	
	  uint16_t usOPThr[2];             /*!< 功率上限              */
	  uint16_t usOFThr;                /*!< 频率上限              */
	  uint16_t usUFThr;                /*!< 频率下限              */
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

extern union un1                          X;                             /*!< 两路母线状态    */

extern struct Optimset_TypeDef            Optimset_st;                  /*!< 参数设置变量    */

extern uint8_t ucBranchState_a[MAXBranchNum ];          /*!< 支路状态 */                                         
extern uint8_t ucBranchSet_a[MAXBranchNum ];            /*!< 支路开关设置，最大144路，1bit表示1路 */

/* function declarations --------------------------------------------------------------------*/




#endif /* end of _SYS_VARIABLE_H */




