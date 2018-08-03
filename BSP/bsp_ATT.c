/**
  ******************************************************************************
  * @file    bsp_ATT.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    21-May-2018
  * @brief   SPI2 configuration and ATT options   
  * @endverbatim
  ******************************************************************************
  */


/*----------------------------------校表参数------------------------------------/
 *Hfconst=(u32)((2.592*10000000000*G1*G1*Vi*Vu)/(3200*220*5));
 *Kp=((2.592*10000000000)/8388608)/(Hfconst*3200);
 *其中 Ec=3200，G1=1.163，Vu=0.12，Vi=0.051，Un=220，In=5;，Hfconst， Kp;
/-----------------------------------------------------------------------------*/


/* Includes -----------------------------------------------------------------------------------------*/
#include "bsp_ATT.h"
#include "bsp_GPIO.h"
#include "bsp_Fram.h"
#include "sys_FramAlloc.h"
#include "cmsis_os.h"

/* private type definitions -------------------------------------------------------------------------*/
/* private macro definitions ------------------------------------------------------------------------*/
/* private variables ---------------------------------------------------------------------------------*/

SPI_HandleTypeDef    hDA_SPI;
static uint8_t ucCalParam[110];

//static uint8_t calibration_Data[110]=
//{0xFF,0xB9,0x00,0x00,0x04,0xFC,0xAA,0x12,0x6F,0x13,0x3C,0x1B,0xAA,0x12,0x6F,0x13,0x3C,0x1B,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0xDF,0x56,0xD9,0x57,0x22,0x63,0x3E,0x8B,0x42,0x8B,0x43,0x8B,0x60,0x01,0x3C,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x01,0x00,0x37,0x34,0x00,0x00,0x00,0x00,0x52,0x2C,0x0F,0x00,0x30,0x00,0xFF,0x7F};

/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/
/* function body -------------------------------------------------------------------------------------*/

/**
  *@brief  ATT7022E SPI init
  *@param  NONE
  *@retval NONE
  *@note 
  */
static void DA_SPIx_Init(void)
{
    hDA_SPI.Instance = DA_SPIx;
    hDA_SPI.Init.Mode = SPI_MODE_MASTER;
    hDA_SPI.Init.Direction = SPI_DIRECTION_2LINES;
    hDA_SPI.Init.DataSize = SPI_DATASIZE_8BIT;
    hDA_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;
    hDA_SPI.Init.CLKPhase = SPI_PHASE_2EDGE;
    hDA_SPI.Init.NSS = SPI_NSS_SOFT;
    hDA_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hDA_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hDA_SPI.Init.TIMode = SPI_TIMODE_DISABLE;
    hDA_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hDA_SPI.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&hDA_SPI);
}

/**
  *@breif  write and receive a byte 
  *@param  byte--byte to write
  *@retval byte to read
  *@note NONE
  */
static uint8_t DA_SPI_WriteRead(uint8_t ucByte)
{
   uint8_t ucRdata = 0,ucWdata = ucByte;
	
	 if(HAL_SPI_TransmitReceive(&hDA_SPI, &ucWdata, &ucRdata, 1, 0xFFFFFF) != HAL_OK)
	 {
	     ucRdata = DUMMY_BYTE;
	 }
	 
	 return ucRdata; 
}


/**
  *@brief  ATT7022E CS Enable
  *@param  ucNum--ATT7022E number(from 0)
  *@retval NONE
  *@note   NONE
  */
static void DA_ATT_Enable(uint8_t ucNum)
{
   switch(ucNum)
	 {
		 case 0: DA_ATT1_CS_ENABLE();
	           break;
		 case 1: DA_ATT2_CS_ENABLE();
		         break;
		 default:break;
	 }
}


/**
  *@brief  ATT7022E CS Disable
  *@param  ucNum--ATT7022E number(from 0)
  *@retval NONE
  *@note   NONE
  */
