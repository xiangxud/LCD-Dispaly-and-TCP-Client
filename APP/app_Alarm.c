/**
  ******************************************************************************
  * @file    app_Alarm.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   handle alarm 
  * @endverbatim
  ******************************************************************************
	* @note  1.����ʵʱ�澯����RTAlmRecordTypeDef��ʽ,flag = GenerateFlagʱ��ɫ������ʾ
	*          flag = TerminateFlagʱ��ɫ������ʾ 
  *        2.������ʷ�澯����PTAlmRecordTypeDef��ʽ��ͨ��flag��ѯ�澯�Ƿ�������澯��
	*          ��ʱ��ʾ����ʱ��  
	*        3.ң��ʵʱ�澯һ�����ϴ����и澯״̬����������̬
  *****************************************************************************/
	
/* Includes -------------------------------------------------------------------*/
#include "app_Alarm.h"
#include "bsp_GPIO.h"
#include "bsp_Fram.h"
#include "bsp_RTC.h"
#include "app_SUBProto.h"
#include "app_PCProto.h"
#include "sys_FramAlloc.h"
#include "sys_Variable.h"
#include "cmsis_os.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
#define  FMOffset_AlmInit          (0)
#define  FMOffset_AlmRelate        (2)	
/* private variables ----------------------------------------------------------*/
AlmRecordTypeDef      gRTAlmRecord[RTAlm_Item];
PTAlmRelatTypeDef     gPTAlmRelate;
AlmMessageTypeDef     gAlmMessage;

__IO uint8_t RTAlmIndex;
static uint8_t LAlmState[164];     /*!< ���и澯�����ĸ澯״̬��¼�������жϵ�ǰ�澯
                                        ״̬�Ƿ�Ϊ�µĸ澯 */
static AlmRecordTypeDef      gPTAlmRecord;

/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
static void AlmComp(const uint8_t CState, uint8_t *LState, enum AlmEventTypeDef AlmEvent);
static void  AlmType_VoltFreq( const float value, 
	                             const uint16_t UpperL,
															 const uint16_t LowerL, 
                               enum AlmEventTypeDef AlmEvent );
															 
static void  AlmType_Current(const float value, 
	                           const uint16_t UpperL,
                             const uint16_t UltUpperL,
                             enum AlmEventTypeDef AlmEvent);
														 
static void  AlmType_Power(  const float value, 
	                           const uint16_t UpperL,
                             enum AlmEventTypeDef AlmEvent);
														 
static void  AlmType_IO(  const uint8_t state, 
                          enum AlmEventTypeDef AlmEvent);	

static void Alm_BusbarVariable(const uint8_t Num);
static void Alm_BranchState(void);
static void Alm_BusState(void);
static void Alm_Generate(void);
static void Alm_Terminate(enum AlmEventTypeDef AlmEvent);
/* function body --------------------------------------------------------------*/

/**
  *@brief  Past time alarm related variable init
	*@param  *pAlmRelate
	*@retval NONE
	*@note
	*/
void AlmRelate_Init(void)
{
   uint16_t InitFlag;	 
	
	 Fram_I2C_Read((uint8_t*)&InitFlag, FMAddr_PastAlm + FMOffset_AlmInit, sizeof(InitFlag));
	
	 /* �ѳ�ʼ�� */
	 if(InitFlag == 0x55AA)
	 {
       Fram_I2C_Read((uint8_t *)&gPTAlmRelate, FMAddr_PastAlm + FMOffset_AlmRelate, sizeof(gPTAlmRelate));
	 }
	 else
	 {
	 
	 	   InitFlag = 0x55AA;
		   Fram_I2C_Write((uint8_t*)&InitFlag, FMAddr_PastAlm + FMOffset_AlmInit, sizeof(InitFlag));
			
       gPTAlmRelate.AlmIndex = 0;
			 gPTAlmRelate.FullFlag = 0;
			 Fram_I2C_Write((uint8_t *)&gPTAlmRelate, FMAddr_PastAlm + FMOffset_AlmRelate, sizeof(gPTAlmRelate));
	 } 

   return;	 
}


/**
  *@brief  alarm handle
  *@param  NONE
  *@retval NONE
  *@note
  */
