/**
  ******************************************************************************
  * @file    app_Optimset.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   ethernet communication 
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_Optimset.h"
#include "bsp_Fram.h"
#include "sys_FramAlloc.h"
#include "sys_Variable.h"
#include "app_Alarm.h"
#include "cmsis_os.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
#define FMOpt_DevInfo                 (4)
#define FMOpt_UsrPW                   (60)	
#define FMOpt_SwitchSet               (70)

/* private variables ----------------------------------------------------------*/
const struct Optimset_TypeDef      Optimset_Default=
{
    0x01,               /*!< 通信地址 */
	  0x9600,             /*!< 通信波特率 */
		1,                  /*!< 动态IP */
	  {192,168,20,100},   /*!< 本机IP地址 */
		{192,168,20,254},   /*!< 本机网关 */
		{255,255,255,0,},   /*!< 本机子网掩码 */
		{192,168,20,84},    /*!< 远程IP地址 */
		4196,               /*!< 远程端口号 */
		{0x01,1,1,},     /*!< 系统配置 */
	  {200,200,},
		{200,200,},
		{72,72,},
		242,
		187,
		80,
		100,
		{132,132,},
		52,
		48,
};


uint16_t UsrPassword;

/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
/* function body --------------------------------------------------------------*/

/**
  *@brief   Optimset save
  *@param  NONE
  *@retval NONE
  *@note
  */
void Opt_Save(void)
{
   taskENTER_CRITICAL();
   Fram_I2C_Write((uint8_t*)&Optimset_st, FMAddr_OptimSet+FMOpt_DevInfo, sizeof(Optimset_st));
	 taskEXIT_CRITICAL();
	 
	 return;
}

/**
  *@brief  user password save
  *@param  NONE
  *@retval NONE
  *@note
  */
void Opt_PWSave(void)
{  
   taskENTER_CRITICAL();
   Fram_I2C_Write((uint8_t*)&UsrPassword, FMAddr_OptimSet+FMOpt_UsrPW, sizeof(UsrPassword));
   taskEXIT_CRITICAL();	 
}

/**
  *@brief  branch set save
  *@param  Set--Branch set
  *        Num--Branch number
  *@retval NONE
  *@note
  */
void Opt_BranchSet_Save(uint8_t Set, uint8_t Num)
{
   taskENTER_CRITICAL();
   Fram_I2C_Write(&Set, FMAddr_OptimSet + FMOpt_SwitchSet +Num, 1);
	 taskEXIT_CRITICAL();
	 
	 return;
}


/**
  *@brief  optimset init
  *@param  NONE
  *@retval NONE
  *@note
  */
void Opt_Init(void)
{
   uint16_t InitFlag;
	
	 Fram_I2C_Read((uint8_t*)&InitFlag, FMAddr_OptimSet, sizeof(InitFlag));
	
	 /* 已初始化 */
	 if(InitFlag == 0x55AA)
	 {
	    Fram_I2C_Read((uint8_t*)&Optimset_st, FMAddr_OptimSet + FMOpt_DevInfo, sizeof(Optimset_st));
		  Fram_I2C_Read(ucBranchSet_a, FMAddr_OptimSet + FMOpt_SwitchSet, MAXBranchNum);
			Fram_I2C_Read((uint8_t*)&UsrPassword, FMAddr_OptimSet+FMOpt_UsrPW, sizeof(UsrPassword));
	 }
	 else
	 {
	    InitFlag = 0x55AA;
		  Fram_I2C_Write((uint8_t*)&InitFlag, FMAddr_OptimSet, sizeof(InitFlag));
		 
		  memcpy(&Optimset_st,&Optimset_Default,sizeof(struct Optimset_TypeDef));
		  Fram_I2C_Write((uint8_t*)&Optimset_Default, FMAddr_OptimSet + FMOpt_DevInfo, sizeof(struct Optimset_TypeDef));
		 
		  Fram_I2C_Write(ucBranchSet_a, FMAddr_OptimSet + FMOpt_SwitchSet, MAXBranchNum);
			
			UsrPassword = 1234;
			Fram_I2C_Write((uint8_t*)&UsrPassword, FMAddr_OptimSet+FMOpt_UsrPW, sizeof(UsrPassword));
	 }
   
	 return;
}

/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************/



