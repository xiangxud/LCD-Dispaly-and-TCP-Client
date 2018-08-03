/**
  ******************************************************************************
  * @file    app_PCProto.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    23-May-2018
  * @brief   handle alarm 
  * @endverbatim
	*-----------------------------------------------------------------------------
	*@note  1.����������˳���ȵ��ֽں���ֽ�
	*       2.����������˳���ȸ��ֽں���ֽ�
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_PCProto.h"
#include "bsp_UART.h"
#include "bsp_RTC.h"
#include "bsp_Fram.h"
#include "app_Alarm.h"
#include "app_Optimset.h"
#include "app_PTEnergy.h"
#include "function.h"
#include "sys_framAlloc.h"
#include "sys_Variable.h"
#include "cmsis_os.h"
#include "string.h"

/* private type definitions ---------------------------------------------------*/
typedef struct FrameFormatTypeDef
{
   uint8_t  SOI;
	 uint8_t  VER;
	 uint8_t  ADR;
	 uint8_t  CID1;
	 uint8_t  CID2;
	 uint16_t LENGTH;
	 void  *INFO;
	 uint16_t CHKSUM;
	 uint8_t  EOI; 
}FrameFormatTypeDef;

FrameFormatTypeDef   FramStruct_st;


typedef enum CID2TypeDef
{
    ePutAnalog           =0x41,      /*!< ��ȡģ����ָ�� */
	  ePutIO               =0x43,      /*!< ��ȡ����״ָ̬�� */
	  ePutAlarm            =0x44,      /*!< ��ȡ�澯״ָ̬�� */
	  ePutOptimset         =0x47,      /*!< ��ȡ����ָ��������� */
	  eSetOptimset         =0x49,      /*!< �趨����ָ��������� */  
	  ePutPTEnergy         =0x4B,      /*!< ��ȡ��ʷ����ָ��������� */
	  ePutPTAlarm          =0x4C,      /*!< ��ȡ��ʷ�澯ָ�� */
	  ePutTime             =0x4D,      /*!< ��ȡʱ��ָ�� */
	  eSetTime             =0x4E,      /*!< �趨ʱ��ָ�� */
	  ePutVersion          =0x4F,      /*!< ��ȡ�汾��ָ�� */
	  ePutAddress          =0x50,      /*!< ��ȡ��ַָ�� */
	  ePutBrands           =0x51,      /*!< ��ȡ������Ϣָ�� */
	  ePutDevice           =0x80,      /*!< ��ȡ�豸����ָ�� */
}CID2TypeDef;


typedef enum  RTNTypeDef
{
    eNormal,                         /*!< ���� */
    eVERError,                       /*!< �汾�Ŵ��� */
    eCHKError,                       /*!< ֡��У����� */
	  eLCHKError,                      /*!< ֡���Ⱥ�У����� */
	  eCID2Void,                       /*!< CID2��Ч */                    
	  eOFError,                        /*!< �����ʽ���󣬳��Ȳ����֡ͷ֡β����ʱ�ж�Ϊ��ʽ���� */
	  eVoidData,                       /*!< ��Ч���ݣ�command_type���� */                       
}RTNTypeDef;


typedef enum  RxHandleTypeDef
{
    eSEOIOK,                         /*!< ֡ͷ֡β��ȷ */
	  eCID1OK,                         /*!< CID1��ȷ */
	  eCHKOK,                          /*!< ��У����ȷ */
	  eADROK,                          /*!< ��ַ��ȷ */
	  eVEROK,                          /*!< �汾��ȷ */
}RxHandleTypeDef;

/* private macro definitions --------------------------------------------------*/
#define  FramFrm_SOI         0x7E
#define  FramFrm_VER         0x31
#define  FramFrm_EOI         0x0D


/* private variables ----------------------------------------------------------*/
uint8_t  ucPCProto_DataFlag = 0;                          /*!< Э���׼YD 1363-3 
                                                           �е����ݸ��±�־data_flag*/

const uint8_t ucSYSName[10] = "DPJ_G1K   ";              /*!< ϵͳ����*/   
const uint16_t ucSoftVer = 0x030B;                       /*!< ����汾 */
const uint8_t ucBrands[20] = "HAIHONGTONGXIN      ";     /*!< �������� */

static uint8_t *DataInfo  ;                              /*!< data_info��Hex��ʽ���� */ 


/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
static uint16_t nTxDataInfo(const uint8_t *pRxData, CID2TypeDef CID2, RTNTypeDef  *RTN);
static void nTxFramHandle(const uint16_t len, RTNTypeDef RTN);
static void nSet_DataInfo(const uint8_t command, const uint8_t* data, const uint16_t len, RTNTypeDef *RTN);
static uint16_t nAnolog_DataInfo(const uint8_t Command, RTNTypeDef *RTN);
static uint16_t nRTAlm_DataInfo(const uint8_t Command, RTNTypeDef *RTN);
static uint16_t nIO_DataInfo(const uint8_t Command, RTNTypeDef *RTN);
static uint16_t nPutOpt_DataInfo(const uint8_t Command, RTNTypeDef *RTN);
static uint16_t nPTEnergy_DataInfo(uint8_t Command,RTNTypeDef *RTN);
static uint16_t nPTAlarm_DataInfo(uint8_t Command, RTNTypeDef *RTN);
static uint16_t nPutTime_DataInfo(void);
static uint16_t nBrand_DataInfo(void);
static uint16_t nDevice_DataInfo(void);
														 