void Alm_AllHandler(void)
{
    uint8_t i;
		
    Alm_BusState();
	
	  if((Optimset_st.Setup.Setup_st.bBusMode == BusModeD) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeH))
		{
		   Alm_BusbarVariable(0);
			 Alm_BranchState();
		}
		else
		{
		   Alm_BusbarVariable(0);
			 Alm_BusbarVariable(1);
			 Alm_BranchState();
		}
		
		/* ��ǰ�и澯ʱ���������޸澯ʱ�رո澯�ƺͷ����� */
		for(i = 0; i < 20; i++)
		{
		    if(gAlmMessage.BusAlm[i] != 0)
				{
				   return;
				}
		}
		
		for(i = 0; i < MAXBranchNum; i++)
		{
		    if(gAlmMessage.BranchAlm[i] != 0)
				{
				   return;
				}
		}
		
		ALM_AURAL_OFF();
		ALM_VISUAL_OFF();
		ALM_LED_OFF();
		return;
}


/**
  *@brief  busbar variable alarm
  *@param  Num--busbar number, 0--A, 1--B
  *@retval NONE
  *@note   
  */
static void Alm_BusbarVariable(const uint8_t Num)
{
	  uint16_t UpperL, UltUpperL;
	  /* ��ѹ */
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUa, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUa + Num * 10));
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUb, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUb + Num * 10));
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUc, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUc + Num * 10));
	
	  /* Ƶ�� */
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fF, Optimset_st.usOFThr, Optimset_st.usUFThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AF+Num*10));
	
	  /* ���� */
	  UpperL = Optimset_st.usRateI[Num] * Optimset_st.ucOCThrFst / 100;
	  UltUpperL = Optimset_st.usRateI[Num] * Optimset_st.ucOCThrSec / 100;
	
	  AlmType_Current(Acq_BusVar_st_a[Num].fIa, UpperL, UltUpperL, 
	                  (enum AlmEventTypeDef)(eAlmEvent_AIa + Num * 10));
	  AlmType_Current(Acq_BusVar_st_a[Num].fIb, UpperL, UltUpperL, 
	                  (enum AlmEventTypeDef)(eAlmEvent_AIb + Num * 10));
	  AlmType_Current(Acq_BusVar_st_a[Num].fIc, UpperL, UltUpperL, 
		                (enum AlmEventTypeDef)(eAlmEvent_AIc + Num * 10));
	
	  /* ���� */
	  AlmType_Power(Acq_BusVar_st_a[Num].fPt, Optimset_st.usOPThr[Num],
		              (enum AlmEventTypeDef)(eAlmEvent_AP + Num * 10));
									
		return;
}

/**
  *@brief  branch switch state alarm
  *@param  Num--busbar number, 0--A, 1--B
  *@retval NONE
  *@note   
  */
static void Alm_BranchState(void)
{
    uint8_t i;
	  
    if(Optimset_st.Setup.Setup_st.bQF == 1)
    {		
				for(i = 0; i < MAXBranchNum; i ++)
				{
						AlmType_IO(ucBranchState_a[i], (enum AlmEventTypeDef)(eAlmEvent_SW + i));
				}
		}
		else
		{
		   for(i = 0; i < MAXBranchNum; i ++)
				{
						AlmType_IO(0x20, (enum AlmEventTypeDef)(eAlmEvent_SW + i));
				}
		}
		
		return;
}


/**
  *@brief  busbar state alarm
  *@param  NoNE
  *@retval NONE
  *@note   D--ֻA·���غͷ��׸��Ա���
  *        S--��·���غͷ��׸��Ա���
  *        H--ֻA���ף���·���ؾ��Ͽ�ʱ��A���ر���
  *        Y--��·���׸��Ա�������·���ؾ��Ͽ�ʱ��A���ر���
  */
