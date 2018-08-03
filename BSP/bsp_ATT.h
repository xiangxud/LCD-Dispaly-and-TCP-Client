/**
  ******************************************************************************
  * @file    bsp_ATT.h
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    21-May-2018
  * @brief   Interface of source file bsp_ATT.c    
  * @endverbatim
  ******************************************************************************
  */
	
#ifndef _BSP_ATT_H
#define _BSP_ATT_H

/* Includes ---------------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* type definitions -------------------------------------------------------------------------*/
typedef enum 
{
	r_DeviceID              =(uint8_t)0x00,       /*!< Device ID */
	r_Pa                    =(uint8_t)0x01,       /*!<phase A active power */
	r_Pb                    =(uint8_t)0x02,       /*!<phase B active power */
  r_Pc                    =(uint8_t)0x03,       /*!<phase C active power */
	r_Pt                    =(uint8_t)0x04,       /*!<appulse active power */
	r_Qa                    =(uint8_t)0x05,       /*!<phase A reactive power */
	r_Qb                    =(uint8_t)0x06,       /*!<phase B reactive power */
	r_Qc                    =(uint8_t)0x07,       /*!<phase C reactive power */
	r_Qt                    =(uint8_t)0x08,       /*!<appulse reactive power */
	r_Sa                    =(uint8_t)0x09,       /*!<phase A apparent power */
	r_Sb                    =(uint8_t)0x0A,       /*!<phase B apparent power */
	r_Sc                    =(uint8_t)0x0B,       /*!<phase C apparent power */
	r_St                    =(uint8_t)0x0C,       /*!<appulse apparent power */
	r_UaRms                 =(uint8_t)0x0D,       /*!<phase A Urms */
	r_UbRms                 =(uint8_t)0x0E,       /*!<phase B Urms */
	r_UcRms                 =(uint8_t)0x0F,       /*!<phase C Urms */
	r_IaRms                 =(uint8_t)0x10,       /*!<phase A Irms */
	r_IbRms                 =(uint8_t)0x11,       /*!<phase B Irms */
	r_IcRms                 =(uint8_t)0x12,       /*!<phase C Irms */
	r_ItRms                 =(uint8_t)0x13,       /*!<appulse Irms */
	r_Pfa                   =(uint8_t)0x14,       /*!<phase A power factor */
	r_Pfb                   =(uint8_t)0x15,       /*!<phase B power factor */
	r_Pfc                   =(uint8_t)0x16,       /*!<phase C power factor */
	r_Pft                   =(uint8_t)0x17,       /*!<appulse power factor */
	r_Pga                   =(uint8_t)0x18,       /*!<A phase angle of voltage and current */
	r_Pgb                   =(uint8_t)0x19,       /*!<B phase angle of voltage and current */
	r_Pgc                   =(uint8_t)0x1A,       /*!<C phase angle of voltage and current */
	r_INTFlag               =(uint8_t)0x1B,       /*!<interrupt identification,got zero */
	r_Freq                  =(uint8_t)0x1C,       /*!<line frequency */
	r_EFlag                 =(uint8_t)0x1D,       /*!<operating state of energy register */
	r_Epa                   =(uint8_t)0x1E,       /*!<phase A active energy */
	r_Epb                   =(uint8_t)0x1F,       /*!<phase B active energy */
	r_Epc                   =(uint8_t)0x20,       /*!<phase C active energy */
	r_Ept                   =(uint8_t)0x21,       /*!<appulse active energy */
	r_Eqa                   =(uint8_t)0x22,       /*!<phase A reactive energy */
	r_Eqb                   =(uint8_t)0x23,       /*!<phase B reactive energy */
	r_Eqc                   =(uint8_t)0x24,       /*!<phase C reactive energy */
	r_Eqt                   =(uint8_t)0x25,       /*!<appulse reactive energy */
	r_YUaUb                 =(uint8_t)0x26,       /*!<angle between Ua and Ub */
	r_YUaUc                 =(uint8_t)0x27,       /*!<angle between Ua and Uc */
	r_YUbUc                 =(uint8_t)0x28,       /*!<angel between Ub and Uc */
	r_I0Rms                 =(uint8_t)0x29,       /*!<rms of neutral line curent I0 */
	r_TPSD                  =(uint8_t)0x2A,       /*!<out of temperature sensor */
	r_UtRms                 =(uint8_t)0x2B,       /*!<rms or three-phase vector sum */
	r_Sflag                 =(uint8_t)0x2C,       /*!<state flag register */
	r_BckReg                =(uint8_t)0x2D,       /*!<communicate data backup register*/
	r_ComChksum             =(uint8_t)0x2E,       /*!<communication checksum register */
	r_Sample_IA             =(uint8_t)0x2F,       /*!< ADC sampel of phase A current */
	r_Sample_IB             =(uint8_t)0x30,       /*!< ADC sampel of phase B current */
	r_Sample_IC             =(uint8_t)0x31,       /*!< ADC sampel of phase C current */
	r_Sample_UA             =(uint8_t)0x32,       /*!< ADC sampel of phase A voltage */
	r_Sample_UB             =(uint8_t)0x33,       /*!< ADC sampel of phase B voltage */
	r_Sample_UC             =(uint8_t)0x34,       /*!< ADC sampel of phase C voltage */
	r_Esa                   =(uint8_t)0x35,       /*!<phase A apparent energy */
	r_Esb                   =(uint8_t)0x36,       /*!<phase B apparent energy */
	r_Esc                   =(uint8_t)0x37,       /*!<phase C apparent energy */
	r_Est                   =(uint8_t)0x38,       /*!<appulse apparent energy */
	r_FstCntA               =(uint8_t)0x39,       /*!<phase A fast pulse count */
	r_FstCntB               =(uint8_t)0x3A,       /*!<phase B fast pulse count */
	r_FstCntC               =(uint8_t)0x3B,       /*!<phase C fast pulse count */
	r_FstCntT               =(uint8_t)0x3C,       /*!<appulse fast pulse count */
	r_PFlag                 =(uint8_t)0x3D,       /*!<power direction,0-positive,1-negtive */
	r_ChkSum                =(uint8_t)0x3E,       /*!<checksum of calibration data */
	r_Sample_I0             =(uint8_t)0x3F,       /*!<out of neutral line current I0 sample channel */
	
	/*Peculiar to ATT7022E*/
	r_LinePa                =(uint8_t)0x40,       /*!<phase A fundamental acive power */
	r_LinePb                =(uint8_t)0x41,       /*!<phase B fundamental acive power */
	r_LinePc                =(uint8_t)0x42,       /*!<phase C fundamental acive power */
	r_LinePt                =(uint8_t)0x43,       /*!<appulse fundamental acive power */
	r_LineEpa               =(uint8_t)0x44,       /*!<phase A fundamental acive energy */
	r_LineEpb               =(uint8_t)0x45,       /*!<phase B fundamental acive energy */
	r_LineEpc               =(uint8_t)0x46,       /*!<phase C fundamental acive energy */
	r_LineEpt               =(uint8_t)0x47,       /*!<appulse fundamental acive energy */
	r_LineUaRrms            =(uint8_t)0x48,       /*!<phase A fundamental Vrms */
	r_LineUbRrms            =(uint8_t)0x49,       /*!<phase B fundamental Vrms */
	r_LineUcRrms            =(uint8_t)0x4A,       /*!<phase C fundamental Vrms */
	r_LineIaRrms            =(uint8_t)0x4B,       /*!<phase A fundamental Irms */
	r_LineIbRrms            =(uint8_t)0x4C,       /*!<phase B fundamental Irms */
	r_LineIcRrms            =(uint8_t)0x4D,       /*!<phase C fundamental Irms */
	r_LEFlag                =(uint8_t)0x4E,       /*!<operating state of fundamental energy register,got zero */
  r_SAGFlag               =(uint8_t)0x4F,       /*!<SAG flag */
  r_PeakUa                =(uint8_t)0x50,       /*!<phase A  max voltage */
  r_PeakUb                =(uint8_t)0x51,       /*!<phase B  max voltage */	
	r_PeakUc                =(uint8_t)0x52,       /*!<phase C  max voltage */
	r_PtrWavebuff           =(uint8_t)0x7E,       /*!<pointer to buffer data */
	r_WaveBuff              =(uint8_t)0x7F,       /*!<buffer data register */
	
	/* Calibration Reg */
	
}ATTERegAddr_type;