/* function body --------------------------------------------------------------*/


/**
  *@brief  PC protocol handle
	*@param  pRxBuf--poiter of received fram
	*        RxLen --Length of received fram
	*@retval 0--�ɹ��� 1--ʧ��
	*@note
	*/
uint8_t gPCProtoHandle(const uint8_t *pRxData, const uint16_t len)
{	
		RTNTypeDef RTN = eNormal;
	  RxHandleTypeDef  ucState = eSEOIOK;
	
	  uint8_t ucCommand, *ucRxHexBuf;
	
	  uint16_t usHexLen = 0, halfLenid = 0;	
	  uint16_t usRxCHKsum = 0,usCalCHKsum = 0; 
		
	  ucRxHexBuf = NULL;
	  DataInfo = NULL;
	
		/* ֡ͷ֡β�ж� */	
	  if((pRxData[0] == FramFrm_SOI) && (pRxData[len - 1] == FramFrm_EOI))
		{
			 FramStruct_st.SOI = FramFrm_SOI;
			 FramStruct_st.EOI = FramFrm_EOI;
			
		   ucState = eSEOIOK;		
		}
		else
		{
			 	
			 return 1;
		}
		
		switch(ucState)
		{
      /* CID1�ж� */
			case eSEOIOK:
				   usHexLen = (len - 2)/2;
			     
			     /* ���붯̬�ڴ� */
			     ucRxHexBuf = (uint8_t *)pvPortMalloc(usHexLen);
			
			     ASCII2HEX(ucRxHexBuf, pRxData+1, usHexLen );
			     if(FramFrm_CID1 == ucRxHexBuf[2])
					 {
					     ucState = eCID1OK;
					 }
					 else
					 {
						   /* �ͷ��ڴ� */
							 vPortFree(ucRxHexBuf);
						   return 1;
					 }
					 
			/* ��ַ�жϣ���ȡ��ַʱ���ж� */
			case eCID1OK:
				   ucCommand =(CID2TypeDef)ucRxHexBuf[3];
			     if(ucCommand == ePutAddress)
					 {
							 usCalCHKsum = CHKsum(pRxData, len - 5); 
							 usRxCHKsum = (ucRxHexBuf[usHexLen - 2] << 8) | ucRxHexBuf[usHexLen - 1];
							 if(usCalCHKsum == usRxCHKsum) 
							 {
									 FramStruct_st.VER = ucRxHexBuf[0];
									 FramStruct_st.ADR = Optimset_st.ucComAddr;
									 FramStruct_st.CID1 = FramFrm_CID1;					 
									 halfLenid = 0;
								   goto Tx;
							 }
							 else
							 {
								 	 FramStruct_st.VER = ucRxHexBuf[0];
							     FramStruct_st.ADR = ucRxHexBuf[1];
						       FramStruct_st.CID1 = FramFrm_CID1;
									 RTN = eCHKError;
									 halfLenid = 0;
									 goto Tx;
							 }
					 }
					 else
					 {
					     if(ucRxHexBuf[1] == Optimset_st.ucComAddr)
							 {
							     ucState = eADROK;
							 }
							 else
							 {
								   /* �ͷ��ڴ� */
							     vPortFree(ucRxHexBuf);
								   return 1;
							 }
					 }
					 
			/* У����ж� */
			case eADROK:
				   usCalCHKsum = CHKsum(pRxData, len - 5); 
           usRxCHKsum = (ucRxHexBuf[usHexLen - 2] << 8) | ucRxHexBuf[usHexLen - 1];
			     if(usCalCHKsum == usRxCHKsum) 
					 {
					     ucState = eCHKOK;
					 }
					 else
					 {
						 	 FramStruct_st.VER = FramFrm_VER;
							 FramStruct_st.ADR = Optimset_st.ucComAddr;
						   FramStruct_st.CID1 = FramFrm_CID1;
						   RTN = eCHKError;
						   halfLenid = 0;
						   goto Tx;
					 }
			/* �汾�жϣ���ȡ�汾ʱ���ж� */
			case eCHKOK:
				   if(ucCommand == ePutVersion)
					 {
							FramStruct_st.VER = FramFrm_VER;
							FramStruct_st.ADR = Optimset_st.ucComAddr;
						  FramStruct_st.CID1 = FramFrm_CID1;	
              halfLenid = 0;
							goto Tx;						 
					 }
					 else
					 {
              if(ucRxHexBuf[0] == FramFrm_VER)
							{
							   ucState = eVEROK;
							}
							else
							{						  
								 FramStruct_st.VER = FramFrm_VER;
							   FramStruct_st.ADR = Optimset_st.ucComAddr;
						     FramStruct_st.CID1 = FramFrm_CID1;
							   RTN = eVERError;
								 halfLenid = 0;
								 goto Tx;
							}
					 }
					 
			/* ����У���ж� */
			case eVEROK:
				   usRxCHKsum = *(uint16_t *)&ucRxHexBuf[4];
					 usRxCHKsum = __REV16(usRxCHKsum);
					 usCalCHKsum = LCHKsum((usHexLen-8)*2);
			     if(usRxCHKsum == usCalCHKsum)
					 {					  						   
						   FramStruct_st.VER = FramFrm_VER;
							 FramStruct_st.ADR = Optimset_st.ucComAddr;
               FramStruct_st.CID1 = FramFrm_CID1;
						   if((ucCommand == eSetOptimset) || (ucCommand == eSetTime))
							 {
							    halfLenid = 0;
								  nSet_DataInfo(ucCommand, ucRxHexBuf, usHexLen - 9, &RTN);
								  goto Tx;
							 }
							 else
							 {
									 halfLenid = nTxDataInfo(ucRxHexBuf, (CID2TypeDef)ucCommand, &RTN);
								 
								   /* �� DATA_INFO����ӳ�䵽֡�ṹ��*/
		               FramStruct_st.INFO = DataInfo;	
									 goto Tx;	
							 }								 
					 }
					 else
					 {
						  FramStruct_st.VER = FramFrm_VER;
							FramStruct_st.ADR = Optimset_st.ucComAddr;
						  FramStruct_st.CID1 = FramFrm_CID1;
					    RTN = eLCHKError;
						  halfLenid = 0;
						  goto Tx;
					 }
			default: break;					 
		}			
		Tx:
		nTxFramHandle(halfLenid, RTN);
		
		/* �ͷ��ڴ� */
		vPortFree(ucRxHexBuf);
		return 0;
}



