#include "touch.h"
#include <stdio.h> 
#include <string.h>

#include "bsp_Fram.h"
#include "sys_FramAlloc.h"
#include "cmsis_os.h"




/* 私有变量 ------------------------------------------------------------------*/
XPT2046_Calibration cal_value={0};
TP_Calibration_TypeDef   TP_Cal;

 /**SPI3 GPIO Configuration    
  PC12     ------> SPI3_MOSI
  PB3     ------> SPI3_SCK
  PB4     ------> SPI3_MISO 
  PB5     ------> SPI3_CS
	*/
			
void Touch_Init_GPIO(void)
{ 
  GPIO_InitTypeDef GPIO_InitStruct;

  /* 开启GPIO时钟 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* 模拟SPI GPIO初始化 */
  GPIO_InitStruct.Pin = TP_SPI_SCK_Pin|TP_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin  = TP_SPI_MOSI_Pin; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = TP_SPI_MISO_Pin; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull =GPIO_PULLUP ;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = TP_IRQ_Pin; 
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
    
  /* 拉低片选，选择XPT2046 */
  TP_SPI_CS_LOW();
}


/**
  * 函数功能: us 级别延时，不是很精确
  * 输入参数: cnt：延时时间
  * 返 回 值: 无
  * 说    明：无
  */
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  	      
		while ( uc -- );     //延1微秒	
	}	
}


/**
  * 函数功能: 写命令
  * 输入参数: CHX 	0x90 	//通道Y+的选择控制字 
  *           CHY   0xd0	//通道X+的选择控制字
  * 返 回 值: 无
  * 说    明：无
  */
static void XPT2046_WriteCMD(uint8_t ucCmd)
{
	uint8_t i;
	TP_SPI_MOSI_LOW();	
	TP_SPI_SCK_LOW();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? TP_SPI_MOSI_HIGH() : TP_SPI_MOSI_LOW();		
	  XPT2046_DelayUS(5);		
		TP_SPI_SCK_HIGH();
	  XPT2046_DelayUS(5);
		TP_SPI_SCK_LOW();
	}	
}



/**
  * 函数功能: 读数据
  * 输入参数: 无
  * 返 回 值: short：读到的数据
  * 说    明：无
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;
  TP_SPI_MOSI_LOW();
	TP_SPI_SCK_HIGH();
	for ( i=0;i<12;i++ ) 
	{
		TP_SPI_SCK_LOW();	
		usTemp = TP_SPI_MISO_READ();		
		usBuf |= usTemp << ( 11 - i );	
	  TP_SPI_SCK_HIGH();		
	}	
	return usBuf;
}


/**
  * 函数功能: 选择一个模拟通道，启动ADC，并返回ADC采样结果
  * 输入参数: _ucCh = 0x90 表示Y通道；
  *                   0xd0 表示X通道
  * 返 回 值: 无
  * 说    明：无
  */
static uint16_t XPT2046_ReadAdc(uint8_t _ucCh)
{
  XPT2046_WriteCMD(_ucCh);
	return 	XPT2046_ReadCMD();
}

/**
  * 函数功能: 读取TP x y 的AD值(12bit，最大是4096)
  * 输入参数: x：读到AD值
  *           y：读到AD值
  * 返 回 值: 无
  * 说    明：无
  */
static void XPT2046_ReadAdc_XY(int16_t *sX_Ad,int16_t *sY_Ad)  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 	
	
	sX_Ad_Temp = XPT2046_ReadAdc(CMD_RDX);
	XPT2046_DelayUS(10); 
	sY_Ad_Temp = XPT2046_ReadAdc(CMD_RDY); 		
	* sY_Ad = sX_Ad_Temp; 
	* sX_Ad = sY_Ad_Temp;	
}


/**
  * 函数功能: 得到简单滤波之后的X Y
  * 输入参数: pTouch_AD_x：保存测量X+对应的AD值
  *           pTouch_AD_y：保存测量Y+对应的AD值
  *           wait：松开等待使能
  *            参数 1：使能等待松开
  *                 0：无需等待松开
  * 返 回 值: 无
  * 说    明：无
  */
