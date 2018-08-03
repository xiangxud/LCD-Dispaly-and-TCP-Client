/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.30                          *
*        Compiled Jul  1 2015, 10:50:32                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "include.h"
#include "app_Acquisition.h"
#include "app_Optimset.h"
#include "sys_Variable.h"
#include <string.h>
#include <stdlib.h>
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_2    (GUI_ID_USER + 0x03)
#define ID_TEXT_3    (GUI_ID_USER + 0x04)

#define ID_TEXT_4    (GUI_ID_USER + 0x06)
#define ID_TEXT_5    (GUI_ID_USER + 0x07)
#define ID_EDIT_0    (GUI_ID_USER + 0x08)
#define ID_EDIT_1    (GUI_ID_USER + 0x09)
#define ID_EDIT_2    (GUI_ID_USER + 0x0B)
#define ID_EDIT_3    (GUI_ID_USER + 0x0A)
#define ID_EDIT_4    (GUI_ID_USER + 0x0C)
#define ID_EDIT_5    (GUI_ID_USER + 0x0D)
#define ID_TEXT_6    (GUI_ID_USER + 0x0E)

#define ID_TEXT_7    (GUI_ID_USER + 0x10)
#define ID_TEXT_8    (GUI_ID_USER + 0x11)
#define ID_BUTTON_0    (GUI_ID_USER + 0x12)
#define ID_BUTTON_1    (GUI_ID_USER + 0x13)
#define ID_BUTTON_2    (GUI_ID_USER + 0x14)
#define ID_BUTTON_3    (GUI_ID_USER + 0x15)
#define ID_BUTTON_4    (GUI_ID_USER + 0x16)
#define ID_BUTTON_5    (GUI_ID_USER + 0x17)
#define ID_BUTTON_6    (GUI_ID_USER + 0x18) 
#define ID_TEXT_9    (GUI_ID_USER + 0x19)
#define ID_TEXT_10    (GUI_ID_USER + 0x1A)
#define ID_TEXT_11    (GUI_ID_USER + 0x1B)
#define ID_TEXT_12    (GUI_ID_USER + 0x1C)
#define ID_TEXT_13    (GUI_ID_USER + 0x1D)
#define ID_CHECKBOX_0   (GUI_ID_USER + 0x1E)
#define ID_CHECKBOX_1    (GUI_ID_USER + 0x1F)
#define ID_CHECKBOX_2   (GUI_ID_USER + 0x20)
#define ID_CHECKBOX_3   (GUI_ID_USER + 0x21)
#define ID_CHECKBOX_4   (GUI_ID_USER + 0x22)
#define ID_CHECKBOX_5   (GUI_ID_USER + 0x23)