static void Alm_BusState(void)
{	
	  /* ĸ�߿��� */
    if(Optimset_st.Setup.Setup_st.bBusMode == BusModeS)
		{			  
		    AlmType_IO(X.BusStatus_st_a[0].bQF, eAlmEvent_AQF);			
				AlmType_IO(X.BusStatus_st_a[1].bQF, eAlmEvent_BQF); 			
		}
		else if(Optimset_st.Setup.Setup_st.bBusMode == BusModeD)
		{
		 		AlmType_IO(X.BusStatus_st_a[0].bQF, eAlmEvent_AQF);
        AlmType_IO(0x20, eAlmEvent_BQF);			
		}
		else if((Optimset_st.Setup.Setup_st.bBusMode == BusModeH) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
		{
		    if((X.BusStatus_st_a[0].bQF == 1) && (X.BusStatus_st_a[1].bQF) == 1)
				{
					 AlmType_IO(0x01, eAlmEvent_AQF);					
				}
				else
				{
				   AlmType_IO(0x00, eAlmEvent_AQF);		
				}
				AlmType_IO(0x20, eAlmEvent_BQF);
		}
		
		/* ĸ�߷��� */
		if(Optimset_st.Setup.Setup_st.bSPD == 1)
		{
		 		AlmType_IO(X.BusStatus_st_a[0].bSPD, eAlmEvent_ASPD);
			  
			  if((Optimset_st.Setup.Setup_st.bBusMode == BusModeS) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
				AlmType_IO(X.BusStatus_st_a[1].bSPD, eAlmEvent_BSPD);    
		}
		else
		{
		 		AlmType_IO(0x20, eAlmEvent_ASPD);
			  
			  if((Optimset_st.Setup.Setup_st.bBusMode == BusModeD) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeH))
				AlmType_IO(0x20, eAlmEvent_BSPD); 
		}
		
		return;
}


/**
  *@brief  busbar frequency and voltage alarm type judge
  *@param  value-- current value of frequency or voltage
  *        UpperL--upper limit value of frequency or voltage
  *        LowerL--lower limit value of frequency or voltage
  *        Almevent--Alarm event to judge
  *@retval NONE
  *@note 
  */
static void  AlmType_VoltFreq(
	                             const float value, 
	                             const uint16_t UpperL,
															 const uint16_t LowerL, 
                               enum AlmEventTypeDef AlmEvent
															)
{	
   uint8_t i;
	 
   if(value == 0x20)
	 {
	    gAlmMessage.BusAlm[AlmEvent] =  0x20; 
		  return;
	 }		
	 
	 if(value >= UpperL)  //��������
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Over)
			 {
			    if(RTAlmIndex >= RTAlm_Item)
				  {
						 for(i = 1; i < RTAlm_Item; i ++)
						 {
								gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
						 }
						 RTAlmIndex = RTAlm_Item-1;
				  }
					gAlmMessage.BusAlm[AlmEvent] =  eAlmType_Over;
					
					gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_Over;
					gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
	        gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL;
						
					gPTAlmRecord.AlmEvent = AlmEvent;
	        gPTAlmRecord.fCurrentValue = value;
					gPTAlmRecord.usThresholdValue = UpperL;
					
					AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);	
			 }
	 }
	 else if(value  <= LowerL)  //��������
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Under)
			 {
			    if(RTAlmIndex >= RTAlm_Item)
					{
							for(i = 1; i < RTAlm_Item; i ++)
							{
								gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
							}
							RTAlmIndex = RTAlm_Item-1;
					}
					gAlmMessage.BusAlm[AlmEvent] =  eAlmType_Under;
					
					gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_Under;
					gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
	        gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					gRTAlmRecord[RTAlmIndex].usThresholdValue = LowerL;
						
					gPTAlmRecord.AlmType = eAlmType_Under;
					gPTAlmRecord.AlmEvent = AlmEvent;
	        gPTAlmRecord.fCurrentValue = value;
					gPTAlmRecord.usThresholdValue = LowerL;
					
					AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);
			 }
	 }
	 else    //����
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Normal)
			 {
					if(RTAlmIndex >= RTAlm_Item)
						 {
								 for(i = 1; i < RTAlm_Item; i ++)
								 {
										gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
								 }
								 RTAlmIndex = RTAlm_Item-1;
						}
						gAlmMessage.BusAlm[AlmEvent] = eAlmType_Normal;
								
						gRTAlmRecord[RTAlmIndex].AlmType = LAlmState[AlmEvent];
						gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
						gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
						if(LAlmState[AlmEvent] ==  eAlmType_Over)
						{
						    gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL; 
						}
						else if(LAlmState[AlmEvent] ==  eAlmType_Under)
						{
						    gRTAlmRecord[RTAlmIndex].usThresholdValue = LowerL; 
						}
						
						AlmComp(eAlmType_Normal, &LAlmState[AlmEvent], AlmEvent);
		   }			 
	 } 
	 return;
}


