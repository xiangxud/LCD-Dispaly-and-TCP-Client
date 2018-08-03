/**
  ******************************************************************************
  * @file    app_SUBProto.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   handle alarm 
  * @endverbatim
  ******************************************************************************
	* @note  支路开关状态   
	*        2--闭合（未启用）  3--断开（未启用） 0--正常（启用）  1--故障（启用）
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_SUBProto.h"
#include "bsp_UART.h"
#include "app_PCProto.h"
#include "sys_Variable.h"
#include "Function.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/	
/* private variables ----------------------------------------------------------*/
__IO uint8_t IOChangeFlag;
static uint8_t ucSUB_TxBuff[8];
static uint8_t ucSUB_SWRec[20];
static void  SUB_StateAna(uint8_t RecState, uint8_t SetState, uint8_t *AnaState);
/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
/* function body --------------------------------------------------------------*/

/**
  *@brief  SUB_UART transmit protocol
	*@param  NONE
	*@retval NONE
	*@note
	*/
void SUB_Transmit(void )
{
	 uint8_t i;
	 uint16_t usCRC;
	 static uint8_t ucNum = 0;
	
   switch(ucNum)
	 {
		 /*模块1*/
		 case 0: 
			  ucSUB_TxBuff[0] = 0x09;
		    break;
		 case 1:
			  ucSUB_TxBuff[0] = 0x0A;
		    break;
		 case 2:
			  ucSUB_TxBuff[0] = 0x0B;
		    break;
		 case 3:
			  ucSUB_TxBuff[0] = 0x0C;
		    break;
		 default:break;
	 }
	 
	 ucNum++;
	 if(4 == ucNum)
	 {
	    ucNum = 0;
	 }
		 
	 for(i = 1; i< 6; i ++)
   {
	    ucSUB_TxBuff[i] = 0x30;
	 }
   	 
   usCRC = CRC16(ucSUB_TxBuff,6);
	 memcpy(ucSUB_TxBuff + 6, (uint8_t*)&usCRC, 2);
	  
	 HAL_UART_Transmit_DMA(&hSUB_UART, ucSUB_TxBuff,8);
	 
	 return;
}
	

/**
  *@brief  SUB_UART receive protocol
	*@param  NONE
	*@retval NONE
	*@note   0--succeed, 1--fail
	*/
uint8_t SUB_Receive(uint8_t* pData, uint16_t ucLen )
{ 
	 uint16_t usCRC_Rec, usCRC_Cal;
	 uint8_t m,i,j;
	 static uint8_t ucSUB_SWold[20] = {0x00};
	
	 if(ucLen == 10)
	 {
	    if(pData[0] == 0x02)
			{
			   usCRC_Cal = CRC16(pData, 8);
				 memcpy((uint8_t *)&usCRC_Rec, pData+8, 2);
				
				 if(usCRC_Cal == usCRC_Rec)
				 {
				    m = pData[1];
				    if(m >= 0x09)
						{
						   m = m - 0x09;
							 ucSUB_SWRec[m*5+0] = pData[3];
							 ucSUB_SWRec[m*5+1] = pData[4];
							 ucSUB_SWRec[m*5+2] = pData[5];
							 ucSUB_SWRec[m*5+3] = pData[6];
							 ucSUB_SWRec[m*5+4] = pData[7];
               for(j=0; j< 5; j++)
               {
							    if(ucSUB_SWRec[m*5+j] != ucSUB_SWold[m*5+j])
									{
									   for(i=0; i<5; i++)
										 {
										     ucSUB_SWold[i*5+i] = ucSUB_SWRec[i*5+i];
										 }
										 switch(m)
										 {
										    case 0x00:
												     SET_BIT(IOChangeFlag, 0x01);
												     break;
												case 0x01:
												     SET_BIT(IOChangeFlag, 0x02);
												     break;
												case 0x02:
												     SET_BIT(IOChangeFlag, 0x04);
												     break;
												case 0x03:
												     SET_BIT(IOChangeFlag, 0x08);
												     break;
												default: break;
										 }								
										 break;
									}
							 }				 
							 return 0;
						}
						else
							 return 1;
				 }
				 else
					 return 1;
			}
			else
				return 1;
	 }
	 else
		 return 1;
}

/**
  *@brief  branch switch status 
  *@param  NONE
  *@retval NONE
  *@note   S型系统中，开关状态和开关设置均以前72路为A路，后72路为B路
  */
