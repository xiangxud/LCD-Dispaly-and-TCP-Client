/**
  ******************************************************************************
  * @file    app_Alarm.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   handle alarm 
  * @endverbatim
  ******************************************************************************
	* @note  1.本地实时告警采用RTAlmRecordTypeDef格式,flag = GenerateFlag时红色字体显示
	*          flag = TerminateFlag时蓝色字体显示 
  *        2.本地历史告警采用PTAlmRecordTypeDef格式，通过flag查询告警是否结束，告警结
	*          束时显示结束时间  
	*        3.遥信实时告警一次性上传所有告警状态，包括正常态
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
static uint8_t LAlmState[164];     /*!< 所有告警变量的告警状态记录，用于判断当前告警
                                        状态是否为新的告警 */
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
	
	 /* 已初始化 */
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
		
		/* 当前有告警时不作处理，无告警时关闭告警灯和蜂鸣器 */
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
	  /* 电压 */
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUa, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUa + Num * 10));
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUb, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUb + Num * 10));
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fUc, Optimset_st.usOVThr, Optimset_st.usUVThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AUc + Num * 10));
	
	  /* 频率 */
	  AlmType_VoltFreq(Acq_BusVar_st_a[Num].fF, Optimset_st.usOFThr, Optimset_st.usUFThr,
	                   (enum AlmEventTypeDef)(eAlmEvent_AF+Num*10));
	
	  /* 电流 */
	  UpperL = Optimset_st.usRateI[Num] * Optimset_st.ucOCThrFst / 100;
	  UltUpperL = Optimset_st.usRateI[Num] * Optimset_st.ucOCThrSec / 100;
	
	  AlmType_Current(Acq_BusVar_st_a[Num].fIa, UpperL, UltUpperL, 
	                  (enum AlmEventTypeDef)(eAlmEvent_AIa + Num * 10));
	  AlmType_Current(Acq_BusVar_st_a[Num].fIb, UpperL, UltUpperL, 
	                  (enum AlmEventTypeDef)(eAlmEvent_AIb + Num * 10));
	  AlmType_Current(Acq_BusVar_st_a[Num].fIc, UpperL, UltUpperL, 
		                (enum AlmEventTypeDef)(eAlmEvent_AIc + Num * 10));
	
	  /* 功率 */
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
  *@note   D--只A路开关和防雷各自报警
  *        S--两路开关和防雷各自报警
  *        H--只A防雷，两路开关均断开时，A开关报警
  *        Y--两路防雷各自报警，两路开关均断开时，A开关报警
  */
