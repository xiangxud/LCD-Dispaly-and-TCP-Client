/**
  ******************************************************************************
  * @file    app_PTEnergy.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   历史电能按月存取，每记录一条显示或查询一条
  * @endverbatim 
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_PTEnergy.h"
#include "cmsis_os.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
#define  FMOffset_PTEInit          (0)  /* 初始化标志在历史电能存储块中的偏移地址 */     	
#define  FMOffset_BaseTime         (2)  /* 时基在历史电能存储块中的偏移地址 */
#define  FMOffset_SaveFlag         (4)  /* 保存标志在历史电能存储块中的偏移地址  */

/* private variables ----------------------------------------------------------*/
uint16_t usPTEBaseTime;           /*!< 时基 */
static uint8_t  ucPTESaveFlag;    /*!< 保存过标志 */

/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
static void nPTE_Assign(PTEnergyTypeDef *pPTEnergy);
static uint16_t  nPTE_Addr(const uint16_t usTime);

/* function body --------------------------------------------------------------*/

/**
  *@breif  past time energy save
  *@param  NONE
  *@retval NONE
  *@note  每月1日0时0分时保存一次
  */
void gPTE_Save(void)
{
   RTC_DateTypeDef    sDate;
	 RTC_TimeTypeDef    sTime;
	 PTEnergyTypeDef    PTEnergy;
	 
   uint16_t usFM_Addr = 0;
	
	 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	 if((sDate.Date == 1) && (sTime.Hours == 0) && (sTime.Minutes == 0) && (ucPTESaveFlag == 0x00))
	 {
      /* 以第一条记录前的时间为时基 */
      if(usPTEBaseTime == 0x00)
      {
			     if(sDate.Month == 1)
					 {
					     usPTEBaseTime = ((sDate.Year - 1) << 8) | 0x0C; 
					 }
					 else
					 {
   					   usPTEBaseTime = ((sDate.Year << 8) | (sDate.Month - 1));
					 }
					 
					 taskENTER_CRITICAL();	
           Fram_I2C_Write((uint8_t*)&usPTEBaseTime, FMAddr_EP_PastTime + FMOffset_BaseTime, sizeof(usPTEBaseTime));	
           taskEXIT_CRITICAL();					 
			}
			
			/* 获取当前时间点及电能值并保存 */
			PTEnergy.Time = gPTE_FTime();
			if(PTEnergy.Time > usPTEBaseTime)
			{
					nPTE_Assign(&PTEnergy);
					usFM_Addr = nPTE_Addr(PTEnergy.Time);

          taskENTER_CRITICAL();					
					Fram_I2C_Write((uint8_t *)&PTEnergy, usFM_Addr, sizeof( struct PTEnergyTypeDef));
					taskEXIT_CRITICAL();

					/* 已保存标志保存 */
					ucPTESaveFlag = 0x01;
					
					taskENTER_CRITICAL();
					Fram_I2C_Write(&ucPTESaveFlag, FMAddr_EP_PastTime + FMOffset_SaveFlag, sizeof(ucPTESaveFlag));
          taskEXIT_CRITICAL();					
      }			
	 }
	 if((sTime.Minutes != 0) && (ucPTESaveFlag == 0x01))
	 {
	    ucPTESaveFlag = 0x00;
			
			taskENTER_CRITICAL();
			Fram_I2C_Write(&ucPTESaveFlag, FMAddr_EP_PastTime + FMOffset_SaveFlag, sizeof(ucPTESaveFlag));
			taskEXIT_CRITICAL();
	 }
	 return;
}



/**
  *@brief  query any time energy
  *@param  usTimeS
  *        usTimeE
  *@retval 0--succeed, 1--fail
  *@note  共本地查询用
  */
void gPTE_Query(uint16_t usTimeS, uint16_t usTimeE, PTEnergyTypeDef *pPTEnergy)
{
    RTC_DateTypeDef  sDate;
		PTEnergyTypeDef  PTEnergy;
		
	  uint16_t i, usTemp, usFM_Addr ;
	
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	   usTemp = ((sDate.Year << 8) | sDate.Month);
	
	  /* 获取起始点的数据 */
	  if(usTimeS <= usPTEBaseTime)
		{
		    usTimeS = usPTEBaseTime;
        memset((uint8_t *)&PTEnergy, 0, sizeof(struct PTEnergyTypeDef));				
		}
		else
		{
		    usFM_Addr = nPTE_Addr(usTimeS);
				
				taskENTER_CRITICAL();
        Fram_I2C_Read((uint8_t *)&PTEnergy, usFM_Addr, sizeof(struct PTEnergyTypeDef));
        taskEXIT_CRITICAL();				
		}
		
		/*  获取终止点数据 */
		if(usTimeE > usTemp)
		{
		    usTimeE = usTemp;		 
		}
		
		if((usTimeE > usTimeS) && (usTimeE > usPTEBaseTime))
		{
		   	usFM_Addr = nPTE_Addr(usTimeE);
				
				taskENTER_CRITICAL();
        Fram_I2C_Read((uint8_t *)pPTEnergy, usFM_Addr, sizeof(struct PTEnergyTypeDef));
				taskEXIT_CRITICAL();
				
				for(i = 0; i < 8; i ++ )
				{
				   pPTEnergy->EP[i] -= PTEnergy.EP[i];
				}
		}
		else
		{
		    memset((uint8_t *)pPTEnergy, 0, sizeof(struct PTEnergyTypeDef));	
		}
		return;
}