static void DA_ATT_Disable(uint8_t ucNum)
{
   switch(ucNum)
	 {
		 case 0: DA_ATT1_CS_DISABLE();
	           break;
		 case 1: DA_ATT2_CS_DISABLE();
		         break;
		 default:break;
	 }
}


/**
  *@brief  read and check ATT reg data
  *@param  ucNum--ATT7022E number(from 0)
  *        ucRegAddr--ATT reg adress
  *        ucRdata--ATT reg data
  *@retval 1--fail, 0--succeed
  *@note
  */
static uint8_t DA_ATT_ReadReg(uint8_t ucNum, uint8_t ucRegAddr, uint8_t *ucRdata)
{
   uint32_t uiCheckSum=0;
	 uint32_t uiReadSum;
	 uint8_t ucReadSum[3];
	 int8_t  i;
	
	 DA_ATT_Enable(ucNum);
	 DA_SPI_WriteRead(ucRegAddr);
	
	 for(i = 2; i >=0; i --)
	 {
		  ucRdata[i] = DA_SPI_WriteRead(DUMMY_BYTE);
		  uiCheckSum += ucRdata[i];
	 }	 

	 DA_SPI_WriteRead(r_ComChksum);
	 for(i = 0; i < 3; i ++)
	 {
	    ucReadSum[i] = DA_SPI_WriteRead(DUMMY_BYTE);
	 }
	 DA_ATT_Disable(ucNum);
	 
	 if(ucReadSum[0] == ucRegAddr)
	 {
	    uiReadSum = ((ucReadSum[1] << 8)|ucReadSum[2]);
		  uiCheckSum += ucRegAddr;
		  if(uiCheckSum == uiReadSum)
			{
			   return 0;
			}
			else
			{
			   return 1;
			}
	 }
	 
	 return 1;
}





/**
  *@brief  write and check ATT reg data
  *@param  ucNum--ATT7022E number(from 0)
  *        ucRegAddr--ATT reg adress
  *        ucWdata--data write to ATT reg
  *@retval 1--fail, 0--succeed
  *@note
  */
static uint8_t DA_ATT_WriteReg(uint8_t ucNum, uint8_t ucRegAddr, uint8_t *ucWdata)
{
	 uint32_t uiCheckSum=0;
	 uint32_t uiReadSum;
	 uint8_t ucReadSum[3];
	 int8_t  i;
	
	 DA_ATT_Enable(ucNum);
	 DA_SPI_WriteRead(ucRegAddr);
	 for(i = 2; i >= 0; i --)
	 {
	    DA_SPI_WriteRead(ucWdata[i]);
		  uiCheckSum += ucWdata[i];
	 }
	 
	 DA_SPI_WriteRead(r_ComChksum);
	 for( i = 0; i < 3; i ++)
	 {
	    ucReadSum[i] = DA_SPI_WriteRead(DUMMY_BYTE);
	 }
	 DA_ATT_Disable(ucNum);
	 
	 if(ucReadSum[0] == ucRegAddr)
	 {
	    uiReadSum = ((ucReadSum[1] << 8) | ucReadSum[2]);
		  uiCheckSum += ucRegAddr;
		  if(uiReadSum == uiCheckSum)
			{
			   return 0;
			}
			else
			{
			   return 1;
			}		  
	 }
	 
	 return 1;  
}

/** 
  *@brief  ATT7022E init
  *@param  NONE
  *@retval 0--succeed, 1--fail
  *@note  初始化SPI，写入校准参数，配置电能寄存器为读后清零模式
  *       读取芯片ID，检查ATT是否能正常读写
  *       Device:     ATT7022E      ATT7026E     ATT7028E
  *         ID  :     0x7122A0      0x7126A0     0x7128A0
  */
