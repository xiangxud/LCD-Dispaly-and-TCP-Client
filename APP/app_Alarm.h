/**
  ******************************************************************************
  * @file    app_Alarm.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   Interface of source file app_Alarm.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _APP_ALARM_H
#define _APP_ALARM_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys_Variable.h"

/* type definitions -------------------------------------------------------------------------*/

/**
  *@Alarm Event 
  */
enum AlmEventTypeDef
{
   eAlmEvent_AUa = 0x00,     /*!< A母线A相电压告警 */
	 eAlmEvent_AUb = 0x01,     /*!< A母线B相电压告警 */
   eAlmEvent_AUc = 0x02,     /*!< A母线C相电压告警 */
   eAlmEvent_AIa = 0x03,     /*!< A母线A相电流告警 */
   eAlmEvent_AIb = 0x04,     /*!< A母线B相电流告警 */
   eAlmEvent_AIc = 0x05,     /*!< A母线C相电流告警 */
   eAlmEvent_AP  = 0x06,     /*!< A母线功率告警    */
   eAlmEvent_AF  = 0x07,     /*!< A母线频率告警    */
   eAlmEvent_AQF = 0x08,     /*!< A母线开关告警    */
   eAlmEvent_ASPD = 0x09,    /*!< A母线防雷告警    */
	 eAlmEvent_BUa = 0x0A,     /*!< B母线A相电压告警 */
	 eAlmEvent_BUb = 0x0B,     /*!< B母线B相电压告警 */
   eAlmEvent_BUc = 0x0C,     /*!< B母线C相电压告警 */
   eAlmEvent_BIa = 0x0D,     /*!< B母线A相电流告警 */
   eAlmEvent_BIb = 0x0E,     /*!< B母线B相电流告警 */
   eAlmEvent_BIc = 0x0F,     /*!< B母线C相电流告警 */
   eAlmEvent_BP  = 0x10,     /*!< B母线功率告警    */
   eAlmEvent_BF  = 0x11,     /*!< B母线频率告警    */
   eAlmEvent_BQF = 0x12,     /*!< B母线开关告警    */
   eAlmEvent_BSPD = 0x13,    /*!< B母线防雷告警    */
   eAlmEvent_SW   =0x20,	   /*!< 支路开关告警，以支路数在此基础上偏移*/
};


/**
  *@Alarm type 
  */
enum  AlmTypeDef
{
   eAlmType_Normal  = 0x00,        /*!< 正常 */
	 eAlmType_Under   = 0x01,        /*!< 低于下限	*/
   eAlmType_IOTrip  = 0x01,        /*!< 开关故障  */
	 eAlmType_Over    = 0x02,        /*!< 高于上限或一级上限 */
	 eAlmType_OverSec = 0x03,        /*!< 高于二级上限 */
};

/**
  *@Alarm record
  */
#pragma pack(1)
typedef struct AlmRecordTypeDef
{
    uint8_t  GenerateYear;          /*!< 告警产生时间：年的个位和十位数， 即2018年时，该变量为18*/
	  uint8_t  GenerateMonth;                
	  uint8_t  GenerateDate;
	  uint8_t  GenerateHour;
	  uint8_t  GenerateMiniute;
	  uint8_t  GenerateSecond;
		uint8_t  TerminateYear;        /*!< 告警结束时间：年的个位和十位数， 即2018年时，该变量为18*/
	  uint8_t  TerminateMonth;                
	  uint8_t  TerminateDate;
	  uint8_t  TerminateHour;
	  uint8_t  TerminateMiniute;
	  uint8_t  TerminateSecond;
		uint8_t  flag;                  /*!< 0x00--告警产生标志，  0xff--告警结束标志 */
	  float    fCurrentValue;          /*!< 当前值 */
		uint16_t usThresholdValue;        /*!< 界限值 */
	  enum AlmEventTypeDef  AlmEvent;             /* 告警事件 */
	  enum AlmTypeDef  AlmType;              /* 告警内容 */
		
}AlmRecordTypeDef;

/**
  *@Past time alarm flag and index that need to save
  */
typedef struct PTAlmRelatTypeDef
{
   uint8_t FullFlag;    /*!< 满500条标志 */
	 uint16_t AlmIndex;    /*!< 告警条数索引 */
}PTAlmRelatTypeDef;


/**
  *@alarm message for PC
  */
#pragma pack(1)
typedef struct AlmMessageTypeDef
{
   uint8_t BusAlm[20];
	 uint8_t BranchAlm[MAXBranchNum];
}AlmMessageTypeDef;

/* macro definitions ------------------------------------------------------------------------*/	
#define RTAlm_Item      10
#define PTAlm_Item      500
#define GenerateFlag    0x55
#define TerminateFlag   0xFF
#define FMOffset_AlmRecord (6)

/* Expanded Variables -----------------------------------------------------------------------*/
extern AlmRecordTypeDef      gRTAlmRecord[RTAlm_Item];
extern AlmMessageTypeDef     gAlmMessage;
extern PTAlmRelatTypeDef     gPTAlmRelate;
extern __IO uint8_t RTAlmIndex;

/* function declarations --------------------------------------------------------------------*/
void AlmRelate_Init(void);
void Alm_AllHandler(void);
void Alm_RecordClear(void);

#endif /* end of _APP_ALARM_H */
