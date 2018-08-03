/**
  ******************************************************************************
  * @file    bsp_RTC.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   RTC configuration 
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "bsp_RTC.h"
/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/	
/* private variables ----------------------------------------------------------*/
	 RTC_HandleTypeDef   hrtc;
/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
/* function body --------------------------------------------------------------*/

/**
  *@brief  RTC config
	*@param  NONE
	*@revtal NONE
	*@note
  */
void RTC_Init(void)
{
   RTC_TimeTypeDef    sTime;
	 RTC_DateTypeDef    sDate;
	 
	 HAL_PWR_EnableBkUpAccess();
	
	 hrtc.Instance = RTC;
	 hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	 hrtc.Init.AsynchPrediv = 127;
	 hrtc.Init.SynchPrediv = 255;
	 hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	 hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	 hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	 HAL_RTC_Init(&hrtc);
	
	 if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2)
	 {
	    sTime.Hours = 0x23;
		  sTime.Minutes = 0x59;
		  sTime.Seconds = 0x50;
			sTime.TimeFormat = RTC_HOURFORMAT12_AM;
		  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
		 
		  sDate.WeekDay = RTC_WEEKDAY_SATURDAY;
		  sDate.Month = RTC_MONTH_AUGUST;
      sDate.Date = 0x25;
      sDate.Year = 0x18;	
	    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
 
      HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x32F2);		 
	 }
}


void HAL_RTC_MspInit(RTC_HandleTypeDef  *rtcHandle)
{
   
   if(rtcHandle->Instance == RTC)
	 {
	    __HAL_RCC_RTC_ENABLE();
	 }
}


void HAl_RTC_MspDeInit(RTC_HandleTypeDef  *rtcHandle)
{
   if(rtcHandle->Instance == RTC)
	 {
	    __HAL_RCC_RTC_DISABLE();
			 HAL_PWR_DisableBkUpAccess();
      __HAL_RCC_PWR_CLK_DISABLE();
	 }
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/