uint8_t DA_ATT_Init(void)
{
   uint32_t uiFM_OperateAddr,uiCheckSum[2],uiReadSum;
	 uint8_t ucRegAddr = 0x81;
	 uint32_t uiRegCache = 0;
	 uint8_t i,j,k,ucConter = 0;
	
	 DA_SPIx_Init();
	 HAL_Delay(5);
	 DA_ATT1_RESET_DISABLE();
	 DA_ATT2_RESET_DISABLE();	
	
	 for(j = 0; j < 2; j++)
	 {
	    uiReadSum = 0xFF;
		  if(j == 0)
			{
			    uiFM_OperateAddr = FMAddr_ATT1_CalData;
			}
			else
			{
			    uiFM_OperateAddr = FMAddr_ATT2_CalData;
			}				
			
			/* 读取ATT ID */
			DA_ATT_ReadReg(j, 0x00, (uint8_t *)&uiRegCache);
			
			/* 写操作使能 */
			uiRegCache = 0x00005A;
			DA_ATT_WriteReg(j,0xC9, (uint8_t*)&uiRegCache);
			
			while(uiCheckSum[j] != uiReadSum)
			{
			   uiCheckSum[j] = 0;
				 				 
				 Fram_I2C_Read(ucCalParam,uiFM_OperateAddr, 110);				 
				 
				 for(i = 0; i < 0x37; i ++)
				 {
				    uiRegCache = ((ucCalParam[i*2+1]<<8) | ucCalParam[i*2]);  
					  for(k = 0; k < 5; k++)
					  {
						   if(DA_ATT_WriteReg(j, ucRegAddr + i, (uint8_t*)&uiRegCache) == 0)
							 {
							    break;
							 }
						}
            uiCheckSum[j] += uiRegCache;         //计算校准参数校验和					 
				 }
				 for(k = 0; k < 5 ; k ++)
				 {
				    if(DA_ATT_ReadReg(j, 0x3E, (uint8_t*)&uiRegCache) == 0)
						{
						   uiReadSum = uiRegCache;          //读取校准参数校验和
							 break;
						}
				 }
				 uiCheckSum[j] += 0x5500;
				 ucConter ++;
				 if(ucConter == 5)
				 {
				    ucConter = 0;
					  break;;
				 }				 
			}
			/* 配置ATT电能为读后清零 */
			uiRegCache = 0x00FC84;			
			DA_ATT_WriteReg(j, 0x83, (uint8_t*)&uiRegCache);
			
			/* ATT写失能 */
			uiRegCache = 0x000000;
			DA_ATT_WriteReg(j, 0xC9, (uint8_t*)&uiRegCache);
	 }
	 return 0;
}

/**
  *@brief  ATT calibration data check
  *@param  ucNum--ATT number
  *@retval NONE
  *@note   检查ATT内部校准参数是否错误，默认铁电内部校准参数正确
  */
void DA_ATT_Check(uint8_t ucNum)
{
    uint32_t uiFM_OperateAddr, uiRegCache = 0, uiReadSum=0, uiCheckSum=0;
	  uint8_t i, k, ucRegAddr = 0x81;
	
	
	  /* 计算校准参数校验和 */
	  if(ucNum == 0)
		{
		    uiFM_OperateAddr = FMAddr_ATT1_CalData;
		}
		else
		{
		    uiFM_OperateAddr = FMAddr_ATT2_CalData;
		}
		uiCheckSum = 0;

    taskENTER_CRITICAL();
		Fram_I2C_Read(ucCalParam, uiFM_OperateAddr, 110);
		taskEXIT_CRITICAL();
		
		for(i = 0; i < 0x37; i++)
		{
			 uiRegCache = ((ucCalParam[i*2+1]<<8) | ucCalParam[i*2]);
		   uiCheckSum += uiRegCache;
		}
		
		/*读取校准参数校验和*/
		for(k = 0; k < 5; k ++)
		{
		   if(DA_ATT_ReadReg(ucNum, 0x3E, (uint8_t*)&uiRegCache) == 0)
			 {
			     uiReadSum = uiRegCache;
				   break;
			 }
		}
		
		/* 比较检查 */
		if((uiCheckSum + 0x5500) != uiReadSum-0x0080)
		{
			
			  /* 写操作使能 */
			  uiRegCache = 0x00005A;
			  DA_ATT_WriteReg(ucNum,0xC9, (uint8_t*)&uiRegCache);
			
		    for(i = 0; i < 0x37; i++)
		    {
					   uiRegCache = ((ucCalParam[i*2+1]<<8) | ucCalParam[i*2]);  
					   for(k = 0; k < 5; k++)
					   {
						    if(DA_ATT_WriteReg(ucNum, ucRegAddr + i, (uint8_t*)&uiRegCache) == 0)
							  {
							     break;
							  }
						 }
				}
				
				/* 配置ATT电能为读后清零 */
			  uiRegCache = 0x00FC84;			
			  DA_ATT_WriteReg(ucNum, 0x83, (uint8_t*)&uiRegCache);
				
				/* ATT写失能 */
			  uiRegCache = 0x000000;
			  DA_ATT_WriteReg(ucNum, 0xC9, (uint8_t*)&uiRegCache);			
		}		
}

