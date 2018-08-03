/**
  ******************************************************************************
  * @file    app_TPCal.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    01-June-2018
  * @brief   this file calibrate the touch panel  
  * @endverbatim
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "app_TPCal.h"
 
/* private type definitions -------------------------------------------------------------------------*/
typedef struct XPT2046_Calibration_TypeDef
{	
  int32_t lcd_x[5];
	int32_t lcd_y[5];
  int32_t touch_x[5];
  int32_t touch_y[5];
	int32_t adjust[7];
}XPT2046_Calibration_TypeDef;

XPT2046_Calibration_TypeDef  cal_value;

/* private macro definitions ------------------------------------------------------------------------*/
/* private variables ---------------------------------------------------------------------------------*/
TP_Calibration_TypeDef   TP_Cal;

/* Expanded Variables --------------------------------------------------------------------------------*/
/* private functions prototype -----------------------------------------------------------------------*/


/* function body -------------------------------------------------------------------------------------*/

/**
  *@brief 得到简单滤波之后的X Y
  *@param pTouch_AD_x：保存测量X+对应的AD值
  *           pTouch_AD_y：保存测量Y+对应的AD值
  *           wait：松开等待使能
  *           参数 1：使能等待松开
  *                0：无需等待松开
  *@retval 无
  *@note   无
  */
static uint8_t TPCal_ReadAdc_Smooth_XY(int32_t *pTouch_AD_x,int32_t *pTouch_AD_y,uint8_t wait)
{
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray[2][10]={{0},{0}};  //坐标X和Y进行多次采样	
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  while(ucCount<10)
  {
			if(XPT2046_IRQ_Read()==0)
			{
				sAD_X = TP_XPT2046_WriteRead(XPT2046_CHANNEL_X);
				HAL_Delay(1);
				sAD_Y = TP_XPT2046_WriteRead(XPT2046_CHANNEL_Y);
				sBufferArray[0][ucCount]=sAD_X;  
				sBufferArray[1][ucCount]=sAD_Y;
				ucCount ++;  			
			}
			 if(wait==0)
				break;
  }
  
  while(wait)
  {
			if(XPT2046_IRQ_Read()!=0)break;
  }
  
	if(ucCount==10)
  {
			lX_Max=lX_Min=sBufferArray[0][0];
			lY_Max=lY_Min=sBufferArray[1][0];       
			for(i=1;i<10;i++)
			{
					if(sBufferArray[0][i]<lX_Min)
					{
						 lX_Min=sBufferArray[0][i];
					}
					else if(sBufferArray[0][i]>lX_Max)
					{
						 lX_Max=sBufferArray[0][i];
					}
			}
			for(i=1;i<10;i++)
			{
					if(sBufferArray[1][i]<lY_Min)
					{
						lY_Min=sBufferArray[1][i];
          }						
					else if(sBufferArray[1][i]>lY_Max)
					{
						lY_Max=sBufferArray[1][i];
				  }
			}		
			
			/*去除最小值和最大值之后求平均值*/
			*pTouch_AD_x=(sBufferArray[0][0]+sBufferArray[0][1]+sBufferArray[0][2]+sBufferArray[0][3]+sBufferArray[0][4]+
								 sBufferArray[0][5]+sBufferArray[0][6]+sBufferArray[0][7]+sBufferArray[0][8]+sBufferArray[0][9]-lX_Min-lX_Max)>>3;
			*pTouch_AD_y=(sBufferArray[1][0]+sBufferArray[1][1]+sBufferArray[1][2]+sBufferArray[1][3]+sBufferArray[1][4]+
								 sBufferArray[1][5]+sBufferArray[1][6]+sBufferArray[1][7]+sBufferArray[1][8]+sBufferArray[1][9]-lY_Min-lY_Max)>>3;
			return 0;
  }
  *pTouch_AD_x=-1;
  *pTouch_AD_y=-1;
  return 1;
}



/**
  *@brief  电阻屏校准算法实现
  *@param  XPT2046_Calibration结构体指针
  *@retval 0：计算成功，1：无法计算
  *@note   NONE
  */
static uint8_t TPCal_perform_calibration(XPT2046_Calibration_TypeDef *cal) 
{
		int j;
		float n, x, y, x2, y2, xy, z, zx, zy;
		float det, a, b, c, e, f, i;
		float scaling = 65536.0;

	// 获取矩阵总和
		n = x = y = x2 = y2 = xy = 0;
		for(j=0;j<5;j++) 
		{
				n += 1.0f;
				x += (float)cal->touch_x[j];
				y += (float)cal->touch_y[j];
				x2 += (float)(cal->touch_x[j]*cal->touch_x[j]);
				y2 += (float)(cal->touch_y[j]*cal->touch_y[j]);
				xy += (float)(cal->touch_x[j]*cal->touch_y[j]);
		}

	// 获取矩阵行列式，检查行列式是否太小
		det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
		if(det < 0.1f && det > -0.1f) 
		{
			 return 1;
		}

	// 获取逆矩阵元素
		a = (x2*y2 - xy*xy)/det;
		b = (xy*y - x*y2)/det;
		c = (x*xy - y*x2)/det;
		e = (n*y2 - y*y)/det;
		f = (x*y - n*xy)/det;
		i = (n*x2 - x*x)/det;

	// 获取x校准总和
		z = zx = zy = 0;
		for(j=0;j<5;j++) 
		{
				z += (float)cal->lcd_x[j];
				zx += (float)(cal->lcd_x[j]*cal->touch_x[j]);
				zy += (float)(cal->lcd_x[j]*cal->touch_y[j]);
		}

	// 获取帧缓冲区*坐标的校准值
		cal->adjust[0] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
		cal->adjust[1] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
		cal->adjust[2] = (int32_t)((c*z + f*zx + i*zy)*(scaling));

	// 获取y校准总和
		z = zx = zy = 0;
		for(j=0;j<5;j++) 
		{
				z += (float)cal->lcd_y[j];
				zx += (float)(cal->lcd_y[j]*cal->touch_x[j]);
				zy += (float)(cal->lcd_y[j]*cal->touch_y[j]);
		}

	// 获取帧缓冲区*坐标的校准值
		cal->adjust[3] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
		cal->adjust[4] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
		cal->adjust[5] = (int32_t)((c*z + f*zx + i*zy)*(scaling));
		
	// If we got here, we're OK, so assign scaling to a[6] and return
		cal->adjust[6] = (int32_t)scaling;
		
		return 0;
}