#define IS_ATT_ALL_REG(REG) (((REG) == r_DeviceID)\
                             ((REG) == r_Pa) || \
                             ((REG) == r_Pb) || \
                             ((REG) == r_Pc) || \
                             ((REG) == r_Pt) || \
                             ((REG) == r_Qa) || \
                             ((REG) == r_Qb) || \
                             ((REG) == r_Qc) || \
                             ((REG) == r_Qt) || \
                             ((REG) == r_Sa) || \
                             ((REG) == r_Sb) || \
                             ((REG) == r_Sc) || \
                             ((REG) == r_St) || \
                             ((REG) == r_UaRms) || \
                             ((REG) == r_UbRms) || \
                             ((REG) == r_UcRms) || \
                             ((REG) == r_IaRms) || \
                             ((REG) == r_IbRms) || \
                             ((REG) == r_IcRms) || \
                             ((REG) == r_ItRms) || \
                             ((REG) == r_Pfa) || \
                             ((REG) == r_Pfb) || \
														 ((REG) == r_Pfc) || \
														 ((REG) == r_Pft) || \
														 ((REG) == r_Pga) || \
														 ((REG) == r_Pgb) || \
														 ((REG) == r_Pgc) || \
														 ((REG) == r_INTFlag) || \
														 ((REG) == r_Freq) || \
														 ((REG) == r_EFlag) || \
														 ((REG) == r_Epa) ||\
														 ((REG) == r_Epb) ||\
														 ((REG) == r_Epc) ||\
														 ((REG) == r_Ept) ||\
														 ((REG) == r_Eqa) ||\
														 ((REG) == r_Eqb) ||\
														 ((REG) == r_Eqc) ||\
														 ((REG) == r_Eqt) ||\
														 ((REG) == r_YUaUb) ||\
														 ((REG) == r_YUaUc) ||\
														 ((REG) == r_YUbUc) ||\
														 ((REG) == r_I0Rms) ||\
														 ((REG) == r_TPSD) ||\
														 ((REG) == r_UtRms) ||\
														 ((REG) == r_Sflag) ||\
														 ((REG) == r_BckReg) ||\
														 ((REG) == r_ComChksum) ||\
														 ((REG) == r_Sample_IA) ||\
														 ((REG) == r_Sample_IB) ||\
														 ((REG) == r_Sample_IC) ||\
														 ((REG) == r_Sample_UA) ||\
														 ((REG) == r_Sample_UB) ||\
														 ((REG) == r_Sample_UC) ||\
														 ((REG) == r_Esa) ||\
														 ((REG) == r_Esb) ||\
														 ((REG) == r_Esc) ||\
														 ((REG) == r_Est) ||\
														 ((REG) == r_FstCntA) ||\
														 ((REG) == r_FstCntB) ||\
														 ((REG) == r_FstCntC) ||\
														 ((REG) == r_FstCntT) ||\
														 ((REG) == r_PFlag) ||\
														 ((REG) == r_ChkSum ) ||\
														 ((REG) == r_Sample_I0) ||\
														 ((REG) == r_LinePa) ||\
														 ((REG) == r_LinePb) ||\
														 ((REG) == r_LinePc) ||\
														 ((REG) == r_LinePt) ||\
														 ((REG) == r_LineEpa) ||\
														 ((REG) == r_LineEpb) ||\
														 ((REG) == r_LineEpc) ||\
														 ((REG) == r_LineEpt) ||\
														 ((REG) == r_LineUaRrms) ||\
														 ((REG) == r_LineUbRrms) ||\
														 ((REG) == r_LineUcRrms) ||\
														 ((REG) == r_LineIaRrms) ||\
														 ((REG) == r_LineIbRrms) ||\
														 ((REG) == r_LineIcRrms) ||\
														 ((REG) == r_LEFlag) ||\
														 ((REG) == r_SAGFlag) ||\
														 ((REG) == r_PeakUa) ||\
														 ((REG) == r_PeakUb) ||\
														 ((REG) == r_PeakUc) ||\
														 ((REG) == r_PtrWavebuff) ||\
														 ((REG) == r_WaveBuff))
														 
														 
/* macro definitions ------------------------------------------------------------------------*/	
/* Expanded Variables -----------------------------------------------------------------------*/
/* function declarations --------------------------------------------------------------------*/
uint8_t DA_ATT_Init(void);
void DA_ATT_Check(uint8_t ucNum);
float DA_ATT_RMSRead(uint8_t ucNum, ATTERegAddr_type ATTReg);
float DA_ATT_PowerRead(uint8_t ucNum, ATTERegAddr_type ATTReg);
float DA_ATT_PFRead(uint8_t ucNum, ATTERegAddr_type ATTReg);
float DA_ATT_AGRead(uint8_t ucNum, ATTERegAddr_type ATTReg);
float DA_ATT_FreqRead(uint8_t ucNum, ATTERegAddr_type ATTReg);
float DA_ATT_EPRead(uint8_t ucNum, ATTERegAddr_type ATTReg);


#endif /* end of _BSP_ATT_H */



