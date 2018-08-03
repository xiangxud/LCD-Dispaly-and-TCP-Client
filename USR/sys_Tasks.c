/**
  ******************************************************************************
  * @file    sys_Tasks.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    28-May-2018
  * @brief   all user tasks init  
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "sys_Tasks.h"
#include "sys_Init.h"
#include "bsp_IWDG.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "fsmc.h"
#include "ssd1963.h"
#include "touch.h"
#include "app_ETH.h"
#include "app_TCPClient.h"
#include "app_SUBProto.h"
#include "app_PCProto.h"
#include "app_PTEnergy.h"
#include "sys_FramAlloc.h"
#include "sys_Variable.h"
#include "SEGGER_RTT.h"
#include "include.h"
#include "dialog.h"
#include "GUI.h"

/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/	
#define  FILE_NAME    "0:HRMC-G1K.bin"

/* private variables ---------------------------------------------------------------------------------*/
osThreadId Data_TaskHandle;
osThreadId IAP_TaskHandle;
osThreadId ETH_TaskHandle;
osThreadId PC_TaskHandle;
osThreadId HMI_TaskHandle;
osThreadId SUB_TaskHandle;
osThreadId OPT_TaskHandle;
osThreadId DBG_TaskHandle;


osMessageQId  PC_QueueHandle;
osMessageQId  SUB_QueueHandle;


WM_HWIN hPage[25];

/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
void Data_TaskFun(void const *pvParameters);
void PC_TaskFun(void const *pvParameters);
void HMI_TaskFun(void const *pvParameters);
void SUB_TaskFun(void const *pvParameters);
void OPT_TaskFun(void const *pvParameters);
void IAP_TaskFun(void const *pvParameters);
void ETH_TaskFun(void const *pvParameters);
void DBG_TaskFun(void const *pvParameters);
void GUI_MainTask(void);
/* function body -------------------------------------------------------------------------------------*/
void MX_FREERTOS_Init(void)
{
/* ���񴴽�-------------------------------------------------------------------------*/
	
	  /* vTask_Data�����弰����, ���ڴ�������*/
		osThreadDef(vTask_Data, Data_TaskFun, osPriorityBelowNormal, 0, 512);
	  Data_TaskHandle = osThreadCreate(osThread(vTask_Data), NULL);
	
	  /* vTask_PC�����弰������������λ���շ�����*/
		osThreadDef(vTask_PC,   PC_TaskFun,   osPriorityAboveNormal, 0, 512);
	  PC_TaskHandle = osThreadCreate(osThread(vTask_PC), NULL);
	
		/* vTask_HMI�����弰�����������˻���������*/
		osThreadDef(vTask_HMI,   HMI_TaskFun, osPriorityRealtime, 0, 512);
	  HMI_TaskHandle = osThreadCreate(osThread(vTask_HMI), NULL);
	
	  /* vTask_SUB�����弰������ ������λ���������ݴ��� */
		osThreadDef(vTask_SUB,  SUB_TaskFun,  osPriorityNormal, 0, 512);
	  SUB_TaskHandle = osThreadCreate(osThread(vTask_SUB), NULL);
	
	  /* vTask_ETH�����弰����, ������̫��ͨ�� */
		osThreadDef(vTask_ETH,  ETH_TaskFun,  osPriorityAboveNormal, 0, 512);
	  ETH_TaskHandle = osThreadCreate(osThread(vTask_ETH), NULL);
	
//	  /* vTask_DBG�����弰���������ڵ��� */
//		osThreadDef(vTask_DBG,  DBG_TaskFun,  osPriorityRealtime, 0, 512);
//	  DBG_TaskHandle = osThreadCreate(osThread(vTask_DBG), NULL);
		
/* �����ͨ�Ż��ƴ��� -----------------------------------------------------------------*/

    /* MessageQ_PC��Ϣ���ж��壬 ���ڴ��жϴ�����λ���������ݵ����� */
    osMessageQDef(MessageQ_PC, 10, sizeof(struct MessageType));
	  PC_QueueHandle = osMessageCreate(osMessageQ(MessageQ_PC), PC_QueueHandle);		
		
		/* MessageQ_SUB��Ϣ���ж��壬 ���ڴ��жϴ�����λ���������ݵ����� */
		osMessageQDef(MessageQ_SUB, 10, sizeof(struct MessageType));
	  SUB_QueueHandle = osMessageCreate(osMessageQ(MessageQ_SUB), SUB_QueueHandle);
}