/**
  *@brief  Transmitt data_info
  *@param  pRxBuf--poiter of received fram
	*        RxLen --Length of received fram
  *@retval NONE
  *@note
  */
static uint16_t nTxDataInfo(const uint8_t *pRxData, CID2TypeDef CID2, RTNTypeDef  *RTN)
{	
	  uint16_t halfLenid;
	
		switch(CID2)
		{
			case ePutAddress: case ePutVersion:
					 halfLenid = 0;
					 break;
			case ePutAnalog:					   
					 halfLenid = nAnolog_DataInfo(pRxData[6], RTN);
					 break;
			case ePutIO:
					 halfLenid = nIO_DataInfo(pRxData[6], RTN);
					 break;
			case ePutTime:
           halfLenid = nPutTime_DataInfo();
					 break;
			case ePutOptimset:
				   halfLenid = nPutOpt_DataInfo(pRxData[6], RTN);
					 break;
			case ePutAlarm:
					 halfLenid = nRTAlm_DataInfo(pRxData[6], RTN);
					 break;
			case ePutPTAlarm:
				   halfLenid = nPTAlarm_DataInfo(pRxData[6], RTN);
					 break;
			case ePutPTEnergy:
				   halfLenid = nPTEnergy_DataInfo(pRxData[6], RTN);
					 break;
			case ePutDevice:
				   halfLenid = nDevice_DataInfo(); 
					 break;
			case ePutBrands:
				   halfLenid = nBrand_DataInfo();
					 break;
			default: break;						
		}
    return halfLenid;		
}



/**
  *@brief  handle transmmit fram  
	*@param  len--Length of fram to send
	*        RxLen --Length of received fram
	*        *pCommand -- received command_type
	*        *RTN -- protocol return
	*@retval 0--������������Ҫ����    1--���󣬽��մ�
	*@note
	*/ 
static void nTxFramHandle(const uint16_t len, RTNTypeDef RTN)
{
    uint16_t LENID, Length, usCHKsum;	  
	  uint8_t *TxAsciiBuf;
	
	  TxAsciiBuf = NULL;
	
	
	  /* HEX����ת��ΪASCII���� */
	  LENID = 2*len;
	
	  /* Ϊ�����͵�ASCII���������ڴ� */
	  TxAsciiBuf = (uint8_t *)pvPortMalloc(18 + LENID);
	  
	  /* ֡ͷ֡β��ֵ */
	  TxAsciiBuf[0] = FramStruct_st.SOI;
	  TxAsciiBuf[17 + LENID] =FramFrm_EOI;
	 
	/* CID2:������ */
	  FramStruct_st.CID2 = RTN;
	  
	  /* LENGTH */
	  Length = LCHKsum(LENID);
	  FramStruct_st.LENGTH = __REV16(Length);
   	
	  /* HEX תASCII */
	  HEX2ASCII(TxAsciiBuf+1, (uint8_t *)&FramStruct_st + 1, 6);		
		HEX2ASCII(TxAsciiBuf+1+12, (uint8_t *)FramStruct_st.INFO, len);
	  
	  /* CHKSUM */
	  usCHKsum = CHKsum(TxAsciiBuf, 13+LENID);
		FramStruct_st.CHKSUM = __REV16(usCHKsum);
		HEX2ASCII(TxAsciiBuf+13+LENID , (uint8_t *)&FramStruct_st.CHKSUM, 2);
		
		/* DMA��ʽ����		*/
		HAL_UART_Transmit_DMA(&hPC_UART, TxAsciiBuf, 18 + LENID);	
		
		/* �ͷ��ڴ� */
		vPortFree(DataInfo);
    vPortFree(TxAsciiBuf);

    return;		
}


