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
   eAlmEvent_AUa = 0x00,     /*!< Aĸ��A���ѹ�澯 */
	 eAlmEvent_AUb = 0x01,     /*!< Aĸ��B���ѹ�澯 */
   eAlmEvent_AUc = 0x02,     /*!< Aĸ��C���ѹ�澯 */
   eAlmEvent_AIa = 0x03,     /*!< Aĸ��A������澯 */
   eAlmEvent_AIb = 0x04,     /*!< Aĸ��B������澯 */
   eAlmEvent_AIc = 0x05,     /*!< Aĸ��C������澯 */
   eAlmEvent_AP  = 0x06,     /*!< Aĸ�߹��ʸ澯    */
   eAlmEvent_AF  = 0x07,     /*!< Aĸ��Ƶ�ʸ澯    */
   eAlmEvent_AQF = 0x08,     /*!< Aĸ�߿��ظ澯    */
   eAlmEvent_ASPD = 0x09,    /*!< Aĸ�߷��׸澯    */
	 eAlmEvent_BUa = 0x0A,     /*!< Bĸ��A���ѹ�澯 */
	 eAlmEvent_BUb = 0x0B,     /*!< Bĸ��B���ѹ�澯 */
   eAlmEvent_BUc = 0x0C,     /*!< Bĸ��C���ѹ�澯 */
   eAlmEvent_BIa = 0x0D,     /*!< Bĸ��A������澯 */
   eAlmEvent_BIb = 0x0E,     /*!< Bĸ��B������澯 */
   eAlmEvent_BIc = 0x0F,     /*!< Bĸ��C������澯 */
   eAlmEvent_BP  = 0x10,     /*!< Bĸ�߹��ʸ澯    */
   eAlmEvent_BF  = 0x11,     /*!< Bĸ��Ƶ�ʸ澯    */
   eAlmEvent_BQF = 0x12,     /*!< Bĸ�߿��ظ澯    */
   eAlmEvent_BSPD = 0x13,    /*!< Bĸ�߷��׸澯    */
   eAlmEvent_SW   =0x20,	   /*!< ֧·���ظ澯����֧·���ڴ˻�����ƫ��*/
};


/**
  *@Alarm type 
  */
enum  AlmTypeDef
{
   eAlmType_Normal  = 0x00,        /*!< ���� */
	 eAlmType_Under   = 0x01,        /*!< ��������	*/
   eAlmType_IOTrip  = 0x01,        /*!< ���ع���  */
	 eAlmType_Over    = 0x02,        /*!< �������޻�һ������ */
	 eAlmType_OverSec = 0x03,        /*!< ���ڶ������� */
};

/**
  *@Alarm record
  */
#pragma pack(1)
typedef struct AlmRecordTypeDef
{
    uint8_t  GenerateYear;          /*!< �澯����ʱ�䣺��ĸ�λ��ʮλ���� ��2018��ʱ���ñ���Ϊ18*/
	  uint8_t  GenerateMonth;                
	  uint8_t  GenerateDate;
	  uint8_t  GenerateHour;
	  uint8_t  GenerateMiniute;
	  uint8_t  GenerateSecond;
		uint8_t  TerminateYear;        /*!< �澯����ʱ�䣺��ĸ�λ��ʮλ���� ��2018��ʱ���ñ���Ϊ18*/
	  uint8_t  TerminateMonth;                
	  uint8_t  TerminateDate;
	  uint8_t  TerminateHour;
	  uint8_t  TerminateMiniute;
	  uint8_t  TerminateSecond;
		uint8_t  flag;                  /*!< 0x00--�澯������־��  0xff--�澯������־ */
	  float    fCurrentValue;          /*!< ��ǰֵ */
		uint16_t usThresholdValue;        /*!< ����ֵ */
	  enum AlmEventTypeDef  AlmEvent;             /* �澯�¼� */
	  enum AlmTypeDef  AlmType;              /* �澯���� */
		
}AlmRecordTypeDef;

/**
  *@Past time alarm flag and index that need to save
  */
typedef struct PTAlmRelatTypeDef
{
   uint8_t FullFlag;    /*!< ��500����־ */
	 uint16_t AlmIndex;    /*!< �澯�������� */
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