extern WM_HWIN hPage[25];
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsongti15;
static const GUI_WIDGET_CREATE_INFO _aDialogCreateS[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "D", ID_TEXT_0, 30, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "S", ID_TEXT_1, 145, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Y", ID_TEXT_2, 260, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "H", ID_TEXT_3, 380, 25, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "fanglei", ID_CHECKBOX_0, 102, 138, 20, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "fanglei", ID_TEXT_4, 7, 137, 90, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Ahuoerbianbi", ID_TEXT_5, 145, 137, 85, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Ahuoerbianbi", ID_EDIT_0, 238, 135, 50, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Switch_function", ID_TEXT_6, 7, 207, 90, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Switch", ID_CHECKBOX_1, 102, 208, 20, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Abranch_num", ID_TEXT_7, 147, 207, 100, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Abranch_num", ID_EDIT_1, 238, 205, 50, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Bbranch_num", ID_TEXT_8, 323, 207, 100, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Bbranch_num", ID_EDIT_2, 416, 205, 50, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Config_ok", ID_BUTTON_0, 1, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Device_Information", ID_BUTTON_1, 69, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Home_Page", ID_BUTTON_2, 137, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Clean_Max", ID_BUTTON_3, 205, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Clean_Energy", ID_BUTTON_4, 273, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Change_Password", ID_BUTTON_5, 341, 236, 68, 23, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Heip", ID_BUTTON_6, 410, 236, 68, 23, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Ie_A", ID_TEXT_9, 115, 172,120, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Ie_A", ID_EDIT_3, 238, 170, 50, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Ie_B", ID_TEXT_10, 295, 172, 120, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Ie_B", ID_EDIT_4, 416, 170, 50, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Bhuoerbianbi", ID_TEXT_11, 321, 137, 85, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Bhuoerbianbi", ID_EDIT_5, 416, 135, 50, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_12, 428,259, 45, 13, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_13, 360, 259, 55, 13, 0, 0x64, 0 },
	{ CHECKBOX_CreateIndirect, "System_D", ID_CHECKBOX_2, 55,43, 20, 20, 0, 0x0, 0   },
	{ CHECKBOX_CreateIndirect, "System_S", ID_CHECKBOX_3, 170,43, 20, 20, 0, 0x0, 0  },
	{ CHECKBOX_CreateIndirect, "System_Y", ID_CHECKBOX_4, 285,43, 20, 20, 0, 0x0, 0   },
	{ CHECKBOX_CreateIndirect, "System_H", ID_CHECKBOX_5, 400,43, 20, 20, 0, 0x0, 0  },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreateD[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "D", ID_TEXT_0, 30, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "S", ID_TEXT_1, 145, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Y", ID_TEXT_2, 260, 25, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "H", ID_TEXT_3, 380, 25, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "fanglei", ID_CHECKBOX_0, 102, 148, 20, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "fanglei", ID_TEXT_4, 7, 147, 90, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "huoerbianbi", ID_TEXT_5, 145, 147, 85, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "huoerbianbi", ID_EDIT_0, 233, 145, 50, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Switch_function", ID_TEXT_6, 7,197, 90, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Switch", ID_CHECKBOX_1, 102, 198, 20, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "branch_num", ID_TEXT_7, 147, 197, 100, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "branch_num", ID_EDIT_1, 233, 195, 50, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Config_ok", ID_BUTTON_0, 1, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Device_Information", ID_BUTTON_1, 69, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Home_Page", ID_BUTTON_2, 137, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Clean_Max", ID_BUTTON_3, 205, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Clean_Energy", ID_BUTTON_4, 273, 236, 67, 23, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Change_Password", ID_BUTTON_5, 341, 236, 68, 23, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Heip", ID_BUTTON_6, 410, 236, 68, 23, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Ie_A", ID_TEXT_9, 300, 148,120, 20, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Ie_A", ID_EDIT_3, 416, 147, 50, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_12, 428,259, 45, 13, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_13, 360, 259, 55, 13, 0, 0x64, 0 },
	{ CHECKBOX_CreateIndirect, "System_D", ID_CHECKBOX_2, 55,43, 20, 20, 0, 0x0, 0   },
	{ CHECKBOX_CreateIndirect, "System_S", ID_CHECKBOX_3, 170,43, 20, 20, 0, 0x0, 0  },
	{ CHECKBOX_CreateIndirect, "System_Y", ID_CHECKBOX_4, 285,43, 20, 20, 0, 0x0, 0   },
	{ CHECKBOX_CreateIndirect, "System_H", ID_CHECKBOX_5, 400,43, 20, 20, 0, 0x0, 0  },
};



static void Draw_Fuse(int x,int y)
{
	GUI_DrawCircle(x,y,5);
	GUI_DrawCircle(x-10,y,5);
	GUI_DrawLine(x-10,y-8,x-10,y+8);
	GUI_DrawCircle(x+10,y,5);
	GUI_DrawLine(x+10,y-8,x+10,y+8);
}