void Data_TaskFun(void const *pvParameters)
{	
	  static uint8_t TimeCounter = 0;
	  uint32_t xLastWakeTime;
	
		/* ATT7022E��ʼ�� */
	  DA_ATT_Init();
					
	  xLastWakeTime = osKernelSysTick();
    while(1)
		{
			  /* ĸ�����ݲɼ� */
		   	Acq_BusVar();
			
			  /* ĸ��״̬��� */
		  	Acq_BusState();	

			  /* �澯���� */
			  Alm_AllHandler();
			
		    /* ʵʱ�����ۼƣ�2S����һ�� */
			  if((TimeCounter %10) == 0)
				{
						Acq_RTEnergy();
				}
        
				/* ��ʷ���� */
				gPTE_Save();
			  
			  /* �������㣺1min����һ�� */
				if(TimeCounter == 30)
				{
			     Acq_Req();
					 TimeCounter = 0;
				}
			  
        /* ��λ���������� */				
				SUB_Transmit();
				
				RUN_LED_Twinkle();
				TimeCounter++;						
			  osDelayUntil(&xLastWakeTime, 200);
		}	
}


void PC_TaskFun(void const *pvParameters)
{
	  osEvent  event;   
	  struct MessageType   *Msg;
	
    while(1)
		{
		     event = osMessageGet(PC_QueueHandle, osWaitForever);
			   if(event.status == osEventMessage )
				 {
					   Msg = event.value.p;
					 
					   if(gPCProtoHandle((uint8_t *)&(Msg->Rx_Buffer), Msg->Rx_Len) == 0)
						 {
						    PC_LED_Twinkle();
						 }							 
				 }
		}				
}

void HMI_TaskFun(void const *pvParameters)
{   
	 while(1)
	 {
	    GUI_MainTask();
	 }
}



void SUB_TaskFun(void const *pvParameters)
{
	  osEvent  event;   
	  struct MessageType   *Msg;
	
    while(1)
		{
		     event = osMessageGet(SUB_QueueHandle, osWaitForever);
			   if(event.status == osEventMessage )
				 {
					   Msg = event.value.p;
				     SUB_Receive((uint8_t *)&(Msg->Rx_Buffer), Msg->Rx_Len);						 
					   SUB_BranchState();
				 }
		}		
	
}

void ETH_TaskFun(void const *pvParameters)
{ 		
    uint8_t Cable_state = 0;  //��ʼ������û��
    uint32_t regvalue;
		
		low_level_init(&gnetif);		
	  HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);	
		
	  Cable_state = 1;
	  netif_set_link_up(&gnetif);
	 
	  /* Initialize the LwIP stack */
	  lwip_init();
	  Netif_Config();
	  User_notification(&gnetif);
	 
	 	if((regvalue & PHY_LINKED_STATUS) == PHY_LINKED_STATUS)
    {				
       /* connect to TCP sever */				
			 tcp_echoclient_connect();
		}		
				
    while(1)
		{			                                                                                              
				 HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);
				 
				 /* ���������� */		     
				 if((Cable_state == 0) && ((regvalue & PHY_LINKED_STATUS) == PHY_LINKED_STATUS))
			   {
				     Cable_state = 1;
						 
						 netif_set_link_up(&gnetif);
					 
						 /* ���ӷ����� */
						 tcp_echoclient_connect();
			   }
				 
				 /* ���߶Ͽ��� */
				 if((Cable_state == 1) && ((regvalue & PHY_LINKED_STATUS) != PHY_LINKED_STATUS))
				 {
				     Cable_state = 0;
						 
						 netif_set_link_down(&gnetif);
				 }				 
				 
				 /* Read a received packet from the Ethernet buffers and send it 
						to the lwIP for handling */
				 ethernetif_input(&gnetif);
				 
				 /* Handle timeouts */
				 sys_check_timeouts();
						 
				 if(Optimset_st.ucDHCPEn == 1)
				 {
						 #ifdef USE_DHCP
							/* handle periodic timers for LwIP */
							DHCP_Periodic_Handle(&gnetif);
						 #endif
				 }
		   osDelay(500);
		}		
	
}