void  SUB_BranchState(void)
{
    uint8_t i, temp, StateRecord[MAXBranchNum];

	  if(Optimset_st.Setup.Setup_st.bQF == 1)
		{
				if(Optimset_st.Setup.Setup_st.bBusMode == BusModeS)
				{
						for(i = 0; i < 9; i ++)
						{
								/* 解析接收到的开关状态字节的低4位 */
								if(i % 2 == 0)
								{		
										/* A路开关状态 */							
										temp = ((ucSUB_SWRec[i/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+0], &ucBranchState_a[i*4+0] );							
										temp = ((ucSUB_SWRec[i/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+1], &ucBranchState_a[i*4+1] );							
										temp = ((ucSUB_SWRec[i/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+2], &ucBranchState_a[i*4+2] );							
										temp = ((ucSUB_SWRec[i/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+3], &ucBranchState_a[i*4+3] );
									
										temp = ((ucSUB_SWRec[(i+20)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+0], &ucBranchState_a[(i+9)*4+0] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+1], &ucBranchState_a[(i+9)*4+1] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+2], &ucBranchState_a[(i+9)*4+2] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+3], &ucBranchState_a[(i+9)*4+3] );
									
										/* B路开关状态 */
										temp = ((ucSUB_SWRec[(i+10)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+0], &ucBranchState_a[(i+18)*4+0] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+1], &ucBranchState_a[(i+18)*4+1] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+2], &ucBranchState_a[(i+18)*4+2] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+3], &ucBranchState_a[(i+18)*4+3] );	
									
										temp = ((ucSUB_SWRec[(i+30)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+0], &ucBranchState_a[(i+27)*4+0] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+1], &ucBranchState_a[(i+27)*4+1] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+2], &ucBranchState_a[(i+27)*4+2] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+3], &ucBranchState_a[(i+27)*4+3] );							  
								}
								/* 解析接收到的开关状态字节的高4位 */
								else
								{
										temp = ((ucSUB_SWRec[i/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+0], &ucBranchState_a[i*4+0] );							
										temp = ((ucSUB_SWRec[i/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+1], &ucBranchState_a[i*4+1] );							
										temp = ((ucSUB_SWRec[i/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+2], &ucBranchState_a[i*4+2] );							
										temp = ((ucSUB_SWRec[i/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+3], &ucBranchState_a[i*4+3] );
									
										temp = ((ucSUB_SWRec[(i+20)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+0], &ucBranchState_a[(i+9)*4+0] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+1], &ucBranchState_a[(i+9)*4+1] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+2], &ucBranchState_a[(i+9)*4+2] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+3], &ucBranchState_a[(i+9)*4+3] );
									
										/* B路开关状态 */
										temp = ((ucSUB_SWRec[(i+10)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+0], &ucBranchState_a[(i+18)*4+0] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+1], &ucBranchState_a[(i+18)*4+1] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+2], &ucBranchState_a[(i+18)*4+2] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+3], &ucBranchState_a[(i+18)*4+3] );	
									
										temp = ((ucSUB_SWRec[(i+30)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+0], &ucBranchState_a[(i+27)*4+0] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+1], &ucBranchState_a[(i+27)*4+1] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+2], &ucBranchState_a[(i+27)*4+2] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+3], &ucBranchState_a[(i+27)*4+3] );		
								}
						}
						for(i = Optimset_st.ucBranchN_a[0]; i <72; i++ )
						{
						   ucBranchState_a[i] = 0x20;
						}
						for(i = 72 + Optimset_st.ucBranchN_a[1]; i < MAXBranchNum; i++ )
						{
						   ucBranchState_a[i] = 0x20;
						}
						for(i = 0; i < MAXBranchNum; i ++)
						{
						   if(ucBranchState_a[i] != StateRecord[i])
							 {
							     ucPCProto_DataFlag |= 0x10;
								   StateRecord[i] = ucBranchState_a[i];
							 }
						}
				}
				else
				{
						for(i  =0;i < 9; i ++)
						{
								/* 解析接收到的开关状态字节的低4位 */
								if(i % 2 == 0)
								{								
										temp = ((ucSUB_SWRec[i/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+0], &ucBranchState_a[i*4+0] );							
										temp = ((ucSUB_SWRec[i/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+1], &ucBranchState_a[i*4+1] );							
										temp = ((ucSUB_SWRec[i/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+2], &ucBranchState_a[i*4+2] );							
										temp = ((ucSUB_SWRec[i/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+3], &ucBranchState_a[i*4+3] );
									
										temp = ((ucSUB_SWRec[(i+10)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+0], &ucBranchState_a[(i+9)*4+0] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+1], &ucBranchState_a[(i+9)*4+1] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+2], &ucBranchState_a[(i+9)*4+2] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+3], &ucBranchState_a[(i+9)*4+3] );
									
										temp = ((ucSUB_SWRec[(i+20)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+0], &ucBranchState_a[(i+18)*4+0] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+1], &ucBranchState_a[(i+18)*4+1] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+2], &ucBranchState_a[(i+18)*4+2] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+3], &ucBranchState_a[(i+18)*4+3] );	
									
										temp = ((ucSUB_SWRec[(i+30)/2] >> 0) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+0], &ucBranchState_a[(i+27)*4+0] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 1) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+1], &ucBranchState_a[(i+27)*4+1] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 2) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+2], &ucBranchState_a[(i+27)*4+2] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 3) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+3], &ucBranchState_a[(i+27)*4+3] );							  
								}
								/* 解析接收到的开关状态字节的高4位 */
								else
								{
										temp = ((ucSUB_SWRec[i/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+0], &ucBranchState_a[i*4+0] );							
										temp = ((ucSUB_SWRec[i/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+1], &ucBranchState_a[i*4+1] );							
										temp = ((ucSUB_SWRec[i/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+2], &ucBranchState_a[i*4+2] );							
										temp = ((ucSUB_SWRec[i/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[i*4+3], &ucBranchState_a[i*4+3] );
									
										temp = ((ucSUB_SWRec[(i+10)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+0], &ucBranchState_a[(i+9)*4+0] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+1], &ucBranchState_a[(i+9)*4+1] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+2], &ucBranchState_a[(i+9)*4+2] );
										temp = ((ucSUB_SWRec[(i+10)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+9)*4+3], &ucBranchState_a[(i+9)*4+3] );
									
										temp = ((ucSUB_SWRec[(i+20)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+0], &ucBranchState_a[(i+18)*4+0] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+1], &ucBranchState_a[(i+18)*4+1] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+2], &ucBranchState_a[(i+18)*4+2] );
										temp = ((ucSUB_SWRec[(i+20)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+18)*4+3], &ucBranchState_a[(i+18)*4+3] );	
									
										temp = ((ucSUB_SWRec[(i+30)/2] >> 4) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+0], &ucBranchState_a[(i+27)*4+0] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 5) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+1], &ucBranchState_a[(i+27)*4+1] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 6) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+2], &ucBranchState_a[(i+27)*4+2] );
										temp = ((ucSUB_SWRec[(i+30)/2] >> 7) & 0x01);
										SUB_StateAna(temp, ucBranchSet_a[(i+27)*4+3], &ucBranchState_a[(i+27)*4+3] );		
								}
						}
						for(i = Optimset_st.ucBranchN_a[0]; i < MAXBranchNum; i++)
						{
							 ucBranchState_a[i] = 0x20;
						}
						for(i = 0; i < MAXBranchNum; i ++)
						{
							 if(ucBranchState_a[i] != StateRecord[i])
							 {
									 ucPCProto_DataFlag |= 0x10;
									 StateRecord[i] = ucBranchState_a[i];
							 }
						}
				}
		}
		else
		{
		    for(i = 0 ; i < MAXBranchNum; i ++ )
			  {
			      ucBranchState_a[i] = 0x20;
			  } 
		}
		
		return;
}


/**
  *@brief  branch switch status analyze sub 
  *@param  NONE
  *@retval NONE
  *@note   1）2--闭合（未启用）  3--断开（未启用） 0--正常（启用）  1--故障（启用）
  */
static void  SUB_StateAna(uint8_t RecState, uint8_t SetState, uint8_t *AnaState)
{
	  /* 未启用 */
		if(SetState == 0x00)    
		{
				if(RecState == 0x01) //闭合
				{
					 *AnaState = 0x02;
				}
				else    //断开
				{
					 *AnaState = 0x03;
				}
		}
		/* 启用 */
		else
		{
				if(RecState == 0x01) //正常
				{
					 *AnaState = 0x00;
				}
				else    //故障
				{
					 *AnaState = 0x01;
				}
		}
		return;
}



/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/