static void System_Con_Init(WM_MESSAGE * pMsg,WM_HWIN hItem,char *buf)
{
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"独立单路(D)");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"独立双路(S)");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"主备双路(Y)");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"主备互锁(H)");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"防雷监测功能");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"母线CT变比");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"开关监测功能");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"母线支路数");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);		
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
	TEXT_SetFont(hItem,&GUI_Fontsongti15);
	TEXT_SetText(hItem,"母线额定电流(A)");
	TEXT_SetTextColor(hItem,GUI_BLUE_98);	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"确认配置");	
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"设备信息");	
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"主页");		
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"清除最值");
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);       
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"清除电能");
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"修改密码");
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
	BUTTON_SetFont(hItem,&GUI_Fontsongti15);
	BUTTON_SetText(hItem,"帮助");
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);    //A母线电流变比
	EDIT_SetFont(hItem,&GUI_Fontsongti15);
	EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	sprintf(buf,"%d",Optimset_st.usCTR[0]);
	EDIT_SetText(hItem,buf);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);         //A母线支路数
	EDIT_SetFont(hItem,&GUI_Fontsongti15);
	EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	sprintf(buf,"%d",Optimset_st.ucBranchN_a[0]);
	EDIT_SetText(hItem,buf);
	hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);        //A母线额定电流
	EDIT_SetFont(hItem,&GUI_Fontsongti15);
	EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	sprintf(buf,"%d",Optimset_st.usRateI[0]);
	EDIT_SetText(hItem,buf);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);
	hItem=WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
	CHECKBOX_SetBkColor(hItem,GUI_WHITE);

}