/**
  *@brief 历史电能逐条实时显示
  *@param  page-当前页码
	*@retval NONE
	*@note NONE
  */

/**
  *@brief  PTE_TimeBase
  *@param  NONE  
  *@retval NONE
  *@note   电能清零时调用
  */
void gPTE_BaseTime(void)
{
   RTC_DateTypeDef  sDate;
	
	 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	 usPTEBaseTime = ((sDate.Year << 8) | sDate.Month);
	
	 taskENTER_CRITICAL();
	 Fram_I2C_Write((uint8_t*)&usPTEBaseTime, FMAddr_EP_PastTime + FMOffset_BaseTime, sizeof(usPTEBaseTime));
	 taskEXIT_CRITICAL();
	
	 
	 return;
}

/**
  *@brief   usPTE_BaseTime ,ucSavedFlag and ucPTEIndex init
  *@param   NONE
  *@retval  NONE
  *@note    时基按初始化时间初始化，标志和索引均为0,初始化标志为 0x55AA
	*         FMAddr_EP_PastTime--初始化标志
  *         FMAddr_EP_PastTime + 2 --时基
  *         FMAddr_EP_PastTime + 4 -- 已保存标志
  *         FMAddr_EP_PastTime + 5 -- 记录条数	
  */
void gPTE_Init(void)
{	
	 uint16_t usTemp;

   Fram_I2C_Read((uint8_t*)&usTemp, FMAddr_EP_PastTime + FMOffset_PTEInit, sizeof(usTemp));
   if(usTemp == 0x55AA)
	 {
	     Fram_I2C_Read((uint8_t *)&usPTEBaseTime, FMAddr_EP_PastTime + FMOffset_BaseTime, 
			               sizeof(usPTEBaseTime));
		   Fram_I2C_Read(&ucPTESaveFlag, FMAddr_EP_PastTime + FMOffset_SaveFlag, 
			               sizeof(ucPTESaveFlag));		 
	 }
   else
	 {
	     /* 初始化初始化标志 */
	     usTemp = 0x55AA;
		   Fram_I2C_Write((uint8_t*)&usTemp, FMAddr_EP_PastTime + FMOffset_PTEInit, sizeof(usTemp));
		  
			 /* 初始化时基 */
			 usPTEBaseTime = 0x00;				
			 Fram_I2C_Write((uint8_t*)&usPTEBaseTime, FMAddr_EP_PastTime + FMOffset_BaseTime, 
			                sizeof(usPTEBaseTime));
				
			 /* 初始化已保存标志 */
			 ucPTESaveFlag = 0x00;
       Fram_I2C_Write(&ucPTESaveFlag, FMAddr_EP_PastTime + FMOffset_SaveFlag, sizeof(ucPTESaveFlag)); 
			 
	 }	
   return;	 
}


/**
  *@brief   assign EP value to PTEnergy
  *@param   NONE
  *@retval  NONE
  *@note
  */
static void nPTE_Assign(PTEnergyTypeDef *pPTEnergy)
{
	 uint8_t  i;
	
   for(i = 0; i < 2; i ++)
	 {
	    pPTEnergy->EP[i*4+0] = Acq_BusVar_st_a[i].uiEPa;
		  pPTEnergy->EP[i*4+1] = Acq_BusVar_st_a[i].uiEPb;
		  pPTEnergy->EP[i*4+2] = Acq_BusVar_st_a[i].uiEPc;
		  pPTEnergy->EP[i*4+3] = Acq_BusVar_st_a[i].uiEPt;	  
	 }
	 return;
}

/**
  *@brief  get current time flag
  *@param  NONE
  *@retval time flag
  *@note
  */
uint16_t gPTE_FTime(void)
{
	 RTC_DateTypeDef  sDate;
	
	 uint16_t usTemp;
	
	 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	 usTemp = ((sDate.Year << 8) | sDate.Month);
	
	 return usTemp;
}

/**
  *@brief  get save address
  *@param  Time
  *@retval Address
  *@note
  */
static uint16_t  nPTE_Addr(const uint16_t usTime)
{
   uint16_t  usAddr,usOffset;

	 usOffset = ((((usTime >> 8) & 0x00FF)-((usPTEBaseTime >> 8) & 0x00FF)) * 12
							 +((usTime & 0x00FF) - (usPTEBaseTime & 0x00FF))-1) 
							 *sizeof(struct PTEnergyTypeDef);
	 usAddr = FMAddr_EP_PastTime + FMOffset_PTE + usOffset;
	 	 
	 return usAddr;
}


/**
  *@brief 获取历史记录条数
	*@param none
	*@retval 条数
	*@note NONE
	*/
uint8_t gPTE_GetIndex(void)
{
    uint8_t temp;
		
		temp=(((gPTE_FTime() >> 8) & 0x00FF)-((usPTEBaseTime >> 8) & 0x00FF)) * 12
							 +((gPTE_FTime() & 0x00FF) - (usPTEBaseTime & 0x00FF));
							 
		return temp;
}
/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************/