/**
  *@brief  busbar current alarm type judge
  *@param  *value-- current current value of current
  *        *UpperL--upper limit value of current
  *        *UltUpperL--ultimate lower limit value of current
  *        Almevent--Alarm event to judge
  *@retval NONE
  *@note 
  */
static void  AlmType_Current(
	                           const float value, 
	                           const uint16_t UpperL,
                             const uint16_t UltUpperL,
                             enum AlmEventTypeDef AlmEvent
														)
{	
   uint8_t i;
		
   if(value == 0x20)
   {
	    gAlmMessage.BusAlm[AlmEvent] =  0x20;
		  return;
	 }
	 
	 if(value >= UltUpperL)  //��������
	 {
	     if(LAlmState[AlmEvent] != eAlmType_OverSec)
			 {
			    if(RTAlmIndex >= RTAlm_Item)
				  {
						 for(i = 1; i < RTAlm_Item; i ++)
						 {
								gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
						 }
						 RTAlmIndex = RTAlm_Item-1;
				  }
					gAlmMessage.BusAlm[AlmEvent] =  eAlmType_OverSec;
					
					gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_OverSec;
					gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
	        gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					gRTAlmRecord[RTAlmIndex].usThresholdValue = UltUpperL;
						
					gPTAlmRecord.AlmType = eAlmType_OverSec;
					gPTAlmRecord.AlmEvent = AlmEvent;
	        gPTAlmRecord.fCurrentValue = value;
					gPTAlmRecord.usThresholdValue = UltUpperL;
					
					AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);	
			 } 
	 }
	 else if(value  >= UpperL)  //һ������
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Over)
			 {
			    if(RTAlmIndex >= RTAlm_Item)
					 {
							 for(i = 1; i < RTAlm_Item; i ++)
							 {
									gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
							 }
							 RTAlmIndex = RTAlm_Item-1;
					 }
					gAlmMessage.BusAlm[AlmEvent] =  eAlmType_Over;
					
					gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_Over;
					gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
	        gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL;
					
          gPTAlmRecord.AlmType = eAlmType_Over;					
					gPTAlmRecord.AlmEvent = AlmEvent;
	        gPTAlmRecord.fCurrentValue = value;
					gPTAlmRecord.usThresholdValue = UpperL;
					
					AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);
			 }
	 }
	 else    //����
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Normal)
			 {
						if(RTAlmIndex >= RTAlm_Item)
						 {
								 for(i = 1; i < RTAlm_Item; i ++)
								 {
										gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
								 }
								 RTAlmIndex = RTAlm_Item-1;
						 }
						gAlmMessage.BusAlm[AlmEvent] =  eAlmType_Normal;
								
						gRTAlmRecord[RTAlmIndex].AlmType = LAlmState[AlmEvent];
						gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
						gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
						if(LAlmState[AlmEvent] ==  eAlmType_OverSec)
						{
						   gRTAlmRecord[RTAlmIndex].usThresholdValue = UltUpperL; 
						}
						else if(LAlmState[AlmEvent] ==  eAlmType_Over)
						{
						    gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL; 
						}
						
						AlmComp(eAlmType_Normal, &LAlmState[AlmEvent], AlmEvent);
		   }
	 }	 
	 return;
}


/**
  *@brief  busbar power alarm type judge
  *@param  *value-- current power value of power
  *        *UpperL--upper limit value of power
  *        Almevent--Alarm event to judge
  *@retval NONE
  *@note 
  */