static void Alm_BusState(void)
{	
	  /* 母线开关 */
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
		
		/* 母线防雷 */
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
	 
	 if(value >= UpperL)  //高于上限
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
	 else if(value  <= LowerL)  //低于下限
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
	 else    //正常
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
	 
	 if(value >= UltUpperL)  //二级过流
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
	 else if(value  >= UpperL)  //一级过流
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
	 else    //正常
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
	 
	 if(value  >= UpperL)  //高于上限
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
	 else    //正常
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
  *@note   0--正常， 1--告警
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
	 
	 if(state == eActiveOFF) //故障
	 {
			 /* 母线开关防雷 */
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
			 /* 支路开关 */
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
	 else if(state == eActiveON) //正常
	 {
			 /* 母线开关防雷 */
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
			 /* 支路开关 */
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
		 			 
		 /* 告警产生 */
		 if(CState != eAlmType_Normal)
		 {
				 Alm_Generate();
		 }
		 /* 告警结束 */
		 else
		 {
				 Alm_Terminate(AlmEvent);
		 }
		 
		 /* 更新实时告警记录索引值,大于预定义的条数后，去掉最旧记录，在最靠前的位置放入最新记录 */
		 RTAlmIndex++;		 
 
		 /* 告诉上位机有新的告警信息 */
		 ucPCProto_DataFlag |= 0x01;
		 
		 /* 更新上次告警状态为当前状态 */
		 *LState = CState;       				 
		 
		 return;
}



/**
  *@brief  handle alarm-generate
	*@param  NONE
	*@retval NONE
	*@note   1.本地实时告警--告警状态改变时，产生新的记录
	*        2.历史告警--告警产生时，产生新的告警记录，告警结束时更新告警结束时间，不产生新的告警记录
	*/
static void Alm_Generate(void)
{
     RTC_TimeTypeDef           sTime;
	   RTC_DateTypeDef           sDate;
		 
		 uint16_t  usFM_OperateAddr;

		 /* 获取当前告警记录的时间 */
		 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		 
		 
		 /* 历史告警记录 */   
		 gPTAlmRecord.flag = GenerateFlag;
		 gPTAlmRecord.GenerateYear = sDate.Year;
		 gPTAlmRecord.GenerateMonth = sDate.Month;
		 gPTAlmRecord.GenerateDate = sDate.Date;
		 
		 gPTAlmRecord.GenerateHour = sTime.Hours;
		 gPTAlmRecord.GenerateMiniute = sTime.Minutes;
		 gPTAlmRecord.GenerateSecond = sTime.Seconds;
		 
		 /* 获取当前告警记录的存储地址,保存当前告警记录 */
		 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + gPTAlmRelate.AlmIndex*sizeof(gPTAlmRecord);
		 
		 taskENTER_CRITICAL();
		 Fram_I2C_Write((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));
		 taskEXIT_CRITICAL();
	 
		 /* 更新历史告警记录索引值,并保存 */
		 gPTAlmRelate.AlmIndex++;
		 if(gPTAlmRelate.AlmIndex >= PTAlm_Item)
		 {
				gPTAlmRelate.FullFlag = 0x01;
				gPTAlmRelate.AlmIndex = 0;
		 }
		 
		 taskENTER_CRITICAL();
		 Fram_I2C_Write((uint8_t*)&gPTAlmRelate, FMAddr_PastAlm+ FMOffset_AlmRelate, sizeof(struct PTAlmRelatTypeDef));	
     taskEXIT_CRITICAL();		 
		 

		 
		 /* 实时告警记录 */
		 gRTAlmRecord[RTAlmIndex].flag = GenerateFlag;
		 gRTAlmRecord[RTAlmIndex].GenerateYear = sDate.Year;
		 gRTAlmRecord[RTAlmIndex].GenerateMonth = sDate.Month;
		 gRTAlmRecord[RTAlmIndex].GenerateDate = sDate.Date;
		 
		 gRTAlmRecord[RTAlmIndex].GenerateHour = sTime.Hours;
		 gRTAlmRecord[RTAlmIndex].GenerateMiniute = sTime.Minutes;
		 gRTAlmRecord[RTAlmIndex].GenerateSecond = sTime.Seconds;
		 
		 /* 有告警产生时，告警灯亮，蜂鸣器响 */
		 ALM_AURAL_ON();
		 ALM_VISUAL_ON();
		 ALM_LED_ON();
		 
		 return;
}



/**
  *@brief  handle alarm-terminate
	*@param  AlmEvent  当前告警事件
	*@retval NONE
	*@note   1.本地实时告警--告警状态改变时，产生新的记录
	*        2.历史告警--告警产生时，产生新的告警记录，告警结束时更新告警结束时间，不产生新的告警记录
	*/
static void Alm_Terminate(enum AlmEventTypeDef AlmEvent)
{
     RTC_TimeTypeDef           sTime;
	   RTC_DateTypeDef           sDate;
		 
		 uint16_t usFM_OperateAddr;
		 int16_t  i;
		 
		 /* 获取当前告警记录的时间 */
		 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);		 
		 
		 /* 没满500条 */
     if(gPTAlmRelate.FullFlag == 0x00)
		 {
		     for(i = gPTAlmRelate.AlmIndex-1; i >= 0; i-- )
				 {
						 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
						 
						 taskENTER_CRITICAL();
						 Fram_I2C_Read((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));		
						 taskEXIT_CRITICAL();
						 
						 /* 记录结束时间 */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {
							  		 /* z史佴薤輫录 */   
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
		 /* 满500条 */
		 else 
		 {		 
		     /* 先从0~PTAlmRelate.AlmIndex-1中查询该告警事件，从最新开始 */
		     for(i = gPTAlmRelate.AlmIndex-1; i >= 0; i-- )
				 {
						 usFM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + i * sizeof(gPTAlmRecord);
						 
						 taskENTER_CRITICAL();
						 Fram_I2C_Read((uint8_t *)&gPTAlmRecord, usFM_OperateAddr, sizeof(gPTAlmRecord));		
						 taskEXIT_CRITICAL();
						 
						 /* 记录结束时间 */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {
							   							  		 /* z史佴薤輫录 */   
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
						 
						 /* 记录结束时间 */
             if(gPTAlmRecord.AlmEvent == AlmEvent)		 
             {	 
							   /* z史佴薤輫录 */   
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
				 
		 /* 实时告警记录 */
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
	 /* 实时告警 */
   RTAlmIndex = 0;
	
	 /* 历史告警 */
   gPTAlmRelate.AlmIndex = 0;
   gPTAlmRelate.FullFlag = 0;
	
	 taskENTER_CRITICAL();
	 Fram_I2C_Write((uint8_t*)&gPTAlmRelate, FMAddr_PastAlm+ FMOffset_AlmRelate, sizeof(struct PTAlmRelatTypeDef));	
   taskEXIT_CRITICAL();	
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/