static void _cbSystem_ConD(WM_MESSAGE * pMsg) {
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  int              i;
  static char  buf[5]="0";	
	static unsigned char systemD=0,systemS=0,systemH=0,systemY=0,BusMode=0,fanglei=0,Switch=0;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:		
	  System_Con_Init(pMsg,hItem,buf);
    for (i = 0; i < 3; i++)
		{
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0+i);  /* Get the handle of the edit widget */
			EDIT_SetFocussable(hItem,1);
			if(User_login==0x00)
				EDIT_SetFocussable(hItem,0);
    }
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeD)
		{
			systemD=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),1);
		}
		else systemD=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeS)
		{
			systemS=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),1);
		}
		else systemS=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeY)
		{
			systemY=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),1);
		}
		else systemY=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeH)
		{
			systemH=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),1);
		}
		else systemH=0;
		if(Optimset_st.Setup.Setup_st.bQF==0x01)
		{
		  Switch=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),1);
		}
		else 
	  {
		  Switch=0;
 		  CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);
		}
		if(Optimset_st.Setup.Setup_st.bSPD==0x01)
		{
		  fanglei=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),1);
		}
		else  
		{
		  fanglei=0;
		  CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);
		}
    break;
	case WM_PAINT:
	   	GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(25,65,105,125);
			GUI_DrawLine(65,65,65,82);
			GUI_DrawLine(60,77,70,87);
			GUI_DrawLine(70,77,60,87);
			GUI_DrawLine(58,90,65,95);
			GUI_DrawLine(65,95,65,125);
			Draw_Fuse(65,107);

			GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(140,65,220,125);
			GUI_DrawLine(163,65,163,82);
			GUI_DrawLine(158,77,168,87);
			GUI_DrawLine(168,77,158,87);
			GUI_DrawLine(156,90,163,95);
			GUI_DrawLine(163,95,163,125);
			Draw_Fuse(163,107);
			GUI_DrawLine(197,65,197,82);
			GUI_DrawLine(192,77,202,87);
			GUI_DrawLine(202,77,192,87);
			GUI_DrawLine(190,90,197,95);
			GUI_DrawLine(197,95,197,125);
			Draw_Fuse(197,107);

			GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(255,65,335,125);  
			GUI_DrawLine(278,65,278,82);
			GUI_DrawLine(273,77,283,87);
			GUI_DrawLine(283,77,273,87);
			GUI_DrawLine(271,90,278,95);
			GUI_DrawLine(278,95,278,118);
			Draw_Fuse(278,107);
			GUI_DrawLine(312,65,312,82);
			GUI_DrawLine(307,77,317,87);
			GUI_DrawLine(317,77,307,87);
			GUI_DrawLine(306,90,312,95);
			GUI_DrawLine(312,95,312,118);
			Draw_Fuse(312,107);
			GUI_DrawLine(278,118,312,118);
			GUI_DrawLine(295,118,295,125);
			   
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(370,65,450,125);
			GUI_DrawLine(393,65,393,82);
			GUI_DrawLine(388,77,398,87);
			GUI_DrawLine(398,77,388,87);
			GUI_DrawLine(386,90,393,95);
			GUI_DrawLine(393,95,393,103);

			GUI_DrawLine(427,65,427,82);
			GUI_DrawLine(422,77,432,87);
			GUI_DrawLine(432,77,422,87);
			GUI_DrawLine(420,90,427,95);
			GUI_DrawLine(427,95,427,103);
			GUI_DrawLine(393,103,427,103);
			GUI_DrawLine(410,103,410,125);
			Draw_Fuse(410,113);

			GUI_SetColor(GUI_LIGHTBLUE);
			GUI_FillRect(0,0,480,20);
			GUI_SetColor(GUI_LIGHTBLUE);
		  GUI_FillRect(0,260,480,272);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Fontsongti15);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt("交流配电监测单元",140,3);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font13_1);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt(SoftVersion,5,4);

			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Fontsongti15);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt("系统配置",400,3);

			if(systemD==1)
		  {
		    BusMode=BusModeD;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),0);
			}
		  if(systemS==1)
		  {
		    BusMode=BusModeS;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),0);
			}
			if(systemY==1)
		  {
		    BusMode=BusModeY;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),0);
			}
			if(systemH==1)
		  {	
		    BusMode=BusModeH;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),0);
			}
			 if(fanglei==1) 
			{	
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);
			}
			if(Switch==1)
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),1);
			}
			else
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);
			}
	  break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
		switch (NCode)
		{
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:   
				if((Id>=ID_EDIT_0)&&(Id<=ID_EDIT_3))
				{
					if((User_login==0xFF)||(User_login==0xCC))
					{
					 if(Keyboard_Call==0) hPage[13] = CreateWindowNumberpad(hPage[8]);
						EDIT_SetText(WM_GetDialogItem(pMsg->hWin, Id),"");
						WM_SetFocus(WM_GetDialogItem(pMsg->hWin, Id));
						Keyboard_Call+=1;
					}
					else
						hPage[12] = CreateWindowKeypad(hPage[8]);		
				}	
				switch(Id)
				{
					case ID_CHECKBOX_0:
							if((User_login==0xFF)||(User_login==0xCC))
								fanglei=!fanglei;
								WM_InvalidateWindow(pMsg->hWin);
							break;
					case ID_CHECKBOX_1:
						if((User_login==0xFF)||(User_login==0xCC))
							Switch=!Switch;
							WM_InvalidateWindow(pMsg->hWin);
						break;
					case ID_CHECKBOX_2:
						if((User_login==0xFF)||(User_login==0xCC))
							{
								systemD=1;
								systemS=0;
								systemY=0;
								systemH=0;
							}
								WM_InvalidateWindow(pMsg->hWin);
							break;
						case ID_CHECKBOX_3:
							if((User_login==0xFF)||(User_login==0xCC))
							{
								systemD=0;
								systemS=1;
								systemY=0;
								systemH=0;
							}
								WM_InvalidateWindow(pMsg->hWin);
							break;
						case ID_CHECKBOX_4:
							if((User_login==0xFF)||(User_login==0xCC))
							{
								systemD=0;
								systemS=0;
								systemY=1;
								systemH=0;
							}
								WM_InvalidateWindow(pMsg->hWin);				
							break;
						case ID_CHECKBOX_5:
							if((User_login==0xFF)||(User_login==0xCC))
							{
								systemD=0;
								systemS=0;
								systemY=0;
								systemH=1;
							}
								WM_InvalidateWindow(pMsg->hWin);
						break;
				 case ID_BUTTON_0: // Notifications sent by 'Config_ok'
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), buf, sizeof(buf)); //A母线电流变比
						Optimset_st.usCTR[0] = atoi(buf); 
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), buf, sizeof(buf)); //A母线支路数
						Optimset_st.ucBranchN_a[0]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), buf, sizeof(buf)); //B母线支路数
						Optimset_st.ucBranchN_a[1]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3), buf, sizeof(buf)); //A母线额定电流
						Optimset_st.usRateI[0]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4), buf, sizeof(buf)); //B母线额定电流
						Optimset_st.usRateI[1]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5), buf, sizeof(buf)); //B母线电流变比
						Optimset_st.usCTR[1]=atoi(buf);
						Optimset_st.Setup.Setup_st.bBusMode=BusMode;
						Optimset_st.Setup.Setup_st.bSPD=fanglei;       //防雷功能                              
						Optimset_st.Setup.Setup_st.bQF=Switch;         //开关功能
						Opt_Save();	
					 break;
				 case ID_BUTTON_1: // Notifications sent by 'Device_Information'
					  WM_DeleteWindow(hPage[18]);
						hPage[9] = CreateWindowDevice_Inform(WM_HBKWIN);
						WM_ShowWindow(hPage[9]);
					 break;
				 case ID_BUTTON_2: // Notifications sent by 'Home_Page'
					 
					 if((User_login==0xCC)||(User_login==0xFF))
					 {
					   hPage[20] = CreateWindowUserlogin_Clear(hPage[18]);
						 WM_ShowWindow(hPage[20]);
					 }
					 else
					 {
						 WM_DeleteWindow(hPage[18]);
						 hPage[0]=CreateWindowHome(WM_HBKWIN);
						 WM_ShowWindow(hPage[0]);
					 }
           break; 
				 case ID_BUTTON_3: // Notifications sent by 'Clean_Max'
					 if(User_login==0x00)
						{
							 Window_Call_Keypad=0x02;
							 hPage[12] = CreateWindowKeypad(hPage[18]);
							 WM_ShowWindow(hPage[12]);
						}
						else
					    Acq_MAXClear();
					 break;
				 case ID_BUTTON_4: // Notifications sent by 'Clean_Energy'
					 if(User_login==0x00)
						{
							 Window_Call_Keypad=0x03;
							 hPage[12] = CreateWindowKeypad(hPage[18]);
							 WM_ShowWindow(hPage[12]);
						}
				 	  else
						{
					     hPage[19]=CreateWindowEnergy_Clear(hPage[18]);
							 WM_ShowWindow(hPage[19]);
						}
					 break;
				 case ID_BUTTON_5: // Notifications sent by 'Change_Password'
					 if(User_login==0x00)
						{
							 Window_Call_Keypad=0x04;
							 hPage[12] = CreateWindowKeypad(hPage[18]);
							 WM_ShowWindow(hPage[12]);
						}
					  else if(User_login==0xFF)
						{
						  hPage[21]=CreateWindowChange_Password(hPage[18]);
						  WM_ShowWindow(hPage[21]);
						}
					 break;
				 case ID_BUTTON_6: // Notifications sent by 'Help'
					  Window_Help_call=0x11;
					  WM_DeleteWindow(hPage[18]);
					  hPage[20]= CreateWindow_Help(WM_HBKWIN);
					  WM_ShowWindow(hPage[20]);
					 break;				
	   }
			break;	
   }	
		break;
	case  WM_TIMER://定时器消息,到时间时有效		
		  Caculate_RTC(pMsg,ID_TEXT_13,ID_TEXT_12);	
			WM_RestartTimer(pMsg->Data.v, 100);
		break;
  default:
    WM_DefaultProc(pMsg);
    break;
 }
}