static void  AlmType_Power(
	                           const float value, 
	                           const uint16_t UpperL,
                             enum AlmEventTypeDef AlmEvent
													)
{	
   uint8_t i;
	 
   if(value == 0x20)
	 {
	    gAlmMessage.BusAlm[AlmEvent] = 0x20;
		  return;
	 }
	 
	 if(value  >= UpperL)  //��������
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Over)
			 {
			     if(RTAlmIndex >= RTAlm_Item)
					 {
							 for(i = 1; i < RTAlm_Item; i ++)
							 {
									gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
							 }
							 RTAlmIndex = RTAlm_Item-1;
					 }
			     gAlmMessage.BusAlm[AlmEvent] = eAlmType_Over;
					 
					 gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_Over;
				   gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
					 gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					 gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL;

           gPTAlmRecord.AlmType = eAlmType_Over;
				   gPTAlmRecord.AlmEvent = AlmEvent;
					 gPTAlmRecord.fCurrentValue = value;
					 gPTAlmRecord.usThresholdValue = UpperL;

           AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);						 
			 }
		   	 
	 }
	 else    //����
	 {
	     if(LAlmState[AlmEvent] != eAlmType_Normal)
			 {
			     if(RTAlmIndex >= RTAlm_Item)
					 {
							 for(i = 1; i < RTAlm_Item; i ++)
							 {
									gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
							 }
							 RTAlmIndex = RTAlm_Item-1;
					 }
					 gAlmMessage.BusAlm[AlmEvent] = eAlmType_Normal;
							 
					 gRTAlmRecord[RTAlmIndex].AlmType = LAlmState[AlmEvent];
					 gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
					 gRTAlmRecord[RTAlmIndex].fCurrentValue = value;
					 gRTAlmRecord[RTAlmIndex].usThresholdValue = UpperL;

					 AlmComp(eAlmType_Normal, &LAlmState[AlmEvent], AlmEvent);
			 }
	 }	 
	 return;
}

/**
  *@brief  busbar state alarm type judge
  *@param  *state-- current power value of state
  *        Almevent--Alarm event to judge
  *@retval NONE
  *@note   0--������ 1--�澯
  */
static void  AlmType_IO(
	                        const uint8_t state, 
                          enum AlmEventTypeDef AlmEvent
												)
{
   uint8_t i;
	 
   if(state == 0x20)
	 {
	    gAlmMessage.BranchAlm[AlmEvent-eAlmEvent_SW] = 0x20;
      return;		 
	 }
	 
	 if(state == eActiveOFF) //����
	 {
			 /* ĸ�߿��ط��� */
			 if(AlmEvent <= eAlmEvent_BSPD)
			 {
					 if(LAlmState[AlmEvent] != eAlmType_IOTrip)
					 {
					     if(RTAlmIndex >= RTAlm_Item)
							 {
									 for(i = 1; i < RTAlm_Item; i ++)
									 {
											gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
									 }
									 RTAlmIndex = RTAlm_Item-1;
							 }
							 
					     if(RTAlmIndex == RTAlm_Item)
							 {
							    RTAlmIndex = RTAlm_Item-1;
							 }
							 gAlmMessage.BusAlm[AlmEvent] = eAlmType_IOTrip;
							 
							 gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_IOTrip;
							 gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
							 gRTAlmRecord[RTAlmIndex].fCurrentValue = state;
							 gRTAlmRecord[RTAlmIndex].usThresholdValue = eActiveOFF;
							 
							 gPTAlmRecord.AlmType = eAlmType_IOTrip;
							 gPTAlmRecord.AlmEvent = AlmEvent;
							 gPTAlmRecord.fCurrentValue = state;
							 gPTAlmRecord.usThresholdValue = eActiveOFF;
							 
							 AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[AlmEvent], AlmEvent);
					 }
			 }
			 /* ֧·���� */
			 else if(AlmEvent >= eAlmEvent_SW)
			 {		
					 if(LAlmState[eAlmEvent_BSPD+1+(AlmEvent-eAlmEvent_SW)] != eAlmType_IOTrip)
					 {
               if(RTAlmIndex >= RTAlm_Item)
							 {
									 for(i = 1; i < RTAlm_Item; i ++)
									 {
											gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
									 }
									 RTAlmIndex = RTAlm_Item-1;
							 }					 
							 gAlmMessage.BranchAlm[AlmEvent-eAlmEvent_SW] = eAlmType_IOTrip;
							 
							 gRTAlmRecord[RTAlmIndex].AlmType = eAlmType_IOTrip;
							 gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
							 gRTAlmRecord[RTAlmIndex].fCurrentValue = state;
							 gRTAlmRecord[RTAlmIndex].usThresholdValue = eActiveOFF;
							 
							 gPTAlmRecord.AlmType = eAlmType_IOTrip;
							 gPTAlmRecord.AlmEvent = AlmEvent;
							 gPTAlmRecord.fCurrentValue = state;
							 gPTAlmRecord.usThresholdValue = eActiveOFF;
							 
							 AlmComp(gRTAlmRecord[RTAlmIndex].AlmType, &LAlmState[eAlmEvent_BSPD+1+(AlmEvent-eAlmEvent_SW)], AlmEvent);
					 }
			 }					 
	 }
	 else if(state == eActiveON) //����
	 {
			 /* ĸ�߿��ط��� */
			 if(AlmEvent <= eAlmEvent_BSPD)
			 {
					 if(LAlmState[AlmEvent] != eAlmType_Normal)
					 {
					     if(RTAlmIndex >= RTAlm_Item)
							 {
									 for(i = 1; i < RTAlm_Item; i ++)
									 {
											gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
									 }
									 RTAlmIndex = RTAlm_Item-1;
							 }
							 gAlmMessage.BusAlm[AlmEvent] = eAlmType_Normal;
							 
							 gRTAlmRecord[RTAlmIndex].AlmType = LAlmState[AlmEvent];
							 gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
							 gRTAlmRecord[RTAlmIndex].fCurrentValue = state;
							 gRTAlmRecord[RTAlmIndex].usThresholdValue = eActiveOFF;
							
							 AlmComp(eAlmType_Normal, &LAlmState[AlmEvent], AlmEvent);
					 }
			 }
			 /* ֧·���� */
			 else if(AlmEvent >= eAlmEvent_SW)
			 {
					 if(LAlmState[eAlmEvent_BSPD+1+(AlmEvent-eAlmEvent_SW)] != eAlmType_Normal)
					 {
					     if(RTAlmIndex >= RTAlm_Item)
							 {
									 for(i = 1; i < RTAlm_Item; i ++)
									 {
											gRTAlmRecord[i-1] = gRTAlmRecord[i];							  
									 }
									 RTAlmIndex = RTAlm_Item-1;
							 }

							 gAlmMessage.BranchAlm[AlmEvent-eAlmEvent_SW] = eAlmType_Normal;
							 
							 gRTAlmRecord[RTAlmIndex].AlmType = LAlmState[eAlmEvent_BSPD+1+(AlmEvent-eAlmEvent_SW)];
							 gRTAlmRecord[RTAlmIndex].AlmEvent = AlmEvent;
							 gRTAlmRecord[RTAlmIndex].fCurrentValue = state;
							 gRTAlmRecord[RTAlmIndex].usThresholdValue = eActiveOFF;

							 AlmComp(eAlmType_Normal, &LAlmState[eAlmEvent_BSPD+1+(AlmEvent-eAlmEvent_SW)], AlmEvent);
					 }							 
			 }		 
	 }	 
	 return;
}


