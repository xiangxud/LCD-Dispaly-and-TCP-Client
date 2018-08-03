/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "LCDConf.h"
#include "SSD1963.h"
#include "GUIDRV_FlexColor.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  480 // To be adapted to x-screen size    //【修改】 
#define YSIZE_PHYS  272 // To be adapted to y-screen size    //【修改】 

//#define TOUCH_AD_TOP 	 			  226    //YPhys0
//#define TOUCH_AD_BOTTOM  			3854   //YPhys1
//#define TOUCH_AD_LEFT 	 			250   //XPhys0
//#define TOUCH_AD_RIGHT 	 			3780    //XPhys1


#define TOUCH_AD_TOP 	 			  3728    //YPhys0
#define TOUCH_AD_BOTTOM  			182   //YPhys1
#define TOUCH_AD_LEFT 	 			140   //XPhys0
#define TOUCH_AD_RIGHT 	 			4008    //XPhys1
	
// 颜色转换 
#define COLOR_CONVERSION GUICC_M565             //【修改】 


// 显示驱动
#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR        //【修改】 

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION                //【修改】 
  #error Color conversion not defined!
#endif 
#ifndef   DISPLAY_DRIVER                  //【修改】 
  #error No display driver defined! 
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) {
	
	SSD1963_Write_Cmd(Data);  //【修改】 
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
	
	SSD1963_Write_Data(Data);  //【修改】 
	
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
	
	while (NumItems--) {
  SSD1963_Write_Data(*pData++);   //【修改】 

	}	
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
		
   	*pData++=SSD1963_Read_Data();   //【修改】 

  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS); 
	/* Touch calibration */ 

  //
  // Orientation
  //
  //Config.Orientation = GUI_MIRROR_X;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);   //【修改】 
// GUI_TOUCH_SetOrientation((GUI_MIRROR_X * LCD_GetMirrorXEx(0)) |
//              (GUI_MIRROR_Y * LCD_GetMirrorYEx(0)) |
//              (GUI_SWAP_XY  * LCD_GetSwapXYEx (0)));
  //电阻屏需要设置触摸校准值 //【修改】
  GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS-1 ,TOUCH_AD_LEFT , TOUCH_AD_RIGHT);
  GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS-1 ,TOUCH_AD_TOP,TOUCH_AD_BOTTOM );  

}
















/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(Cmd);
  GUI_USE_PARA(pData);
  return 0;
}


/*************************** End of file ****************************/

