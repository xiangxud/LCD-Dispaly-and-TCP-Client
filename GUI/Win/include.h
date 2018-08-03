#ifndef INCLUDE_H           
#define INCLUDE_H
#include "DIALOG.h"
#include "app_Alarm.h"

void Caculate_RTC(WM_MESSAGE * pMsg,int ID_TEXT_Data,int ID_TEXT_Time);
void MainTask(void);
WM_HWIN CreateWindowHome(WM_HWIN hParent);
WM_HWIN CreateWindowBusData_1(WM_HWIN hParent);
WM_HWIN CreateWindowBusData_2(WM_HWIN hParent);
WM_HWIN CreateWindowSystem_ConS(WM_HWIN hParent);
WM_HWIN CreateWindowDevice_Inform(WM_HWIN hParent);
WM_HWIN CreateWindowNowAlarm(WM_HWIN hParent);
WM_HWIN CreateWindowHistoryAlarm(WM_HWIN hParent);
WM_HWIN CreateWindowKeypad(WM_HWIN hParent);
WM_HWIN CreateWindowNumberpad(WM_HWIN hParent);
WM_HWIN CreateWindowAHistory_Energy_1(WM_HWIN hParent);
WM_HWIN CreateWindowAHistory_Energy_2(WM_HWIN hParent);
WM_HWIN CreateWindowSystem_ConD(WM_HWIN hParent);
WM_HWIN CreateWindowEnergy_Clear(WM_HWIN hParent);
WM_HWIN CreateWindow_Help(WM_HWIN hParent);
WM_HWIN CreateWindowChange_Password(WM_HWIN hParent);
WM_HWIN CreateWindowUserlogin_Clear(WM_HWIN hParent);
void delay_ms(U16 nms);
void gAlm_InfoAnalyze(const enum AlmEventTypeDef event, const enum AlmTypeDef type, char *CHbuf);
extern uint8_t User_login;
extern uint8_t Keyboard_Call;
extern int Branch_Num;
extern union un1   Y;
extern uint8_t Window_Help_call;
extern uint16_t SavePassword;
extern uint8_t Window_Call_Keypad;
#endif