/**
  *@brief  compare the current alarm state with the last alarm state to confirm it's a new alarm
  *@param  CState--current alarm
  *        LState--last alarm
  *@retval NONE
  *@note   when the alarm is new,save it and index pluse 1,renew the LState value as NState
  */
static void AlmComp(const uint8_t CState, uint8_t *LState, enum AlmEventTypeDef AlmEvent)
{
		 			 
		 /* �澯���� */
		 if(CState != eAlmType_Normal)
		 {
				 Alm_Generate();
		 }
		 /* �澯���� */
		 else
		 {
				 Alm_Terminate(AlmEvent);
		 }
		 
		 /* ����ʵʱ�澯��¼����ֵ,����Ԥ�����������ȥ����ɼ�¼�����ǰ��λ�÷������¼�¼ */
		 RTAlmIndex++;		 
 
		 /* ������λ�����µĸ澯��Ϣ */
		 ucPCProto_DataFlag |= 0x01;
		 
		 /* �����ϴθ澯״̬Ϊ��ǰ״̬ */
		 *LState = CState;       				 
		 
		 return;
}



/**
  *@brief  handle alarm-generate
	*@param  NONE
	*@retval NONE
	*@note   1.����ʵʱ�澯--�澯״̬�ı�ʱ�������µļ�¼
	*        2.��ʷ�澯--�澯����ʱ�������µĸ澯��¼���澯����ʱ���¸澯����ʱ�䣬�������µĸ澯��¼
	*/