static void _cbSystem_ConS(WM_MESSAGE * pMsg) {
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  int              i;
  static char  buf[5]="0";	
	static unsigned char systemD=0,systemS=0,systemH=0,systemY=0,BusMode=0,fanglei=0,Switch=0;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:		
	  System_Con_Init(pMsg,hItem,buf);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"A母线CT变比");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"A母线支路数");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"B母线支路数");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"A母线额定电流(A)");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"B母线额定电流(A)");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_11);
		TEXT_SetFont(hItem,&GUI_Fontsongti15);
		TEXT_SetText(hItem,"B母线CT变比");
		TEXT_SetTextColor(hItem,GUI_BLUE_98);		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);       //B母线支路数
		EDIT_SetFont(hItem,&GUI_Fontsongti15);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		sprintf(buf,"%d",Optimset_st.ucBranchN_a[1]);
    EDIT_SetText(hItem,buf);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);        //B母线额定电流
		EDIT_SetFont(hItem,&GUI_Fontsongti15);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		sprintf(buf,"%d",Optimset_st.usRateI[1]);
    EDIT_SetText(hItem,buf);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);          //B母线电流变比
		EDIT_SetFont(hItem,&GUI_Fontsongti15);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		sprintf(buf,"%d",Optimset_st.usCTR[1]);
    EDIT_SetText(hItem,buf);
    for (i = 0; i < 6; i++)
		{
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0+i);  /* Get the handle of the edit widget */
			EDIT_SetFocussable(hItem,1);
			if(User_login==0x00)
				EDIT_SetFocussable(hItem,0);
    }
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeD)
		{
			systemD=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),1);
		}
		else systemD=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeS)
		{
			systemS=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),1);
		}
		else systemS=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeY)
		{
			systemY=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),1);
		}
		else systemY=0;
		if(Optimset_st.Setup.Setup_st.bBusMode==BusModeH)
		{
			systemH=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),1);
		}
		else systemH=0;
		if(Optimset_st.Setup.Setup_st.bQF==0x01)
		{
		  Switch=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),1);
		}
		else 
	  {
		  Switch=0;
 		  CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);
		}
		if(Optimset_st.Setup.Setup_st.bSPD==0x01)
		{
		  fanglei=1;
			CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),1);
		}
		else  
		{
		  fanglei=0;
		  CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);
		}
    break;
	case WM_PAINT:
	    GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(25,65,105,125);
			GUI_DrawLine(65,65,65,82);
			GUI_DrawLine(60,77,70,87);
			GUI_DrawLine(70,77,60,87);
			GUI_DrawLine(58,90,65,95);
			GUI_DrawLine(65,95,65,125);
			Draw_Fuse(65,107);

			GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(140,65,220,125);
			GUI_DrawLine(163,65,163,82);
			GUI_DrawLine(158,77,168,87);
			GUI_DrawLine(168,77,158,87);
			GUI_DrawLine(156,90,163,95);
			GUI_DrawLine(163,95,163,125);
			Draw_Fuse(163,107);
			GUI_DrawLine(197,65,197,82);
			GUI_DrawLine(192,77,202,87);
			GUI_DrawLine(202,77,192,87);
			GUI_DrawLine(190,90,197,95);
			GUI_DrawLine(197,95,197,125);
			Draw_Fuse(197,107);

			GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(255,65,335,125);  
			GUI_DrawLine(278,65,278,82);
			GUI_DrawLine(273,77,283,87);
			GUI_DrawLine(283,77,273,87);
			GUI_DrawLine(271,90,278,95);
			GUI_DrawLine(278,95,278,118);
			Draw_Fuse(278,107);
			GUI_DrawLine(312,65,312,82);
			GUI_DrawLine(307,77,317,87);
			GUI_DrawLine(317,77,307,87);
			GUI_DrawLine(306,90,312,95);
			GUI_DrawLine(312,95,312,118);
			Draw_Fuse(312,107);
			GUI_DrawLine(278,118,312,118);
			GUI_DrawLine(295,118,295,125);
			
			GUI_SetColor(GUI_LIGHTBLUE);     
			GUI_SetColor(GUI_LIGHTBLUE);  
			GUI_DrawRect(370,65,450,125);
			GUI_DrawLine(393,65,393,82);
			GUI_DrawLine(388,77,398,87);
			GUI_DrawLine(398,77,388,87);
			GUI_DrawLine(386,90,393,95);
			GUI_DrawLine(393,95,393,103);

			GUI_DrawLine(427,65,427,82);
			GUI_DrawLine(422,77,432,87);
			GUI_DrawLine(432,77,422,87);
			GUI_DrawLine(420,90,427,95);
			GUI_DrawLine(427,95,427,103);
			GUI_DrawLine(393,103,427,103);
			GUI_DrawLine(410,103,410,125);
			Draw_Fuse(410,113);

			GUI_SetColor(GUI_LIGHTBLUE);
			GUI_FillRect(0,0,480,20);
			GUI_SetColor(GUI_LIGHTBLUE);
		  GUI_FillRect(0,260,480,272);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Fontsongti15);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt("交流配电监测单元",140,3);
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font13_1);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt(SoftVersion,5,4);

			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Fontsongti15);
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_DispStringAt("系统配置",400,3);

			if(systemD==1)
		  {
		    BusMode=BusModeD;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2),0);
			}
		  if(systemS==1)
		  {
		    BusMode=BusModeS;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3),0);
			}
			if(systemY==1)
		  {
		    BusMode=BusModeY;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4),0);
			}
			if(systemH==1)
		  {	
		    BusMode=BusModeH;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5),0);
			}
			 if(fanglei==1) 
			{	
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),1);
			}
			else 
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0),0);
			}
			if(Switch==1)
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),1);
			}
			else
			{
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1),0);
			}
	  break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
		switch (NCode)
		{
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:   
				if((Id>=ID_EDIT_0)&&(Id<=ID_EDIT_5))
				{
					if((User_login==0xFF)||(User_login==0xCC))
					{
					 if(Keyboard_Call==0) hPage[13] = CreateWindowNumberpad(hPage[8]);
						EDIT_SetText(WM_GetDialogItem(pMsg->hWin, Id),"");
						WM_SetFocus(WM_GetDialogItem(pMsg->hWin, Id));
						Keyboard_Call+=1;
					}
					else
						hPage[12] = CreateWindowKeypad(hPage[8]);		
				}						
				switch(Id)
				{
					case ID_CHECKBOX_0:
							if((User_login==0xFF)||(User_login==0xCC))
								fanglei=!fanglei;
								WM_InvalidateWindow(pMsg->hWin);
							break;
					case ID_CHECKBOX_1:
						if((User_login==0xFF)||(User_login==0xCC))
							Switch=!Switch;
							WM_InvalidateWindow(pMsg->hWin);
						break;
					case ID_CHECKBOX_2:
						if((User_login==0xFF)||(User_login==0xCC))
						{
						 	systemD=1;
					    systemS=0;
				      systemY=0;
				      systemH=0;
						}
							WM_InvalidateWindow(pMsg->hWin);
						break;
					case ID_CHECKBOX_3:
						if((User_login==0xFF)||(User_login==0xCC))
						{
							systemD=0;
					    systemS=1;
				      systemY=0;
				      systemH=0;
						}
							WM_InvalidateWindow(pMsg->hWin);
						break;
					case ID_CHECKBOX_4:
						if((User_login==0xFF)||(User_login==0xCC))
						{
							systemD=0;
					    systemS=0;
				      systemY=1;
				      systemH=0;
						}
							WM_InvalidateWindow(pMsg->hWin);				
						break;
					case ID_CHECKBOX_5:
						if((User_login==0xFF)||(User_login==0xCC))
						{
							systemD=0;
					    systemS=0;
				      systemY=0;
				      systemH=1;
						}
							WM_InvalidateWindow(pMsg->hWin);
						break;
				 case ID_BUTTON_0: // Notifications sent by 'Config_ok'
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), buf, sizeof(buf)); //A母线电流变比
						Optimset_st.usCTR[0] = atoi(buf); 
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), buf, sizeof(buf)); //A母线支路数
						Optimset_st.ucBranchN_a[0]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), buf, sizeof(buf)); //B母线支路数
						Optimset_st.ucBranchN_a[1]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3), buf, sizeof(buf)); //A母线额定电流
						Optimset_st.usRateI[0]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4), buf, sizeof(buf)); //B母线额定电流
						Optimset_st.usRateI[1]=atoi(buf);
						EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5), buf, sizeof(buf)); //B母线电流变比
						Optimset_st.usCTR[1]=atoi(buf);
				    Optimset_st.Setup.Setup_st.bBusMode=BusMode;
			      Optimset_st.Setup.Setup_st.bSPD=fanglei;       //防雷功能                              
			      Optimset_st.Setup.Setup_st.bQF=Switch;         //开关功能
						Opt_Save();	
           break;
				 case ID_BUTTON_1: // Notifications sent by 'Device_Information'
					  WM_DeleteWindow(hPage[8]);
						hPage[9] = CreateWindowDevice_Inform(WM_HBKWIN);
				    WM_ShowWindow(hPage[9]);
					 break;
				 case ID_BUTTON_2: // Notifications sent by 'Home_Page'
					 if((User_login==0xCC)||(User_login==0xFF))
					 {
					   hPage[20] = CreateWindowUserlogin_Clear(hPage[8]);
						 WM_ShowWindow(hPage[20]);
					 }
					 else
					 {
						 WM_DeleteWindow(hPage[8]);
						 hPage[0]=CreateWindowHome(WM_HBKWIN);
						 WM_ShowWindow(hPage[0]);
					 } 
           break; 
				 case ID_BUTTON_3: // Notifications sent by 'Clean_Max'
					 	if(User_login==0x00)
						{
							 Window_Call_Keypad=0x05;
							 hPage[12] = CreateWindowKeypad(hPage[8]);
							 WM_ShowWindow(hPage[12]);
						}
					   else Acq_MAXClear();
					 break;
				 case ID_BUTTON_4: // Notifications sent by 'Clean_Energy'
					 if(User_login==0x00)
						{
							 Window_Call_Keypad=0x06;
							 hPage[12] = CreateWindowKeypad(hPage[8]);
							 WM_ShowWindow(hPage[12]);
						}
					  else
						{
					     hPage[19]=CreateWindowEnergy_Clear(hPage[8]);
							 WM_ShowWindow(hPage[19]);
						}
					 break;
				 case ID_BUTTON_5: // Notifications sent by 'Change_Password'
					 if(User_login==0x00)
						{
							 Window_Call_Keypad=0x07;
							 hPage[12] = CreateWindowKeypad(hPage[8]);
							 WM_ShowWindow(hPage[12]);
						}
					  else if(User_login==0xFF)
						{
						  hPage[21]=CreateWindowChange_Password(hPage[8]);
						  WM_ShowWindow(hPage[21]);
						}
					 break;
				 case ID_BUTTON_6: // Notifications sent by 'Help'
					  Window_Help_call=0x13;
					  WM_DeleteWindow(hPage[8]);
					  hPage[20]= CreateWindow_Help(WM_HBKWIN);
					  WM_ShowWindow(hPage[20]);
					 break;				
	   }
			break;	
   }	
		break;
	case  WM_TIMER://定时器消息,到时间时有效		
		  Caculate_RTC(pMsg,ID_TEXT_13,ID_TEXT_12);	
			WM_RestartTimer(pMsg->Data.v, 100);
		break;
  default:
    WM_DefaultProc(pMsg);
    break;
 }
}


/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindowSystem_ConS(WM_HWIN hParent);
WM_HWIN CreateWindowSystem_ConS(WM_HWIN hParent) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreateS, GUI_COUNTOF(_aDialogCreateS), _cbSystem_ConS, WM_HBKWIN, 0, 0);
	WM_CreateTimer(WM_GetClientWindow(hWin),0,200,0);
  return hWin;
}

WM_HWIN CreateWindowSystem_ConD(WM_HWIN hParent);
WM_HWIN CreateWindowSystem_ConD(WM_HWIN hParent) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreateD, GUI_COUNTOF(_aDialogCreateD), _cbSystem_ConD, WM_HBKWIN, 0, 0);
	WM_CreateTimer(WM_GetClientWindow(hWin),0,200,0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/