/**
  *@brief anolog command respond--INFO
  *@param *pAnologData -- ָ����Ч�����׵�ַ
  *        Command --  COMMAND_GROUP  
  *                    0x00-ĸ�ߵ���
  *                    0x01-ĸ�߷�������
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = data_flag + dataf
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nAnolog_DataInfo(const uint8_t Command, RTNTypeDef *RTN)
{
	
	  uint8_t *pAnologData; 
	  uint16_t HalfLenid;	
	
	  DataInfo = NULL;
	  
    /* COMMAND_GROUP=00H����ȡĸ�ߵ���� */
	  if(Command == 0x00)
		{			
				/* DATA_INFO��HEX���ֽڳ��ȼ������׵�ַ */
				HalfLenid = 2*sizeof(struct Acq_BusVariableTypeDef) + 1;		
			  pAnologData =(uint8_t *)&Acq_BusVar_st_a;				
			
			/* ���붯̬�ڴ� */
				DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			
				/* DATA_INFO��ֵ */
				DataInfo[0] = ucPCProto_DataFlag;
				memcpy(DataInfo + 1, pAnologData, HalfLenid-1);	
		}
		
		/* COMMAND_GROUP=01H����ȡĸ�߷������� */
		else if(Command == 0x01)
		{
		    /* DATA_INFO��HEX���ֽڳ��ȼ������׵�ַ */
				HalfLenid = 2*sizeof(struct Ana_BusVariableTypeDef) + 1; 				
			  pAnologData =(uint8_t *)&Ana_BusVar_st_a;
			
						/* ���붯̬�ڴ� */
				DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			
				/* DATA_INFO��ֵ */
				DataInfo[0] = ucPCProto_DataFlag;
				memcpy(DataInfo + 1, pAnologData, HalfLenid-1);	
		}
		else
		{
			  HalfLenid = 0;
		    *RTN = eVoidData;
		}					  	

    return HalfLenid;
}


/**
  *@brief IO command respond--INFO
  *@param Command --  COMMAND_GROUP 
  *                   0xFF-���п���״̬
  *                   0x01-ĸ�߿���״̬
  *                   0x02-֧·����״̬
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note 1��data_info = data_flag + runstate
  *      2���ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nIO_DataInfo(const uint8_t Command, RTNTypeDef *RTN)
{
    uint16_t HalfLenid;
	
	  DataInfo = NULL;
	
    switch(Command)
		{		
			 /* ��ȡ���п���״̬ */
			 case 0xFF:		

					 HalfLenid = 4 + MAXBranchNum + 1;
				 
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
					
					 /* DATA_INFO��ֵ */
					 DataInfo[0] = ucPCProto_DataFlag;
           ucPCProto_DataFlag	&= 0xEF;		 
						
				   DataInfo[1] = (uint8_t)X.BusStatus_st_a[0].bQF;
			 
			     if(Optimset_st.Setup.Setup_st.bSPD == 1)
					 {
					    DataInfo[2] = (uint8_t)X.BusStatus_st_a[0].bSPD;
					 }
					 else
					 {
					    DataInfo[2] = 0x20;
					 }
									 
				   /* ˫·ĸ��ģʽ�£�2·ĸ�߿���+2·ĸ�߷���+144·֧·���� */
				   if(Optimset_st.Setup.Setup_st.bBusMode == BusModeS)
					 {						 
						 	 DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
						 
						   if(Optimset_st.Setup.Setup_st.bSPD == 1)
					     {
						       DataInfo[4] = (uint8_t)X.BusStatus_st_a[1].bSPD;
							 }
               else
					     {
					         DataInfo[4] = 0x20;
					     }		
							 memcpy(DataInfo + 5, ucBranchState_a, MAXBranchNum);
					 }
					 else
					 {
						   switch(Optimset_st.Setup.Setup_st.bBusMode)
							 {
								 case BusModeD:
									    DataInfo[3] = 0x20;
								      DataInfo[4] = 0x20;
								      break;
								 case BusModeH:
									    DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
								      DataInfo[4] = 0x20;
								      break;
								case BusModeY:
									    DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
								
											if(Optimset_st.Setup.Setup_st.bSPD == 1)
											{
													DataInfo[4] = (uint8_t)X.BusStatus_st_a[1].bSPD;
											}
											else
											{
												 DataInfo[4] = 0x20;
											}
								      break;
								default:
									    break;								 
							 } 
							 memcpy(DataInfo + 5, ucBranchState_a, MAXBranchNum);
				 }
				 break;
				 
			/* ��ȡĸ�߿��ؼ�����״̬ */
      case 0x01:
					HalfLenid = 4 + 1;
			 
					/* ���붯̬�ڴ� */
					DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
				
					/* DATA_INFO��ֵ */
					DataInfo[0] = ucPCProto_DataFlag;
			    ucPCProto_DataFlag	&= 0xEF;		
						
				   DataInfo[1] = (uint8_t)X.BusStatus_st_a[0].bQF;
			 
			     if(Optimset_st.Setup.Setup_st.bSPD == 1)
					 {
					    DataInfo[2] = (uint8_t)X.BusStatus_st_a[0].bSPD;
					 }
					 else
					 {
					    DataInfo[2] = 0x20;
					 }
									 
				   /* ˫·ĸ��ģʽ�£�2·ĸ�߿���+2·ĸ�߷���+144·֧·���� */
				   if(Optimset_st.Setup.Setup_st.bBusMode == BusModeS)
					 {						 
						 	 DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
						 
						   if(Optimset_st.Setup.Setup_st.bSPD == 1)
					     {
						       DataInfo[4] = (uint8_t)X.BusStatus_st_a[1].bSPD;
							 }
               else
					     {
					         DataInfo[4] = 0x20;
					     }		
					 }
					 else
					 {
						   switch(Optimset_st.Setup.Setup_st.bBusMode)
							 {
								 case BusModeD:
									    DataInfo[3] = 0x20;
								      DataInfo[4] = 0x20;
								      break;
								 case BusModeH:
									    DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
								      DataInfo[4] = 0x20;
								      break;
								case BusModeY:
									    DataInfo[3] = (uint8_t)X.BusStatus_st_a[1].bQF;
								
											if(Optimset_st.Setup.Setup_st.bSPD == 1)
											{
													DataInfo[4] = (uint8_t)X.BusStatus_st_a[1].bSPD;
											}
											else
											{
												 DataInfo[4] = 0x20;
											}
								      break;
								default:
									    break;								 
							 } 
				  }
				  break;
			/* ��ȡ֧·����״̬ */
      case 0x02:
				
					HalfLenid = MAXBranchNum + 1;
			 
					/* ���붯̬�ڴ� */
					DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
				
					/* DATA_INFO��ֵ */
					DataInfo[0] = ucPCProto_DataFlag;
          ucPCProto_DataFlag	&= 0xEF;	
		    
          memcpy(DataInfo+1, ucBranchState_a, MAXBranchNum);
			
					break;
			default:
				   HalfLenid = 0;
				   *RTN = eVoidData;
			     break;					 
		}
		return HalfLenid;   
}
	
