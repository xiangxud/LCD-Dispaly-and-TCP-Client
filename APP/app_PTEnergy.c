/**
  ******************************************************************************
  * @file    app_PTEnergy.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   ��ʷ���ܰ��´�ȡ��ÿ��¼һ����ʾ���ѯһ��
  * @endverbatim 
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_PTEnergy.h"
#include "cmsis_os.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
#define  FMOffset_PTEInit          (0)  /* ��ʼ����־����ʷ���ܴ洢���е�ƫ�Ƶ�ַ */     	
#define  FMOffset_BaseTime         (2)  /* ʱ������ʷ���ܴ洢���е�ƫ�Ƶ�ַ */
#define  FMOffset_SaveFlag         (4)  /* �����־����ʷ���ܴ洢���е�ƫ�Ƶ�ַ  */

/* private variables ----------------------------------------------------------*/
uint16_t usPTEBaseTime;           /*!< ʱ�� */
static uint8_t  ucPTESaveFlag;    /*!< �������־ */

/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
static void nPTE_Assign(PTEnergyTypeDef *pPTEnergy);
static uint16_t  nPTE_Addr(const uint16_t usTime);

/* function body --------------------------------------------------------------*/

/**
  *@breif  past time energy save
  *@param  NONE
  *@retval NONE
  *@note  ÿ��1��0ʱ0��ʱ����һ��
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
      /* �Ե�һ����¼ǰ��ʱ��Ϊʱ�� */
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
			
			/* ��ȡ��ǰʱ��㼰����ֵ������ */
			PTEnergy.Time = gPTE_FTime();
			if(PTEnergy.Time > usPTEBaseTime)
			{
					nPTE_Assign(&PTEnergy);
					usFM_Addr = nPTE_Addr(PTEnergy.Time);

          taskENTER_CRITICAL();					
					Fram_I2C_Write((uint8_t *)&PTEnergy, usFM_Addr, sizeof( struct PTEnergyTypeDef));
					taskEXIT_CRITICAL();

					/* �ѱ����־���� */
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
  *@note  �����ز�ѯ��
  */
void gPTE_Query(uint16_t usTimeS, uint16_t usTimeE, PTEnergyTypeDef *pPTEnergy)
{
    RTC_DateTypeDef  sDate;
		PTEnergyTypeDef  PTEnergy;
		
	  uint16_t i, usTemp, usFM_Addr ;
	
		 HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	   usTemp = ((sDate.Year << 8) | sDate.Month);
	
	  /* ��ȡ��ʼ������� */
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
		
		/*  ��ȡ��ֹ������ */
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
  *@brief ��ʷ��������ʵʱ��ʾ
  *@param  page-��ǰҳ��
	*@retval NONE
	*@note NONE
  */

/**
  *@brief  PTE_TimeBase
  *@param  NONE  
  *@retval NONE
  *@note   ��������ʱ����
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
  *@note    ʱ������ʼ��ʱ���ʼ������־��������Ϊ0,��ʼ����־Ϊ 0x55AA
	*         FMAddr_EP_PastTime--��ʼ����־
  *         FMAddr_EP_PastTime + 2 --ʱ��
  *         FMAddr_EP_PastTime + 4 -- �ѱ����־
  *         FMAddr_EP_PastTime + 5 -- ��¼����	
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
	     /* ��ʼ����ʼ����־ */
	     usTemp = 0x55AA;
		   Fram_I2C_Write((uint8_t*)&usTemp, FMAddr_EP_PastTime + FMOffset_PTEInit, sizeof(usTemp));
		  
			 /* ��ʼ��ʱ�� */
			 usPTEBaseTime = 0x00;				
			 Fram_I2C_Write((uint8_t*)&usPTEBaseTime, FMAddr_EP_PastTime + FMOffset_BaseTime, 
			                sizeof(usPTEBaseTime));
				
			 /* ��ʼ���ѱ����־ */
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
  *@brief ��ȡ��ʷ��¼����
	*@param none
	*@retval ����
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