/**
  * 函数功能: 触摸屏校正函数
  * 输入参数: 无
  * 返 回 值: 0：校正成功
  *           1：校正失败
  * 说    明：无
  */
uint8_t TPCal_Calibrate(void)
{  
		uint8_t i;  
		uint16_t usTest_x = 0, usTest_y = 0;
    float Kx = 0, Ky = 0;		
		
		/* 设定“十”字交叉点的坐标 */ 
		cal_value.lcd_x[0]=20;
		cal_value.lcd_y[0]=20;
		
		cal_value.lcd_x[1]=20;
		cal_value.lcd_y[1]=LCD_VDP+1-20;
		
		cal_value.lcd_x[2]=LCD_HDP+1-20;
		cal_value.lcd_y[2]=cal_value.lcd_y[1];
		
		cal_value.lcd_x[3]=cal_value.lcd_x[2];
		cal_value.lcd_y[3]=cal_value.lcd_y[0];	
		
		cal_value.lcd_x[4]=(LCD_HDP+1)/2;
		cal_value.lcd_y[4]=(LCD_VDP+1)/2;	
		
		for(i=0; i<5; i++)
		{        
				LCD_Clear(GREEN);  		
		 
				/* 适当的延时很有必要 */        
				HAL_Delay(500);   
				LCD_DrawCross(cal_value.lcd_x[i],cal_value.lcd_y[i]); //显示校正用的“十”字 		
				TPCal_ReadAdc_Smooth_XY(&cal_value.touch_x[i],&cal_value.touch_y[i],1);
		}
		
		Kx=(((float)cal_value.touch_x[2]-(float)cal_value.touch_x[0])/((float)cal_value.lcd_x[2]-(float)cal_value.lcd_x[0])
			 +((float)cal_value.touch_x[3]-(float)cal_value.touch_x[1])/((float)cal_value.lcd_x[3]-(float)cal_value.lcd_x[1]))/2;
		Ky=(((float)cal_value.touch_y[1]-(float)cal_value.touch_y[3])/((float)cal_value.lcd_y[1]-(float)cal_value.lcd_y[3])
			 +((float)cal_value.touch_y[2]-(float)cal_value.touch_y[0])/((float)cal_value.lcd_y[2]-(float)cal_value.lcd_y[0]))/2;
		TP_Cal.CalData_XL=(float)cal_value.touch_x[0]+Kx*(0-20);
		TP_Cal.CalData_XR=(float)cal_value.touch_x[0]+Kx*(480-20);
		TP_Cal.CalData_YT=(float)cal_value.touch_y[0]+Ky*(0-20);
		TP_Cal.CalData_YB=(float)cal_value.touch_y[0]+Ky*(272-20);
		
		if(TPCal_perform_calibration(&cal_value)==1)
		{	
			 return 1;
		}

		/* 用原始参数计算出 原始参数与坐标的转换系数。 */
		for(i=0; i<2; i++)
		{    
				int xtemp,ytemp,usGap_x,usGap_y;       
				xtemp=cal_value.touch_x[2*i];
				ytemp=cal_value.touch_y[2*i];
				
				usTest_x=(int)((cal_value.adjust[0]+cal_value.adjust[1]*xtemp+cal_value.adjust[2]*ytemp)/cal_value.adjust[6]);
				usTest_y=(int)((cal_value.adjust[3]+cal_value.adjust[4]*xtemp+cal_value.adjust[5]*ytemp)/cal_value.adjust[6]);
					
				usGap_x=(usTest_x>cal_value.lcd_x[2*i])?(usTest_x-cal_value.lcd_x[2*i]):(cal_value.lcd_x[2*i]-usTest_x);   //实际X坐标与计算坐标的绝对差
				usGap_y=(usTest_y>cal_value.lcd_y[2*i])?(usTest_y-cal_value.lcd_y[2*i]):(cal_value.lcd_y[2*i]-usTest_y);   //实际Y坐标与计算坐标的绝对差
		 
				if((usGap_x>10)||(usGap_y>10))
				{
					 LCD_Clear(YELLOW);;   
					 HAL_Delay(100);  
					 return 1;   
				}      
		} 
		TP_Cal.CalFlag = 0x55AA;
		Fram_I2C_Write((uint8_t *)&TP_Cal, FMAddr_TP, sizeof(TP_Calibration_TypeDef));
		
		LCD_Clear(GREEN);; 
		HAL_Delay(100); 
		LCD_Clear(GREEN);; 
		return 0;    
}







/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE*************************************/