static uint8_t XPT2046_ReadAdc_Smooth_XY(int16_t *pTouch_AD_x,int16_t *pTouch_AD_y,uint8_t wait)
{
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray[2][10]={{0},{0}};  //坐标X和Y进行多次采样	
	int16_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  while(ucCount<10)
  {
    if(TP_IRQ_READ()==0)
    {
      XPT2046_ReadAdc_XY(&sAD_X,&sAD_Y);      
      sBufferArray[0][ucCount]=sAD_X;  
      sBufferArray[1][ucCount]=sAD_Y;
      ucCount ++;  			
    }
     if(wait==0)
      break;
  }
  
  while(wait)
  {
    if(TP_IRQ_READ()!=0)break;
  }
  
	if(ucCount==10)
  {
    lX_Max=lX_Min=sBufferArray[0][0];
    lY_Max=lY_Min=sBufferArray[1][0];       
    for(i=1;i<10;i++)
    {
      if(sBufferArray[0][i]<lX_Min)
        lX_Min=sBufferArray[0][i];
      else if(sBufferArray[0][i]>lX_Max)
        lX_Max=sBufferArray[0][i];
    }
    for(i=1;i<10;i++)
    {
      if(sBufferArray[1][i]<lY_Min)
        lY_Min=sBufferArray[1][i];			
      else if(sBufferArray[1][i]>lY_Max)
        lY_Max=sBufferArray[1][i];
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
  * 函数功能: 电阻屏校准算法实现
  * 输入参数: XPT2046_Calibration结构体指针
  * 返 回 值: 0：计算成功，1：无法计算
  * 说    明：无
  */
static uint8_t perform_calibration(XPT2046_Calibration *cal) 
{
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

// 获取矩阵总和
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) {
		n += 1.0f;
		x += (float)cal->touch_x[j];
		y += (float)cal->touch_y[j];
		x2 += (float)(cal->touch_x[j]*cal->touch_x[j]);
		y2 += (float)(cal->touch_y[j]*cal->touch_y[j]);
		xy += (float)(cal->touch_x[j]*cal->touch_y[j]);
	}

// 获取矩阵行列式，检查行列式是否太小
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1f && det > -0.1f) {
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
	for(j=0;j<5;j++) {
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
	for(j=0;j<5;j++) {
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

 uint8_t XPT2046_Touch_Calibrate(void)
{  
  uint8_t i;  
  uint16_t usTest_x=0,usTest_y=0;  
  
  /* 设定“十”字交叉点的坐标 */ 
  cal_value.lcd_x[0]=20;
  cal_value.lcd_y[0]=20;
  
  cal_value.lcd_x[1]=20;
  cal_value.lcd_y[1]=VDP+1-20;
  
  cal_value.lcd_x[2]=HDP+1-20;
  cal_value.lcd_y[2]=cal_value.lcd_y[1];
  
  cal_value.lcd_x[3]=cal_value.lcd_x[2];
  cal_value.lcd_y[3]=cal_value.lcd_y[0];	
  
  cal_value.lcd_x[4]=(HDP+1)/2;
  cal_value.lcd_y[4]=(VDP+1)/2;	
  
  for(i=0; i<5; i++)
  {        
    LCD_Clear(GREEN);  		
//    LCD_DispString_EN(50,120,"Touch Calibrate...",BACKGROUND,BLUE,USB_FONT_24);	         
//    LCD_DispChar_EN(150, 80, i+'1',BACKGROUND,RED,USB_FONT_24);
 
    /* 适当的延时很有必要 */        
    XPT2046_DelayUS(500000);    
    LCD_DrawCross(cal_value.lcd_x[i],cal_value.lcd_y[i]); //显示校正用的“十”字 		
    XPT2046_ReadAdc_Smooth_XY(&cal_value.touch_x[i],&cal_value.touch_y[i],1);
  }
	
	cal_value.kx=(((float)cal_value.touch_x[2]-(float)cal_value.touch_x[0])/((float)cal_value.lcd_x[2]-(float)cal_value.lcd_x[0])
		+((float)cal_value.touch_x[3]-(float)cal_value.touch_x[1])/((float)cal_value.lcd_x[3]-(float)cal_value.lcd_x[1]))/2;
  cal_value.ky=(((float)cal_value.touch_y[1]-(float)cal_value.touch_y[3])/((float)cal_value.lcd_y[1]-(float)cal_value.lcd_y[3])
		+((float)cal_value.touch_y[2]-(float)cal_value.touch_y[0])/((float)cal_value.lcd_y[2]-(float)cal_value.lcd_y[0]))/2;
	TP_Cal.CalData_XL=(float)cal_value.touch_x[0]+cal_value.kx*(0-20);
	TP_Cal.CalData_XR=(float)cal_value.touch_x[0]+cal_value.kx*(480-20);
	TP_Cal.CalData_YT=(float)cal_value.touch_y[0]+cal_value.ky*(0-20);
	TP_Cal.CalData_YB=(float)cal_value.touch_y[0]+cal_value.ky*(272-20);
  if(perform_calibration(&cal_value)==1) 
    return 1;

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
      osDelay(1000);   
      return 1;   
    }      
  }
  
  TP_Cal.CalFlag = 0x55AA;
	
  taskENTER_CRITICAL();	
	Fram_I2C_Write((uint8_t *)&TP_Cal, FMAddr_TP, sizeof(TP_Calibration_TypeDef));
  Fram_I2C_Write((uint8_t *)&cal_value, FMAddr_TP+sizeof(TP_Calibration_TypeDef), sizeof(XPT2046_Calibration));
	taskEXIT_CRITICAL();
	
  LCD_Clear(GREEN);; 
  osDelay(1000);
  LCD_Clear(GREEN);; 
  return 0;    
}



#define SAMP_CNT        4          // 定义采集次数
#define SAMP_CNT_DIV2   2          // 采集次数/2
/**
  * 函数功能: 选择一个模拟通道，启动ADC，并返回ADC采样结果
  * 输入参数: channel ：通道选择
  *           参数值：0x90 :通道Y
  *           参数值：0xd0 :通道X
  * 返 回 值: 滤波后的12位ADC值
  * 说    明：无
  */
uint16_t adc_x = 0,adc_y = 0;
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel)
{
  uint8_t i, j, min;
  uint16_t temp;
  uint16_t tempXY[SAMP_CNT];
  


  /* 如果检查到触摸屏被按下，才进行触摸屏通道采集，否则直接退出函数 */
  /* 通过触摸屏IRQ引脚可以判断触摸屏是否被触摸，有被触摸时为低电平，否则为高电平 */
  if(TP_IRQ_READ()== 0) 
  {
    /* 连续采样SAMP_CNT次数的数据 */
    for(i = 0;i < SAMP_CNT;i++)
    {
      XPT2046_WriteCMD(channel);
      tempXY[i] = XPT2046_ReadCMD();     
    }
    /* 降序排列 */
    for(i=0; i<SAMP_CNT-1; i++)
    {
      min=i;
      for (j=i+1; j<SAMP_CNT; j++)
      {
        if (tempXY[min] > tempXY[j]) min=j;
      }
      temp = tempXY[i];
      tempXY[i] = tempXY[min];
      tempXY[min] = temp;
    }
    
   // 求中间值的均值   
   if(channel == 0xD0)
   {
      adc_x = (tempXY[SAMP_CNT_DIV2]+tempXY[SAMP_CNT_DIV2-1]) / 2;
      return adc_x;
    }      
    else
    {
      adc_y = (tempXY[SAMP_CNT_DIV2]+tempXY[SAMP_CNT_DIV2-1]) / 2;
      return adc_y;
    }       
  }
  else
  {
    return 0;   //没有触摸，返回0
  }
}



void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}

int  GUI_TOUCH_X_MeasureX(void)	
{
	 return XPT2046_ReadAdc_Fliter(0xD0);
}
int  GUI_TOUCH_X_MeasureY(void)
{       
	return XPT2046_ReadAdc_Fliter(0x90);
}