/**
  *@brief  read RMS from ATT
  *@param  ucNum--ATT number
  *        ucRegAddr--ATT Reg address
  *          This parameter can be one of the following values:
  *          r_UaRms, r_UbRms, r_UcRms, r_IaRms, r_IbRms, r_IcRms, r_ItRms
  *          r_I0Rms, r_UtRms, r_LineUaRrms, r_LineUbRrms, r_LineUcRrms, 
  *          r_LineIaRrms, r_LineIbRrms, r_LineIcRrms 
  *@return   RMS value
  */
float DA_ATT_RMSRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{
   uint8_t i;
   float fRMS;
	 uint32_t uiRegCache = 0;
	
	 assert_param(IS_ATT_ALL_REG(ATTReg));
	
	 for(i = 0; i < 5; i ++)
	 {
	     if(DA_ATT_ReadReg(ucNum, ATTReg, (uint8_t*)&uiRegCache) == 0)
			 {
			    fRMS = (float)(uiRegCache)/8192;
				  break;
			 }
	 } 
	 if(i == 5)
		 return 0;
	 else
		 return fRMS;
}


/**
  *@breif  read power from ATT,Unit:VA/W/Var
  *@param  ucNum--ATT number
  *        ATTReg--reg address
  *          This parameter can be one of the following values:
  *          r_Pa, r_Pb, r_Pc, r_Pt, r_Qa, r_Qb, r_Qc, r_Qt, r_Sa,
  *          r_Sb, r_Sc, r_St, r_LinePa, r_LinePb, r_LinePc, r_LinePt
  *@retval  power value
  *@note  
  */

//uint16_t Ec = 3200;
//	 float G = 1.163;
//	 float Vu = 0.12;
//	 float Vi = 0.051;
//	 uint8_t Un = 220;
//	 uint8_t In = 5;

float DA_ATT_PowerRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{	   
	 double Kp ;
	 	
   uint8_t i;	 
	 float fPower;
	 uint32_t uiRegCache = 0,uiTemp = 0;
	
	 Kp = 0.016093254;
	 assert_param(IS_ALL_ATT_REG(ATTReg));
	
	 for(i = 0; i < 5; i ++)
	 {
	    if(DA_ATT_ReadReg(ucNum, ATTReg, (uint8_t *)&uiRegCache) == 0)
			{
				 uiTemp = uiRegCache;
         if(uiRegCache&0x00800000)	
         {
						 uiTemp=~uiTemp+1;
						 uiTemp=uiTemp&0x00FFFFFF;
         }					 
				 fPower = (float)(uiTemp) * Kp;
				 break;
			}		
	 }
	 if(i == 5)  
		 return 0;
	 else
		 return fPower;
}