//void DBG_TaskFun(void const *pvParameters)
//{
//	  uint8_t  pcWrightBuffer[500];
//	
//    while(1)
//		{
//		    SEGGER_RTT_printf(0, "========================================================\r\n");
//        SEGGER_RTT_printf(0, "������     ����״̬ ���ȼ� ʣ��ջ �������\r\n");
//			  vTaskList((char *)pcWrightBuffer);
//        SEGGER_RTT_printf(0,"%s\r\n", pcWrightBuffer);

//        SEGGER_RTT_printf(0, "\r\n������        ���м���  ʹ����\r\n");
//        vTaskGetRunTimeStats((char *)&pcWrightBuffer);	
//        SEGGER_RTT_printf(0,"%s\r\n", pcWrightBuffer);	

//        osDelay(1000);			
//		}		  
//}




void vApplicationIdleHook(void)
{
    /* ι�� */
    HAL_IWDG_Refresh(&hiwdg);
}



void GUI_MainTask(void)
{
		
			MX_FSMC_Init();
			Touch_Init_GPIO();
			LCD_Init();
			LCD_Clear(WHITE);
			__HAL_RCC_CRC_CLK_ENABLE();

     taskENTER_CRITICAL();
		 Fram_I2C_Read((uint8_t *)&TP_Cal, FMAddr_TP, sizeof(TP_Calibration_TypeDef));
		 Fram_I2C_Read((uint8_t *)&cal_value, FMAddr_TP+sizeof(TP_Calibration_TypeDef), sizeof(XPT2046_Calibration));
		 taskEXIT_CRITICAL();
		 
		 if(TP_Cal.CalFlag != 0x55AA)
		 {
		    /* �ȴ�У׼ */
		    while(XPT2046_Touch_Calibrate()==1)
				{
				  
				}
		 }
		 
    GUI_Init();			
    GUI_UC_SetEncodeUTF8();
		CHECKBOX_SetDefaultSkinClassic();
		WIDGET_SetDefaultEffect(&WIDGET_Effect_3D);	
		hPage[0]=CreateWindowHome(WM_HBKWIN);
		   	
//    hPage[4] = CreateWindowBusData_1A(WM_HBKWIN);		//��������,�����������汳��
//    hPage[5] = CreateWindowBusData_2A(WM_HBKWIN);		//��������,�����������汳��
//	  hPage[6] = CreateWindowBusData_1B(WM_HBKWIN);
//    hPage[7] = CreateWindowBusData_2B(WM_HBKWIN);
//	  hPage[8] = CreateWindowSystem_ConS(WM_HBKWIN);
//	  hPage[9] = CreateWindowDevice_Inform(WM_HBKWIN);
//		hPage[10] = CreateWindowNowAlarm(WM_HBKWIN);
//	  hPage[11] = CreateWindowHistoryAlarm(WM_HBKWIN);
//	  hPage[12] = CreateWindowKeypad(WM_HBKWIN);
//	  hPage[13] = CreateWindowNumberpad(WM_HBKWIN);
//	  hPage[14] = CreateWindowAHistory_Energy_1(WM_HBKWIN);
//	  hPage[15] = CreateWindowAHistory_Energy_2(WM_HBKWIN);
//    hPage[18] = CreateWindowSystem_ConD(WM_HBKWIN);	
//    hPage[19]=CreateWindowEnergy_Clear(WM_HBKWIN);	
//    hPage[20] = CreateWindowUserlogin_Clear(WM_HWIN hParent);
//		WM_HideWindow(hPage[12]);
//		WM_HideWindow(hPage[13]);
//    WM_SetFocus(hPage[0]);			
		
    while(1)
		{
		   GUI_Delay(300);
		}			
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/
	
	
	
	
	
	

	
	
	