/**
  *@brief alarm command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *                   0xFF-���и澯״̬
  *                   0x01-ĸ�߸澯״̬
  *                   0x02-֧·�澯״̬
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = data_flag + dataf
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nRTAlm_DataInfo(const uint8_t Command, RTNTypeDef *RTN)
{
    uint16_t HalfLenid;
	
	  DataInfo = NULL;

    switch(Command)
		{
			/* ���и澯 */
			case 0xFF:
				   HalfLenid = sizeof(gAlmMessage.BranchAlm) + sizeof(gAlmMessage.BusAlm) +1;
				
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
				
					 /* DATA_INFO��ֵ */
					 DataInfo[0] = ucPCProto_DataFlag;
				   ucPCProto_DataFlag	&= 0xFE;
			
           memcpy(DataInfo + 1, (uint8_t *)&gAlmMessage, sizeof(gAlmMessage));	    						 
			    break;
			
			/* ĸ�߸澯 */
			case 0x01:
					 HalfLenid =sizeof(gAlmMessage.BusAlm) +1;
			
			     /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			
			     /* DATA_INFO��ֵ */
					 DataInfo[0] = ucPCProto_DataFlag;
			     ucPCProto_DataFlag	&= 0xFE;
			
			     memcpy(DataInfo + 1, (uint8_t *)&gAlmMessage.BusAlm, sizeof(gAlmMessage.BusAlm));			    
			     break;
			
			/* ֧·�澯 */
			case 0x02:
					 HalfLenid =sizeof(gAlmMessage.BranchAlm) +1;
			
			     /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			
			     /* DATA_INFO��ֵ */
					 DataInfo[0] = ucPCProto_DataFlag;
			     ucPCProto_DataFlag	&= 0xFE;
							
					 memcpy(DataInfo + 1, (uint8_t *)&gAlmMessage.BranchAlm, sizeof(gAlmMessage.BranchAlm));
			     break;
			default:
				    HalfLenid = 0;
			      *RTN = eVoidData;
			      break;			  
		}
    return HalfLenid;		
}


/**
  *@brief output optimset command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *                   0xD0-��ȡ��λ��ͨ�Ų���
  *                   0xE0-��ȡĸ�߸澯��ֵ
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = data_flag + dataf
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nPutOpt_DataInfo(const uint8_t Command, RTNTypeDef *RTN)
{
	  uint16_t HalfLenid, temp;
	
	  DataInfo = NULL;
	
	  /* ��ȡ��λ��ͨ�Ų��� */
	  if(Command == 0xD0)
		{
		    HalfLenid = sizeof(Optimset_st.usComBaud) + sizeof(Optimset_st.ucComAddr);
			
			  /* ���붯̬�ڴ� */
			  DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			  
			  DataInfo[0] = Optimset_st.ucComAddr;
			  temp = __REV16(Optimset_st.usComBaud);
			  memcpy(DataInfo + 1, (uint8_t *)&temp, sizeof(temp));
		}
		else if(Command == 0xE0)
		{
		    HalfLenid = sizeof(Optimset_st.usOVThr) + sizeof(Optimset_st.usUVThr) 
					          + sizeof(Optimset_st.ucOCThrFst) + sizeof(Optimset_st.ucOCThrSec)
										+ sizeof(Optimset_st.usOPThr) + sizeof(Optimset_st.usOFThr)
										+ sizeof(Optimset_st.usUFThr);
										
				/* ���붯̬�ڴ� */
			  DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
										
				temp = __REV16(Optimset_st.usOVThr);
				memcpy(DataInfo, (uint8_t *)&temp, sizeof(temp));
				
		    temp = __REV16(Optimset_st.usUVThr);
				memcpy(DataInfo + 2, (uint8_t *)&temp, sizeof(temp));
				
        DataInfo[4] = Optimset_st.ucOCThrFst;
        DataInfo[5] = Optimset_st.ucOCThrSec;
   
        temp = __REV16(Optimset_st.usOPThr[0]);
        memcpy(DataInfo + 6, (uint8_t *)&temp, sizeof(temp));

        temp = __REV16(Optimset_st.usOPThr[1]);
        memcpy(DataInfo + 8, (uint8_t *)&temp, sizeof(temp));

        temp = __REV16(Optimset_st.usOFThr);
				memcpy(DataInfo + 10, (uint8_t *)&temp, sizeof(temp));
				
				temp = __REV16(Optimset_st.usUFThr);
				memcpy(DataInfo + 12, (uint8_t *)&temp, sizeof(temp));		
		}
		else
		{
		    HalfLenid = 0;
			  *RTN = eVoidData;
		}
		return HalfLenid;
}

