
#include "DIALOG.h"
#include "bsp_RTC.h"
#include "bsp_GPIO.h"
#include "app_Optimset.h"
#include "app_SUBProto.h"
#include "string.h"
#include "sys_Variable.h"
#include "stdlib.h"
#include "include.h"


#define ID_WINDOW_0    (GUI_ID_USER + 0x01)
#define ID_IMAGE_0     (GUI_ID_USER + 0x02)
#define ID_TEXT_0      (GUI_ID_USER + 0x03)
#define ID_TEXT_1      (GUI_ID_USER + 0x04)
#define ID_BUTTON_0    (GUI_ID_USER + 0x05)
#define ID_BUTTON_1    (GUI_ID_USER + 0x06)

extern WM_HWIN hPage[25];
extern GUI_CONST_STORAGE GUI_BITMAP bmDPJ_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsongti15;
static const GUI_WIDGET_CREATE_INFO _Help[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Bus_Data", ID_IMAGE_0, 1, 236, 67, 23, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_0, 428,259, 45, 13, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_1, 360, 259, 55, 13, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "Back", ID_BUTTON_0, 399, 226, 67, 23, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "IAP", ID_BUTTON_1, 330, 226, 67, 23, 0, 0x0, 0 },
};


uint8_t Window_Help_call;
static void _cbWindow_Help(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_Fontsongti15);
    BUTTON_SetText(hItem,"返回");
	  BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
  	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_Fontsongti15);
    BUTTON_SetText(hItem,"固件更新");
	  BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
    break;
	case WM_PAINT:
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_FillRect(0,0,480,20);
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_FillRect(0,260,480,272);
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Fontsongti15);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		if(Optimset_st.Setup.Setup_st.bBusMode==2) 
			GUI_DispStringAt("交流配电监测单元DPJ-G1K-D",100,3);
		if(Optimset_st.Setup.Setup_st.bBusMode==1) 
			GUI_DispStringAt("交流配电监测单元DPJ-G1K-S",100,3);
		if(Optimset_st.Setup.Setup_st.bBusMode==8) 
			GUI_DispStringAt("交流配电监测单元DPJ-G1K-Y",100,3);
		if(Optimset_st.Setup.Setup_st.bBusMode==4) 
			GUI_DispStringAt("交流配电监测单元DPJ-G1K-H",100,3);
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Fontsongti15);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DispStringAt("帮助",430,3);
	  GUI_DrawBitmap(&bmDPJ_1, 40, 70);
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font13_1);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DispStringAt(SoftVersion,5,4);
		GUI_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_Fontsongti15);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DispStringAt("扫描二维码, 获取本系统操作",210,100);
		GUI_DispStringAt("说明书及通信协议等相关资料",210,120);
		GUI_DispStringAt("联系电话: 400-0931-411",210,160);
		GUI_DispStringAt("网址: http://doc.hhte.com.cn/",210,180);
	  break;
	case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
	  switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:			
						if(Id==ID_BUTTON_0) // Notifications sent by 'Bus_Data'
						{
							 if(Window_Help_call==0x01)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[4] = CreateWindowBusData_1(WM_HBKWIN);
								  WM_ShowWindow(hPage[4]);
							 }
							 else if(Window_Help_call==0x02)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[5] = CreateWindowBusData_2(WM_HBKWIN);
								  WM_ShowWindow(hPage[5]);
							 }
							 else if(Window_Help_call==0x05)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[14] = CreateWindowAHistory_Energy_1(WM_HBKWIN);
								  WM_ShowWindow(hPage[14]);
							 }
							 else if(Window_Help_call==0x06)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[15] = CreateWindowAHistory_Energy_2(WM_HBKWIN);
								  WM_ShowWindow(hPage[15]);
							 }
							 else if(Window_Help_call==0x09)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[10] = CreateWindowNowAlarm(WM_HBKWIN);
								  WM_ShowWindow(hPage[10]);
							 }
							 else if(Window_Help_call==0x10)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[11] = CreateWindowHistoryAlarm(WM_HBKWIN);
								  WM_ShowWindow(hPage[11]);
							 }
							 else if(Window_Help_call==0x11)
							 { 
								  WM_DeleteWindow(hPage[20]);
								  hPage[18] = CreateWindowSystem_ConD(WM_HBKWIN);	
								  WM_ShowWindow(hPage[18]);
							 }
							 else if(Window_Help_call==0x12)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[9] = CreateWindowDevice_Inform(WM_HBKWIN);
								  WM_ShowWindow(hPage[9]);
							 }
							 else if(Window_Help_call==0x13)
							 {
								  WM_DeleteWindow(hPage[20]);
								  hPage[8] = CreateWindowSystem_ConS(WM_HBKWIN);	
								  WM_ShowWindow(hPage[8]);
							 }	 
						}
						if(Id==ID_BUTTON_1)
						{
							if(User_login==0xCC)
							{
								  /* 关总中断并复位 */
									 __set_PRIMASK(1);
                   __disable_irq();          // 关闭所有中断								
									 NVIC_SystemReset();  
							}
						}
						break;
					}
		break;
	case WM_TIMER:
			Caculate_RTC(pMsg,ID_TEXT_1,ID_TEXT_0);
    	WM_RestartTimer(pMsg->Data.v,100);
		break;
	default:
			WM_DefaultProc(pMsg);
		break;
  }
 }



WM_HWIN CreateWindow_Help(WM_HWIN hParent);
WM_HWIN CreateWindow_Help(WM_HWIN hParent)
{ 
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_Help, GUI_COUNTOF(_Help), _cbWindow_Help, WM_HBKWIN, 0, 0);
	WM_CreateTimer(WM_GetClientWindow(hWin),0,200,0);
	
	return hWin;
}










