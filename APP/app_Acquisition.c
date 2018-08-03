/**
  ******************************************************************************
  * @file    app_Acquisition.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    22-May-2018
  * @brief   collect busbar data acquisition 
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes -------------------------------------------------------------------*/
#include "app_Acquisition.h"
#include "bsp_GPIO.h"
#include "bsp_ATT.h"
#include "bsp_Fram.h"
#include "sys_FramAlloc.h"
#include "sys_Variable.h"
#include "cmsis_os.h"
#include "math.h"
#include "string.h"


/* private type definitions ---------------------------------------------------*/
/* private macro definitions --------------------------------------------------*/
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

/* private variables ----------------------------------------------------------*/
static float fReq_Buff[14];

/* Expanded Variables ---------------------------------------------------------*/
/* private functions prototype ------------------------------------------------*/
static float THD_Calculate(float fRms, float fLrms);
static float VL2L_Calculate(float fPhV1, float fPhV2);

/* function body --------------------------------------------------------------*/

/**
  *@brief  busbar data acquisition
	*@param  NONE
	*@retval NONE
	*@note   NONE
	*/
void  Acq_BusVar(void)
{
   uint8_t i, temp;
	 float fTemp;
	
	 if((Optimset_st.Setup.Setup_st.bBusMode == BusModeS) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
	 {
			 temp = 2;
	 }
	 else
	 {
		   temp = 1;
       memset(&Acq_BusVar_st_a[1], 0x20, sizeof(Acq_BusVar_st_a[1]));		 
	 }	
	 
	
	 for(i = 0; i < temp; i ++)
	 {
	    DA_ATT_Check(i);
		 
		  /* ÏàµçÑ¹ */
		  Acq_BusVar_st_a[i].fUa = DA_ATT_RMSRead(i, r_UaRms);
		  Acq_BusVar_st_a[i].fUb = DA_ATT_RMSRead(i, r_UbRms);
		  Acq_BusVar_st_a[i].fUc = DA_ATT_RMSRead(i, r_UcRms);
		 
		  /* µçÑ¹Ğ³²¨»û±äÂÊ */
		  if(Acq_BusVar_st_a[i].fUa > 5)
			{
			   fTemp = DA_ATT_RMSRead(i, r_LineUaRrms);
				 Acq_BusVar_st_a[i].fVTHDa = THD_Calculate(Acq_BusVar_st_a[i].fUa, fTemp);			
			}
			else
			{
			   Acq_BusVar_st_a[i].fVTHDa = 0;
			}
			
			if(Acq_BusVar_st_a[i].fUb > 5)
			{
			   fTemp = DA_ATT_RMSRead(i, r_LineUbRrms);
				 Acq_BusVar_st_a[i].fVTHDb = THD_Calculate(Acq_BusVar_st_a[i].fUb, fTemp);			
			}
			else
			{
			   Acq_BusVar_st_a[i].fVTHDb = 0;
			}
			
			if(Acq_BusVar_st_a[i].fUc > 5)
			{
			   fTemp = DA_ATT_RMSRead(i, r_LineUcRrms);
				 Acq_BusVar_st_a[i].fVTHDc = THD_Calculate(Acq_BusVar_st_a[i].fUc, fTemp);			
			}
			else
			{
			   Acq_BusVar_st_a[i].fVTHDc = 0;
			}
		 
		  /* ÏßµçÑ¹ */
		  Acq_BusVar_st_a[i].fUab =  VL2L_Calculate(Acq_BusVar_st_a[i].fUa, Acq_BusVar_st_a[i].fUb);
			Acq_BusVar_st_a[i].fUbc =  VL2L_Calculate(Acq_BusVar_st_a[i].fUb, Acq_BusVar_st_a[i].fUc);
			Acq_BusVar_st_a[i].fUca =  VL2L_Calculate(Acq_BusVar_st_a[i].fUc, Acq_BusVar_st_a[i].fUa);
			
			/* µçÁ÷ */
		  Acq_BusVar_st_a[i].fIa = DA_ATT_RMSRead(i, r_IaRms)*Optimset_st.usCTR[i]/5;
			Acq_BusVar_st_a[i].fIb = DA_ATT_RMSRead(i, r_IbRms)*Optimset_st.usCTR[i]/5;
			Acq_BusVar_st_a[i].fIc = DA_ATT_RMSRead(i, r_IcRms)*Optimset_st.usCTR[i]/5;
			
			/* ×î´óµçÁ÷ */
			Ana_BusVar_st_a[i].fMaxIa = MAX(Ana_BusVar_st_a[i].fMaxIa, Acq_BusVar_st_a[i].fIa);
			Ana_BusVar_st_a[i].fMaxIb = MAX(Ana_BusVar_st_a[i].fMaxIb, Acq_BusVar_st_a[i].fIb);
			Ana_BusVar_st_a[i].fMaxIc = MAX(Ana_BusVar_st_a[i].fMaxIc, Acq_BusVar_st_a[i].fIc);
			
			/* µçÁ÷Ğ³²¨»û±äÂÊ */
			fTemp = DA_ATT_RMSRead(i,r_LineIaRrms)*Optimset_st.usCTR[i]/5;
			Acq_BusVar_st_a[i].fITHDa = THD_Calculate(Acq_BusVar_st_a[i].fIa, fTemp);
			fTemp = DA_ATT_RMSRead(i,r_LineIbRrms)*Optimset_st.usCTR[i]/5;
			Acq_BusVar_st_a[i].fITHDb = THD_Calculate(Acq_BusVar_st_a[i].fIb, fTemp);
			fTemp = DA_ATT_RMSRead(i,r_LineIcRrms)*Optimset_st.usCTR[i]/5;
			Acq_BusVar_st_a[i].fITHDc = THD_Calculate(Acq_BusVar_st_a[i].fIc, fTemp);
			
			/* ÓĞ¹¦¹¦ÂÊ £¬µ¥Î»KW*/
			Acq_BusVar_st_a[i].fPa = DA_ATT_PowerRead(i, r_Pa)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fPb = DA_ATT_PowerRead(i, r_Pb)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fPc = DA_ATT_PowerRead(i, r_Pc)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fPt = DA_ATT_PowerRead(i, r_Pt)*2*Optimset_st.usCTR[i]/5/1000;
 
      /* ÎŞ¹¦¹¦ÂÊ */
      Acq_BusVar_st_a[i].fQa = DA_ATT_PowerRead(i, r_Qa)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fQb = DA_ATT_PowerRead(i, r_Qb)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fQc = DA_ATT_PowerRead(i, r_Qc)*Optimset_st.usCTR[i]/5/1000;
      Acq_BusVar_st_a[i].fQt = DA_ATT_PowerRead(i, r_Qt)*2*Optimset_st.usCTR[i]/5/1000;	

      /* ÆµÂÊ */
      Acq_BusVar_st_a[i].fF = DA_ATT_FreqRead(i, r_Freq);
			
			/* ¹¦ÂÊÒòÊı */
			Acq_BusVar_st_a[i].fPFa = DA_ATT_PFRead(i, r_Pfa);
			Acq_BusVar_st_a[i].fPFb = DA_ATT_PFRead(i, r_Pfb);
			Acq_BusVar_st_a[i].fPFc = DA_ATT_PFRead(i, r_Pfc);
			Acq_BusVar_st_a[i].fPFt = DA_ATT_PFRead(i, r_Pft);
	 }
	 return;
}


/**
  *@brief  busbar state monitor
	*@param  NONE
	*@retval NONE
	*@note   NONE
	*/
void Acq_BusState(void)
{
	  /* AÄ¸Ïß¿ª¹Ø */
    if(BUSA_QF_State() == 0)
		{
		    X.BusStatus_st_a[0].bQF = 0;   //±ÕºÏ
		}
		else
		{
		    X.BusStatus_st_a[0].bQF = 1;   //¶Ï¿ª
		}

	 /* BÄ¸Ïß¿ª¹Ø */		
	  if(Optimset_st.Setup.Setup_st.bBusMode != BusModeD)
		{
				if(BUSB_QF_State() == 0)
				{
						X.BusStatus_st_a[1].bQF = 0;   //±ÕºÏ
				}
				else
				{
						X.BusStatus_st_a[1].bQF = 1;   //¶Ï¿ª
				}
	  }

    if(Optimset_st.Setup.Setup_st.bSPD == 1)
		{
				/* AÄ¸Ïß·ÀÀ× */
				if(BUSA_SPD_State() == 1)
				{
						X.BusStatus_st_a[0].bSPD = 0;   //Õı³£
				}
				else
				{
						X.BusStatus_st_a[0].bSPD = 1;   //¹ÊÕÏ
				}
				
				/* BÄ¸Ïß·ÀÀ× */
				if((Optimset_st.Setup.Setup_st.bBusMode == BusModeS) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
				{						
						if(BUSB_SPD_State() == 1)
						{
								X.BusStatus_st_a[1].bSPD = 0;   //±ÕºÏ
						}
						else
						{
								X.BusStatus_st_a[1].bSPD = 1;   //¹ÊÕÏ
						}
				}
		}

   return;		
}


/**
  *@brief  read energy from FRAM
  *@param  NONE
  *@retval NONE
  *@note  
  */
void Acq_EnergyInit(void)
{
   uint8_t i;
	 uint16_t usFM_OperateAddr;
	
	 for(i = 0; i < 2; i++)
	 {
	    if(0 == i)
			{
			   usFM_OperateAddr = FMAddr_EP_RealTime;
			}
			else
			{
			   usFM_OperateAddr = FMAddr_EP_RealTime + 32;
			}
			Fram_I2C_Read((uint8_t*)&Acq_BusVar_st_a[i].uiEPa, usFM_OperateAddr, 32);
  	}
		
		return;
}

/**
  *@brief  real time energy
  *@param  NONE
  *@retval NONE
  *@note   NONE
  */
void Acq_RTEnergy(void)
{
    uint8_t i,temp;
	  uint16_t usRecEP,usFM_OperateAddr;
	  static float fEPremain[8]={0};
		float fEPtemp;


	  if((Optimset_st.Setup.Setup_st.bBusMode == BusModeS) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
		{
			  temp = 2;
		}
		else
		{
		    temp = 1;	
		}
		
		for(i = 0; i < temp; i ++)
		{
		   if(i == 0)
			 {
			    usFM_OperateAddr = FMAddr_EP_RealTime;
			 }
			 else
			 {
			    usFM_OperateAddr = FMAddr_EP_RealTime + 32;
			 }
			 
			 fEPremain[i*4+0] += DA_ATT_EPRead(i, r_Epa);
			 fEPtemp = fEPremain[i*4+0] *Optimset_st.usCTR[i]/5;
			 usRecEP = fEPtemp * 100;
			 if(fEPremain[i*4+0] >= ((float)usRecEP/100/(Optimset_st.usCTR[i]/5)))
			 {
			    fEPremain[i*4+0] -= (float)usRecEP/100/(Optimset_st.usCTR[i]/5);
				  Acq_BusVar_st_a[i].uiEPa += usRecEP;
			 }
			 
			 fEPremain[i*4+1] += DA_ATT_EPRead(i, r_Epb);
			 fEPtemp = fEPremain[i*4+1] *Optimset_st.usCTR[i]/5;
			 usRecEP = fEPtemp * 100;
			 if(fEPremain[i*4+1] >= ((float)usRecEP/100/(Optimset_st.usCTR[i]/5)))
			 {
			    fEPremain[i*4+1] -= (float)usRecEP/100/(Optimset_st.usCTR[i]/5);
				  Acq_BusVar_st_a[i].uiEPb += usRecEP;
			 }
			 
			 fEPremain[i*4+2] += DA_ATT_EPRead(i, r_Epc);
			 fEPtemp = fEPremain[i*4+2] *Optimset_st.usCTR[i]/5;
			 usRecEP = fEPtemp * 100;
			 if(fEPremain[i*4+2] >= ((float)usRecEP/100/(Optimset_st.usCTR[i]/5)))
			 {
			    fEPremain[i*4+2] -= (float)usRecEP/100/(Optimset_st.usCTR[i]/5);
				  Acq_BusVar_st_a[i].uiEPc += usRecEP;
			 }
			 
			 fEPremain[i*4+3] += DA_ATT_EPRead(i, r_Ept);
			 fEPtemp = fEPremain[i*4+3] *Optimset_st.usCTR[i]/5;
			 usRecEP = fEPtemp * 100;
			 if(fEPremain[i*4+3] >= ((float)usRecEP/100/(Optimset_st.usCTR[i]/5)))
			 {
			    fEPremain[i*4+3] -= (float)usRecEP/100/(Optimset_st.usCTR[i]/5);
				  Acq_BusVar_st_a[i].uiEPt += usRecEP;
			 }
			 
			 Acq_BusVar_st_a[i].fEQa += DA_ATT_EPRead(i, r_Eqa)*Optimset_st.usCTR[i]/5;
			 Acq_BusVar_st_a[i].fEQb += DA_ATT_EPRead(i, r_Eqb)*Optimset_st.usCTR[i]/5;
			 Acq_BusVar_st_a[i].fEQc += DA_ATT_EPRead(i, r_Eqc)*Optimset_st.usCTR[i]/5;
			 Acq_BusVar_st_a[i].fEQt += DA_ATT_EPRead(i, r_Eqt)*Optimset_st.usCTR[i]/5;		
        
			 taskENTER_CRITICAL();
       Fram_I2C_Write((uint8_t *)&Acq_BusVar_st_a[i].uiEPa, usFM_OperateAddr, 32);
       taskEXIT_CRITICAL();			 
		}
		
		return;
}


/**
  *å‡½æ•°åŠŸèƒ½ï¼šç”µèƒ½æ¸…é›¶
  *å…¥å£å‚æ•°ï¼šnone
  *å‡ºå£å‚æ•°ï¼šnone
  *å‡½æ•°è¯´æ˜ï¼šä¿å­˜å†å²å€¼ï¼Œæ¸…é›¶å†å²ç”µèƒ½ä¿å­˜å€¼åŠå‚æ•°ç”µèƒ½è¿ç®—çš„å˜é‡
  *         æ¥æ”¶åˆ°æ¥è‡ªè§¦æ‘¸å±çš„æ¸…é›¶æŒ‡ä»¤æ—¶è°ƒç”¨
  */
void Acq_EnergyClear(void)
{
		uint8_t i;
	  uint16_t usFM_OperateAddr;
	
	  float nTemp=nTemp;
		 
		for(i=0;i<2;i++)
		{
				if(i==0) 
				{
						usFM_OperateAddr=FMAddr_EP_RealTime;
				}				
				else     		
				{
						usFM_OperateAddr=FMAddr_EP_RealTime+32;
				}	
				
				/*set energy register zero*/
				nTemp=DA_ATT_EPRead(i, r_Epa);							
				nTemp=DA_ATT_EPRead(i, r_Epb);						
				nTemp=DA_ATT_EPRead(i, r_Epc);						
				nTemp=DA_ATT_EPRead(i, r_Ept);						
				nTemp=DA_ATT_EPRead(i, r_Eqa);						
				nTemp=DA_ATT_EPRead(i, r_Eqb);
				nTemp=DA_ATT_EPRead(i, r_Eqc);						
				nTemp=DA_ATT_EPRead(i, r_Eqt);
				
				/*set parameter zero*/
				Acq_BusVar_st_a[i].uiEPa=0;
				Acq_BusVar_st_a[i].uiEPb=0;
				Acq_BusVar_st_a[i].uiEPc=0;
				Acq_BusVar_st_a[i].uiEPt=0;
				Acq_BusVar_st_a[i].fEQa=0;
				Acq_BusVar_st_a[i].fEQb=0;
				Acq_BusVar_st_a[i].fEQc=0;
				Acq_BusVar_st_a[i].fEQt=0;
				
				/*set energy memory zero*/
				taskENTER_CRITICAL();
				Fram_I2C_Write((uint8_t *)&Acq_BusVar_st_a[i].uiEPa, usFM_OperateAddr, 32);
        taskEXIT_CRITICAL();					
		}
}




/**
  *@brief  Requirement analysis
  *@param  NONE
  *@retval NONE
  *@note   called per minuite
  */
void Acq_Req(void)
{
    uint8_t i, temp;
	  static uint8_t ucCounter = 0;

	  if((Optimset_st.Setup.Setup_st.bBusMode == BusModeS) || (Optimset_st.Setup.Setup_st.bBusMode == BusModeY))
		{
			  temp = 2;
		}
		else
		{
		    temp = 1;
			  memset(&Ana_BusVar_st_a[1], 0x20, sizeof(Ana_BusVar_st_a[2]));	
		}
		
		for(i = 0; i < temp; i ++)
		{
			 fReq_Buff[i*7+0] += Acq_BusVar_st_a[i].fIa;
			 fReq_Buff[i*7+1] += Acq_BusVar_st_a[i].fIb;
			 fReq_Buff[i*7+2] += Acq_BusVar_st_a[i].fIc;
			 fReq_Buff[i*7+3] += Acq_BusVar_st_a[i].fPa;
			 fReq_Buff[i*7+4] += Acq_BusVar_st_a[i].fPb;
			 fReq_Buff[i*7+5] += Acq_BusVar_st_a[i].fPc;
			 fReq_Buff[i*7+6] += Acq_BusVar_st_a[i].fPt;
		} 
		ucCounter ++;
		if(ucCounter == 15)
		{
				 for(i = 0; i < temp; i ++)
				 {
						Ana_BusVar_st_a[i].fReqIa = fReq_Buff[i*7+0]/15;
						Ana_BusVar_st_a[i].fReqIb = fReq_Buff[i*7+1]/15;
						Ana_BusVar_st_a[i].fReqIc = fReq_Buff[i*7+2]/15;
						Ana_BusVar_st_a[i].fReqPa = fReq_Buff[i*7+3]/15;
						Ana_BusVar_st_a[i].fReqPb = fReq_Buff[i*7+4]/15;
						Ana_BusVar_st_a[i].fReqPc = fReq_Buff[i*7+5]/15;
						Ana_BusVar_st_a[i].fReqPt = fReq_Buff[i*7+6]/15;
					 
						Ana_BusVar_st_a[i].fMaxReqIa = MAX(Ana_BusVar_st_a[i].fMaxReqIa, Ana_BusVar_st_a[i].fReqIa); 
						Ana_BusVar_st_a[i].fMaxReqIb = MAX(Ana_BusVar_st_a[i].fMaxReqIb, Ana_BusVar_st_a[i].fReqIb); 
						Ana_BusVar_st_a[i].fMaxReqIc = MAX(Ana_BusVar_st_a[i].fMaxReqIc, Ana_BusVar_st_a[i].fReqIc); 
						Ana_BusVar_st_a[i].fMaxReqPa = MAX(Ana_BusVar_st_a[i].fMaxReqPa, Ana_BusVar_st_a[i].fReqPa); 
						Ana_BusVar_st_a[i].fMaxReqPb = MAX(Ana_BusVar_st_a[i].fMaxReqPb, Ana_BusVar_st_a[i].fReqPb); 
						Ana_BusVar_st_a[i].fMaxReqPc = MAX(Ana_BusVar_st_a[i].fMaxReqPc, Ana_BusVar_st_a[i].fReqPc); 
						Ana_BusVar_st_a[i].fMaxReqPt = MAX(Ana_BusVar_st_a[i].fMaxReqPt, Ana_BusVar_st_a[i].fReqPt); 					 
				 }
				 for(i = 0; i< 14; i ++)
				 {
						fReq_Buff[i] = 0;
				 }
		}	

    return;		
}


/**
  *å‡½æ•°åŠŸèƒ½ï¼šæ¸…é™¤æœ€å¤§å€¼
  *å…¥å£å‚æ•°ï¼šnone
  *å‡ºå£å‚æ•°ï¼šnone
  *å‡½æ•°è¯´æ˜ï¼š
  */
void Acq_MAXClear(void)
{
		uint8_t i;
		for(i=0;i<2;i++)
		{
				 fReq_Buff[i*7]=0;
				 fReq_Buff[i*7+1]=0;
				 fReq_Buff[i*7+2]=0;
				 fReq_Buff[i*7+3]=0;
				 fReq_Buff[i*7+4]=0;
				 fReq_Buff[i*7+5]=0;
				 fReq_Buff[i*7+6]=0;
					
				 Ana_BusVar_st_a[i].fMaxIa=0;
				 Ana_BusVar_st_a[i].fMaxIb=0;
				 Ana_BusVar_st_a[i].fMaxIc=0;
				 Ana_BusVar_st_a[i].fMaxReqIa=0;
				 Ana_BusVar_st_a[i].fMaxReqIb=0;
				 Ana_BusVar_st_a[i].fMaxReqIc=0;
				 Ana_BusVar_st_a[i].fMaxReqPa=0;
				 Ana_BusVar_st_a[i].fMaxReqPb=0;
				 Ana_BusVar_st_a[i].fMaxReqPc=0;
				 Ana_BusVar_st_a[i].fMaxReqPt=0;
		}
}



/**
  *@brief  Total Harmonic Distortion
  *@param  fRms--È«²¨ÓĞĞ§Öµ
  *        fLrms--»ù²¨ÓĞĞ§Öµ
  *@retval THD value
  *@note
  */
static float THD_Calculate(float fRms, float fLrms)
{
   float  fRms_d, fLrms_d, fTHD;
	
	 fRms_d = fRms * fRms;
	 fLrms_d = fLrms * fLrms;
	 
	 if(fLrms_d > 0)
	 {
	    fTHD = sqrt(fRms_d / fLrms_d -1) * 100;
	 }
	 else
	 {
	    fTHD = 0;
	 }
	
	 return fTHD;
}

/**
  *@brief  Line-to-Line Voltage
  *@param  fPhV1
  *        fPHV2
  *@retval NONE
  *@note   ÇóÏàµçÑ¹fPhV1ºÍfPhV2Ö®¼äµÄÏßµçÑ¹
  */
static float VL2L_Calculate(float fPhV1, float fPhV2)
{
   float fVL2L;
	
	 fVL2L = sqrt((fPhV1 * fPhV1) + (fPhV1 * fPhV2) + (fPhV2 * fPhV2));
	
	 return fVL2L;
}	


/**
  *@brief  branch switch status
  *@param  NONE
  *@retval NONE
  *@note
  */
/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/