/**
  *@brief  ����λ����ȡ���ò���
  *@param  command--����
  *                 0--��ȡʱ��
	*					        D0--��ַ
	*					        D1--������
	*					        E0--��ѹ��ֵ
	*					        E1--Ƿѹ��ֵ
	*					        E2--һ������
	*									E3--��������
	*									E4--Aĸ�߹���
	*									E5--Bĸ�߹���
	*									E6 --��Ƶ
	*					  			E7--ǷƵ
  *        *data--����
  *        len--���ݳ���
  *        *RTN -- ָ����Ӧ�����CID2
  *@retval None
  *@note
  */
static void nSet_DataInfo(const uint8_t command, const uint8_t* data, const uint16_t len, RTNTypeDef *RTN)
{
	  uint8_t  CommadCase;
	  uint16_t temp;
	
	  RTC_DateTypeDef   sDate;
	  RTC_TimeTypeDef   sTime;
	
	 if(command == eSetTime)
	 {
	    CommadCase = 0x00;
	 }
	 else
	 {
	    CommadCase = data[6];
	 }
	
    switch(CommadCase)
		{
			
			case 0x00:
				   if(len == 7) 
					 {
					     temp = *(uint16_t *)(data +6);
               temp = __REV16(temp);
						  						 
						   if((temp <= 2099) && (temp >= 2000))
							 {
									 sDate.Year = temp-2000;
									 sDate.Month = data[8];
									 sDate.Date = data[9];
								   sDate.WeekDay = 0x00;
									 HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

									 sTime.Hours = data[10];
									 sTime.Minutes = data[11];
									 sTime.Seconds = data[12];
								   sTime.TimeFormat = RTC_HOURFORMAT12_AM;
		               sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		               sTime.StoreOperation = RTC_STOREOPERATION_RESET;
									 HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);	
							 }
               else
							 {
							     *RTN = eVoidData;
							 }								 
					 }
           else
					 {
					    *RTN = eOFError;
					 }						 
			     break;
			case 0xD0:
				   if(len == 1)
					 {
				      Optimset_st.ucComAddr = data[7];
						  Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
			     break;
			case 0xD1:
				   if(len == 2)
					 {
							 temp = *(uint16_t *)(data+7);
							 temp = __REV16(temp);
							 if((temp==1200)||(temp==2400)||(temp==4800)||(temp==9600)||(temp==19200)||(temp==38400))
							 {
									Optimset_st.usComBaud = temp;
									Opt_Save();
							 }
							 else
							 {
									*RTN = eVoidData;
							 }
					 }
					 else
					 {
					    *RTN = eOFError;
					 }				   
			     break;
			case 0xE0:
				   if(len == 2)
					 {
					    temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usOVThr = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }				
			     break;			
			case 0xE1:
           if(len == 2)
					 {
					    temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usUVThr = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }						 
           break;
      case 0xE2:
           if(len == 1)
					 {
					     if(data[0] <= 100)
							 {
							     Optimset_st.ucOCThrFst = data[7];
					         Opt_Save();
							 }
							 else
							 {
							    *RTN = eVoidData;
							 }
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;
      case 0xE3:
           if(len == 1)
					 {
					     if(data[0] <= 100)
							 {
							     Optimset_st.ucOCThrSec = data[7];
					         Opt_Save();
							 }
							 else
							 {
							    *RTN = eVoidData;
							 }
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;

      case 0xE4:
           if(len == 2)
					 {
					    temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usOPThr[0] = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;
      case 0xE5:
           if(len == 2)
					 {
					    temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usOPThr[1] = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;			
			case 0xE6:
           if(len == 2)
					 {
							temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usOFThr = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;
      case 0xE7:
           if(len == 2)
					 {
							temp = *(uint16_t *)(data+7);
							temp = __REV16(temp);
						  Optimset_st.usUFThr = temp;
					    Opt_Save();
					 }
					 else
					 {
					    *RTN = eOFError;
					 }
           break;
      default:break;
		}
		
		return;
}


/**
  *@brief past-time-energy command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *                   0x00-��ȡ����һ����ʷ��¼
  *                   0x01-��ȡ��ʷ������ȷ��Ҫ��������һ��
  *                   0x02-��ȡ��ʷ���ݴ��� �ط���һ��
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = data_flag + dataf
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nPTEnergy_DataInfo(uint8_t Command,RTNTypeDef *RTN)
{
    PTEnergyTypeDef   PTEnergy;
		
	  uint16_t HalfLenid;
	  static uint16_t SendIndex = 0;
		uint8_t i;
		uint32_t PTE_temp;
	
	  DataInfo = NULL;
	
	  switch(Command)
		{
			case 0x00:
				   HalfLenid = 39;
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
					 
					 SendIndex = gPTE_FTime();
			     break;
			case 0x01:
			      HalfLenid = 39;
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
					 
				   if(SendIndex > usPTEBaseTime - 1)
					 {
					    if((SendIndex & 0x00FF) == 0x0001)
							{
							   SendIndex = ((SendIndex & 0xFF00) - 0x0100) | 0x0C;
							}
							else
							{
							    SendIndex = SendIndex - 1;
							}
					 }
			     break;
			case 0x02:
			     HalfLenid = 39;
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);			
			     break;
			default:
			      HalfLenid = 0;
						*RTN = eVoidData;
						break;
		}
		
		if(SendIndex <= usPTEBaseTime + 1)
		{
		   DataInfo[0] = 0x01;
		}
		else
		{
		   DataInfo[0] = 0x00;
		}
		
		DataInfo[1] = ucPCProto_DataFlag;
		
		if(SendIndex > usPTEBaseTime)
		{
		   gPTE_Query(usPTEBaseTime, SendIndex, &PTEnergy);
		}
		else
		{
			gPTE_Query(usPTEBaseTime, usPTEBaseTime+1, &PTEnergy);
		}		
					
		DataInfo[2] = (uint8_t)PTEnergy.Time;
		DataInfo[3] = 0x01;
		DataInfo[4] = 0x00;
		DataInfo[5] = 0x00;
		DataInfo[6] = 0x00;
       
	 /* S�ͻ�Y�� */
	 if((Optimset_st.Setup.Setup_st.bBusMode == 1) || (Optimset_st.Setup.Setup_st.bBusMode == 8))
	 {
			 for(i = 0; i < 8; i ++)
			 {
					 PTE_temp = __REV16(PTEnergy.EP[i]);
					 memcpy(DataInfo+7+i*4, (uint8_t *)&PTE_temp, sizeof(PTE_temp));
			 }
	 }
	 else /* D�ͻ�H�� */
	 if((Optimset_st.Setup.Setup_st.bBusMode == 2) || (Optimset_st.Setup.Setup_st.bBusMode == 4))
	 {
			 for(i = 0; i < 4; i++)
			 {
					 PTE_temp = __REV16(PTEnergy.EP[i]);
					 memcpy(DataInfo+7+i*4, (uint8_t *)&PTE_temp, sizeof(PTE_temp));
			 }
			 memset(DataInfo + 7 + 16, 0x20, 4*sizeof(PTE_temp));
		}
		return HalfLenid;
}



/**
  *@brief past-time-alarm command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *                   0x00-��ȡ����һ����ʷ�澯
  *                   0x01-��ȡ��ʷ�澯��ȷ��Ҫ��������һ��
  *                   0x02-��ȡ��ʷ�澯���� �ط���һ��
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = datatype + data_flag + datatime + warnstate
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nPTAlarm_DataInfo(uint8_t Command, RTNTypeDef *RTN)
{
    static AlmRecordTypeDef  fPTAlmRecord;
		
    uint16_t HalfLenid, FM_OperateAddr;
    static uint8_t lastFlag = 0x00, noneFlag = 0x00; //���һ���澯��־
	  static uint16_t sendIndex = 0;
	  
	
	  DataInfo = NULL;
	
	  switch(Command)
		{
			case 0x00:
				   HalfLenid = 9;
	  
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
			
			     if(gPTAlmRelate.AlmIndex > 0)
					 {
					    sendIndex = gPTAlmRelate.AlmIndex - 1;
					 }
					 else if(gPTAlmRelate.AlmIndex == 0x00)
					 {
					    if(gPTAlmRelate.FullFlag == 0x01)
							{
							   sendIndex = PTAlm_Item - 1;
							}
							else
							{
							    lastFlag = 0x01;	
                  noneFlag = 0x01;							  
							}
					 }
					 
					 if(noneFlag == 0x01)
					 {
						  memset(DataInfo+2, 0, 7);
					 }
					 else
					 {
						  FM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + sendIndex * sizeof(fPTAlmRecord);
							
							taskENTER_CRITICAL();
						  Fram_I2C_Read((uint8_t *)&fPTAlmRecord, FM_OperateAddr, sizeof(fPTAlmRecord));
							taskEXIT_CRITICAL();
							
							memcpy(DataInfo + 2, (uint8_t *)&fPTAlmRecord.GenerateMonth, 5);
							DataInfo[7] = fPTAlmRecord.AlmEvent;
							DataInfo[8] = fPTAlmRecord.AlmType;
					 }
			     break;
			case 0x01:
					 HalfLenid = 9;
	  
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
					 
					 /* �Ѿ����������Ѿ��洢 */
					 if(fPTAlmRecord.flag == TerminateFlag)
					 {
					     memcpy(DataInfo + 2, (uint8_t *)&fPTAlmRecord.TerminateMonth, 5);
							 DataInfo[7] = fPTAlmRecord.AlmEvent;
							 DataInfo[8] = fPTAlmRecord.AlmType;
							 fPTAlmRecord.flag = 0;
					 }
					 else
					 {				 			     
							 if(sendIndex > 0)
							 {
									sendIndex-=1;
									if((gPTAlmRelate.FullFlag == 0x00) && (sendIndex == 0))
									{
										 lastFlag = 0x01;
									}
									if(sendIndex == gPTAlmRelate.AlmIndex)
									{
										 lastFlag = 0x01;
									}  
							 }
							 else if((gPTAlmRelate.FullFlag == 0x01) && (sendIndex == 0))
							 {
									sendIndex = PTAlm_Item - 1;
							 }
							 
							 FM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + sendIndex * sizeof(fPTAlmRecord);
							 
							 taskENTER_CRITICAL();
						   Fram_I2C_Read((uint8_t *)&fPTAlmRecord, FM_OperateAddr, sizeof(fPTAlmRecord));
							 taskEXIT_CRITICAL();
							 
							 memcpy(DataInfo + 2, (uint8_t *)&fPTAlmRecord.GenerateMonth, 5);
							 DataInfo[7] = fPTAlmRecord.AlmEvent;
							 DataInfo[8] = fPTAlmRecord.AlmType;
					 }

			     break;
			case 0x02:
					 HalfLenid = 9;	  
					 /* ���붯̬�ڴ� */
					 DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
					 
					 if(noneFlag == 0x01)
					 {
						  memset(DataInfo+2, 0, 7);
					 }
					 else
					 {
						  FM_OperateAddr = FMAddr_PastAlm + FMOffset_AlmRecord + sendIndex * sizeof(fPTAlmRecord);
							
							taskENTER_CRITICAL();
						  Fram_I2C_Read((uint8_t *)&fPTAlmRecord, FM_OperateAddr, sizeof(fPTAlmRecord));
							taskEXIT_CRITICAL();
							
							memcpy(DataInfo + 2, (uint8_t *)&fPTAlmRecord.GenerateMonth, 5);
							DataInfo[7] = fPTAlmRecord.AlmEvent;
							DataInfo[8] = fPTAlmRecord.AlmType;
					 }
			     break;
			default:
				   HalfLenid = 0;
			     *RTN = eVoidData;
			     break;				
		}
		DataInfo[0] = lastFlag;
		DataInfo[1] = ucPCProto_DataFlag;

	  return HalfLenid;	  
}

/**
  *@brief device command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = datatype + data_flag + datatime + warnstate
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nDevice_DataInfo(void)
{
    uint16_t  HalfLenid, temp;
		uint8_t   i;
	
	  DataInfo = NULL;
	  
	  HalfLenid = 7;
	
	  /* ���붯̬�ڴ� */
		DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
	
	  DataInfo[0] = Optimset_st.Setup.ucSetup;
		
		for(i = 0; i < 2; i ++)
		{
			 temp = __REV16(Optimset_st.usRateI[i]);
			 memcpy(DataInfo+1+i*2, (uint8_t *)&temp, sizeof(temp));
		}
	  memcpy(DataInfo + 1 + sizeof(Optimset_st.usRateI), (uint8_t *)&Optimset_st.ucBranchN_a, sizeof(Optimset_st.ucBranchN_a));
	
	  return HalfLenid;
}

/**
  *@brief put-time command respond--INFO
  *@param Command --  COMMAND_GROUP  
  *       *RTN -- ָ����Ӧ�����CID2
  *@retval protocol LENID
  *@note data_info = datatime
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nPutTime_DataInfo(void)
{
	  RTC_DateTypeDef   sDate;
	  RTC_TimeTypeDef   sTime;
	
    uint16_t  HalfLenid, temp;
	
	  DataInfo = NULL;
	
	  HalfLenid = 7;
	
	  /* ���붯̬�ڴ� */
		DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
	
	  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	  temp = sDate.Year + 2000;
	  temp = __REV16(temp);
	  memcpy(DataInfo, (uint8_t *)&temp, sizeof(temp));
	
	  DataInfo[2] = sDate.Month;
	  DataInfo[3] = sDate.Date;
		DataInfo[4] = sTime.Hours;
		DataInfo[5] = sTime.Minutes;
		DataInfo[6] = sTime.Seconds;
		
		return HalfLenid;	
}
	

/**
  *@brief brand command respond--INFO
  *@param NONE
  *@retval protocol LENID/2
  *@note data_info = datatime
  *      �ô�������ڴ��ת��ΪASCII����ͷ�
  */
static uint16_t nBrand_DataInfo(void)
{
    uint16_t  HalfLenid, temp;
	
	  DataInfo = NULL;
	  
	  HalfLenid = 32;
	   
	  /* ���붯̬�ڴ� */
		DataInfo = (uint8_t *)pvPortMalloc(HalfLenid);
	
	  memcpy(DataInfo, ucSYSName, sizeof(ucSYSName));
		temp = __REV16(ucSoftVer);
	  memcpy(DataInfo + sizeof(ucSYSName), (uint8_t *)&temp, sizeof(ucSoftVer));
	  memcpy(DataInfo + sizeof(ucSYSName) +sizeof(ucSoftVer), ucBrands, sizeof(ucBrands));
     
    return HalfLenid;	
}



/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************/