/**
  *@brief   read power factor from ATT
  *@param   ucNUM--ATT number
  *         ATTReg--address
  *          This parameter can be one of the following values:
  *          r_Pfa, r_Pfb, r_Pfc, r_Pft
  *@retval   Power factor value
  *@note
  */
float DA_ATT_PFRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{
   uint8_t i;
	 uint32_t uiRegCache = 0;
	 float fPF;
	
	 for(i = 0; i < 5; i ++)
	 {
		   if(DA_ATT_ReadReg(ucNum, ATTReg, (uint8_t *)&uiRegCache) == 0)
			 {
			     fPF = (float)(uiRegCache) / 8388608;
			 }
			 break;
	 }
	 if(i == 5) 
		   return 0;
	 else
		   return fPF;
}


/**
  *@brief    read angle from ATT
  *@param    ucNum:chip select
  *           This parameter can be one of the following values: 0,1
  *@param    ATTReg:reg address
  *          This parameter can be one of the following values:
  *          r_Pga, r_Pgb, r_Pgc, r_YUaU, r_YUaUc, r_YUbUc
  *@retval   angle value
  *@note  
  */
float DA_ATT_AGRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{
	    uint8_t i;
	    float angle;
	    uint32_t  uiTemp; 
	    uint32_t  uiRegCache = 0;
	
	    assert_param(IS_ATT_ALL_REG(ATTReg));
	
			for(i = 0;i < 5;i ++)                   
			{
				if(DA_ATT_ReadReg(ucNum,ATTReg,(uint8_t*)&uiRegCache) == 0)
					{
							uiTemp=(*((uint32_t *)uiRegCache));
							if(uiTemp > 1048576) 
							{
								  uiTemp=uiTemp - 16777216;
							}
							angle=(float)uiTemp * 180 / 1048576;
			    }
			    break;
		 }	    
		 if(i == 5)      
			 return 0;
		 else
		   return angle;
}


/**
  *@brief    read frequency from ATT
  *@param    ucNum:chip select
  *           This parameter can be one of the following values: 0,1
  *@param    ATTReg:reg address
  *          This parameter can be one of the following values:
  *          r_Freq 
  *@retval    frequency value
  *@note
  */
float DA_ATT_FreqRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{
	    uint8_t i;
	    float fFreq;
	    uint32_t uiRegCache = 0;
	    
	    assert_param(IS_ATT_ALL_REG(ATTReg));
	
			for(i = 0;i < 5;i ++)                   
			{
					 if(DA_ATT_ReadReg(ucNum,ATTReg,(uint8_t*)&uiRegCache) == 0)  
					 {
						     fFreq=(float)(uiRegCache)/8192;	
					       break;
					 }						 
			}
			if(i==5)      
				 return 0;
			else
			   return fFreq;
}



/**
  *@brief    read energy from ATT,Unit:KWh/Kvar
  *@param    ucNum:chip select
  *           This parameter can be one of the following values: 0,1
  *@param    ATTReg:reg address
  *          This parameter can be one of the following values:
  *          r_Epa, r_Epb, r_Epc, r_Ept, r_Eqa, r_Eqb, r_Eqc, r_Eqt,
  *          r_Esa, r_Esb, r_Esc, r_Est, 	r_LineEpa r_LineEpb r_LineEpc, 
  *          r_LineEpt
  *@retval   energy value
  *@note   
  */
float DA_ATT_EPRead(uint8_t ucNum, ATTERegAddr_type ATTReg)
{
	    uint8_t i;  
      float fEP;  
	    uint32_t uiRegCache ;
	    
	    assert_param(IS_ATT_ALL_REG(ATTReg));
	
			for(i = 0;i < 5;i ++)                   
			{
				uiRegCache = 0;
					 if(DA_ATT_ReadReg(ucNum,ATTReg,(uint8_t*)&uiRegCache) == 0)  
					 {
						     fEP=(float)(uiRegCache)/3200;	
						     break;
					 }		 	    
			}
			if(i == 5)      
				 return 0;
      else
	       return fEP;
}


/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/