static void Alm_Generate(void)
{
     RTC_TimeTypeDef           sTime;
	   RTC_DateTypeDef           sDate;
		 
		 uint16_t  usFM_OperateAddr;

		 /* ��ȡ��ǰ�澯��¼��ʱ�� */
		 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		 
		 
		 /* ��ʷ�澯��¼ */   
		 gPTAlmRecord.flag = GenerateFlag;
		 gPTAlmRecord.GenerateYear = sDate.Year;
		 gPTAlmRecord.GenerateMonth = sDate.Month;
		 gPTAlmRecord.GenerateDate = sDate.Date;
		 
		 gPTAlmRecord.GenerateHour = sTime.Hours;
		 gPTAlmRecord.GenerateMiniute = sTime.Minutes;
		 gPTAlmRecord.GenerateSecond = sTime.Seconds;
		 
		 /* ��ȡ��ǰ�澯��¼�Ĵ洢��ַ,���浱ǰ�澯��¼ */
		 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + gPTAlmRelate.AlmIndex*sizeof(gPTAlmRecord);
		 
		 taskENTER_CRITICAL();
		 Fram_I2C_Write((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));
		 taskEXIT_CRITICAL();
	 
		 /* ������ʷ�澯��¼����ֵ,������ */
		 gPTAlmRelate.AlmIndex++;
		 if(gPTAlmRelate.AlmIndex >= PTAlm_Item)
		 {
				gPTAlmRelate.FullFlag = 0x01;
				gPTAlmRelate.AlmIndex = 0;
		 }
		 
		 taskENTER_CRITICAL();
		 Fram_I2C_Write((uint8_t*)&gPTAlmRelate, FMAddr_PastAlm+ FMOffset_AlmRelate, sizeof(struct PTAlmRelatTypeDef));	
     taskEXIT_CRITICAL();		 
		 

		 
		 /* ʵʱ�澯��¼ */
		 gRTAlmRecord[RTAlmIndex].flag = GenerateFlag;
		 gRTAlmRecord[RTAlmIndex].GenerateYear = sDate.Year;
		 gRTAlmRecord[RTAlmIndex].GenerateMonth = sDate.Month;
		 gRTAlmRecord[RTAlmIndex].GenerateDate = sDate.Date;
		 
		 gRTAlmRecord[RTAlmIndex].GenerateHour = sTime.Hours;
		 gRTAlmRecord[RTAlmIndex].GenerateMiniute = sTime.Minutes;
		 gRTAlmRecord[RTAlmIndex].GenerateSecond = sTime.Seconds;
		 
		 /* �и澯����ʱ���澯�������������� */
		 ALM_AURAL_ON();
		 ALM_VISUAL_ON();
		 ALM_LED_ON();
		 
		 return;
}



/**
  *@brief  handle alarm-terminate
	*@param  AlmEvent  ��ǰ�澯�¼�
	*@retval NONE
	*@note   1.����ʵʱ�澯--�澯״̬�ı�ʱ�������µļ�¼
	*        2.��ʷ�澯--�澯����ʱ�������µĸ澯��¼���澯����ʱ���¸澯����ʱ�䣬�������µĸ澯��¼
	*/
static void Alm_Terminate(enum AlmEventTypeDef AlmEvent)
{
     RTC_TimeTypeDef           sTime;
	   RTC_DateTypeDef           sDate;
		 
		 uint16_t usFM_OperateAddr;
		 int16_t  i;
		 
		 /* ��ȡ��ǰ�澯��¼��ʱ�� */
		 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);		 
		 
		 /* û��500�� */
     if(gPTAlmRelate.FullFlag == 0x00)
		 {
		     for(i = gPTAlmRelate.AlmIndex-1; i >= 0; i-- )
				 {
						 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
						 
						 taskENTER_CRITICAL();
						 Fram_I2C_Read((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));		
						 taskEXIT_CRITICAL();
						 
						 /* ��¼����ʱ�� */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {
							  		 /* zʷ٦ޯ݇¼ */   
								 gPTAlmRecord.flag = TerminateFlag;
								 gPTAlmRecord.TerminateYear = sDate.Year;
								 gPTAlmRecord.TerminateMonth = sDate.Month;
								 gPTAlmRecord.TerminateDate = sDate.Date;
								 
								 gPTAlmRecord.TerminateHour = sTime.Hours;
								 gPTAlmRecord.TerminateMiniute = sTime.Minutes;
								 gPTAlmRecord.TerminateSecond = sTime.Seconds;
							 
						     usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
								 
								 taskENTER_CRITICAL();
                 Fram_I2C_Write((uint8_t *)&gPTAlmRecord + 6, usFM_OperateAddr + FMOffset_AlmRecord, 7);	
                 taskEXIT_CRITICAL();
                 break;								 
						 }						 
				 }
		 }
		 /* ��500�� */
		 else 
		 {		 
		     /* �ȴ�0~PTAlmRelate.AlmIndex-1�в�ѯ�ø澯�¼��������¿�ʼ */
		     for(i = gPTAlmRelate.AlmIndex-1; i >= 0; i-- )
				 {
						 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
						 
						 taskENTER_CRITICAL();
						 Fram_I2C_Read((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));		
						 taskEXIT_CRITICAL();
						 
						 /* ��¼����ʱ�� */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {
							   							  		 /* zʷ٦ޯ݇¼ */   
								 gPTAlmRecord.flag = TerminateFlag;
								 gPTAlmRecord.TerminateYear = sDate.Year;
								 gPTAlmRecord.TerminateMonth = sDate.Month;
								 gPTAlmRecord.TerminateDate = sDate.Date;
								 
								 gPTAlmRecord.TerminateHour = sTime.Hours;
								 gPTAlmRecord.TerminateMiniute = sTime.Minutes;
								 gPTAlmRecord.TerminateSecond = sTime.Seconds;
						     usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
								 
								 taskENTER_CRITICAL();
                 Fram_I2C_Write((uint8_t *)&gPTAlmRecord + 6, usFM_OperateAddr + FMOffset_AlmRecord, 7);	
                 taskEXIT_CRITICAL();
                 goto Find;								 
						 }						 
				 }
				 
				 /*  */
				 for(i = 500; i >= gPTAlmRelate.AlmIndex; i-- )
				 {
						 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
						 
						 taskENTER_CRITICAL();
						 Fram_I2C_Read((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));		
						 taskEXIT_CRITICAL();
						 
						 /* ��¼����ʱ�� */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {	 
							   /* zʷ٦ޯ݇¼ */   
								 gPTAlmRecord.flag = TerminateFlag;
								 gPTAlmRecord.TerminateYear = sDate.Year;
								 gPTAlmRecord.TerminateMonth = sDate.Month;
								 gPTAlmRecord.TerminateDate = sDate.Date;
								 
								 gPTAlmRecord.TerminateHour = sTime.Hours;
								 gPTAlmRecord.TerminateMiniute = sTime.Minutes;
								 gPTAlmRecord.TerminateSecond = sTime.Seconds;
						     usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
								 
								 taskENTER_CRITICAL();
                 Fram_I2C_Write((uint8_t *)&gPTAlmRecord + 6, usFM_OperateAddr + FMOffset_AlmRecord, 7);	
                 taskEXIT_CRITICAL();		
                 break;									 
						 }						 
				 }				 
		 }
		 
		 Find:
				 
		 /* ʵʱ�澯��¼ */
		 gRTAlmRecord[RTAlmIndex].flag = TerminateFlag;
		 gRTAlmRecord[RTAlmIndex].TerminateYear = sDate.Year;
		 gRTAlmRecord[RTAlmIndex].TerminateMonth = sDate.Month;
		 gRTAlmRecord[RTAlmIndex].TerminateDate = sDate.Date;
		 
		 gRTAlmRecord[RTAlmIndex].TerminateHour = sTime.Hours;
		 gRTAlmRecord[RTAlmIndex].TerminateMiniute = sTime.Minutes;
		 gRTAlmRecord[RTAlmIndex].TerminateSecond = sTime.Seconds;
		 
		 return;
}



/**
  *@brief  clear past time alarm record
  *@param  NONE
  *@retval NONE
  *@note   NONE
  */
void Alm_RecordClear(void)
{
	 /* ʵʱ�澯 */
   RTAlmIndex = 0;
	
	 /* ��ʷ�澯 */
   gPTAlmRelate.AlmIndex = 0;
   gPTAlmRelate.FullFlag = 0;
	
	 taskENTER_CRITICAL();
	 Fram_I2C_Write((uint8_t*)&gPTAlmRelate, FMAddr_PastAlm+ FMOffset_AlmRelate, sizeof(struct PTAlmRelatTypeDef));	
   taskEXIT_CRITICAL();	
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/



