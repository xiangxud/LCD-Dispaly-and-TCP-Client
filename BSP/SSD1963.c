
#include "fsmc.h"
#include "SSD1963.h"
#include "touch.h"
#include "cmsis_os.h"
#include <stdio.h> 
#include <string.h>

/**
  * 函数功能: 向触摸屏写入命令
  * 输入参数: cmd
  * 返 回 值: 无
  * 说    明：无
  */
 void SSD1963_Write_Cmd(uint16_t Cmd)
{
	
	__IO uint16_t *p = (__IO uint16_t *)(FSMC_LCD_CMD) ;

	*p = Cmd; //液晶屏把data理解成命令
}


/**
  * 函数功能: 向液晶屏写入数据
  * 输入参数: data
  * 返 回 值: 无
  * 说    明：无
  */
 void SSD1963_Write_Data(uint16_t Data)
{	
	 *(__IO uint16_t *)(FSMC_LCD_DATA) = Data ;
}


/**
  * 函数功能: 从液晶屏读取数据
  * 输入参数: 无
  * 返 回 值: 返回读数
  * 说    明：无
  */
 uint16_t SSD1963_Read_Data(void)
{
	 __IO uint16_t *p = (__IO uint16_t *)(FSMC_LCD_DATA) ;	
	return *p;
}


/**
  * 函数功能: 清屏函数，使屏显示一种颜色
  * 输入参数: color：屏显示颜色
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_Clear(unsigned int iColor)
{
    unsigned int iL=480,iW;

	SSD1963_Write_Cmd(0x002A);	
	SSD1963_Write_Data(0);	    
	SSD1963_Write_Data(0);
	SSD1963_Write_Data(HDP>>8);	    
	SSD1963_Write_Data(HDP&0x00ff);
  SSD1963_Write_Cmd(0x002B);	
	SSD1963_Write_Data(0);	    
	SSD1963_Write_Data(0);
	SSD1963_Write_Data(VDP>>8);	    
	SSD1963_Write_Data(VDP&0x00ff);
	SSD1963_Write_Cmd(0x2C);
	while(iL--)
	{
	    for(iW=0;iW<272;iW++)
		{    
          	SSD1963_Write_Data(iColor);
		}
	}
}

/**
  * 函数功能: SSD1963初始化程序
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
 void SSD1963_REG_Config(void)
{
//	SSD1963_Write_Cmd(0x0001);       //复位之后必须至少5ms延时
//	HAL_Delay(5);
	/*设置锁相环*/
	SSD1963_Write_Cmd(0x00E2);    // VCO = Reference input clock x (M + 1)，PLL frequency = VCO / (N + 1)，* Note : 250MHz < VCO < 800MHz
	SSD1963_Write_Data(0x0023);   // M  VCO=10*36=360M 
	SSD1963_Write_Data(0x0002);   // N  PLL=360/3=120M
	SSD1963_Write_Data(0x0004);   // 使乘除法有效
	
	/*锁相环启动*/
	SSD1963_Write_Cmd(0x00E0);
	SSD1963_Write_Data(0x0001);
  HAL_Delay(1);
	SSD1963_Write_Cmd(0x00E0);
	SSD1963_Write_Data(0x0003);
	HAL_Delay(5);
	SSD1963_Write_Cmd(0x0001); 
	HAL_Delay(5);
	
	/*设置Lshift像素钟频率*/    
	SSD1963_Write_Cmd(0x00E6);    //像素钟频率=PLL freq x ((LCDC_FPR + 1) / 2的20次方) =120M*(209714+1)/1048576 = 23.9MHZ
	SSD1963_Write_Data(0x0003);   //LCDC_FPR = 209714
	SSD1963_Write_Data(0x0033);   
	SSD1963_Write_Data(0x0032);   
	
	/*设置液晶面板模式和分辨率*/  
	SSD1963_Write_Cmd(0x00B0);
	SSD1963_Write_Data(0x00);  //TFT屏数据宽度 18bit , Disable FRC or dithering , 数据在下降沿锁存 ，水平脉冲，垂直脉冲低电平有效
	SSD1963_Write_Data(0x00);  //TFT模式
	SSD1963_Write_Data((HDP>>8)&0x00FF); //水平面板大小480
	SSD1963_Write_Data(HDP&0x00FF); 	
	SSD1963_Write_Data((VDP>>8)&0x00FF); //竖直面板大小272
  SSD1963_Write_Data(VDP&0x00FF); 	
	SSD1963_Write_Data(0x00);	 //RGB序列
	
	/*设置前沿*/
	SSD1963_Write_Cmd(0x00B4);
  SSD1963_Write_Data((HT>>8)&0x00FF);  // HT[10:8] = 0x02 
  SSD1963_Write_Data(HT&0x00FF);       // HT[7:0]  = 0x0D, HT[10:0]=0x020D, 水平总周期 = HT + 1 pixels 
  SSD1963_Write_Data((HPS>>8)&0x00FF); // HPS[10:8] = 0x00    
  SSD1963_Write_Data(HPS&0x00FF);      // HPS[7:0]  = 0x28, HPS[10:0] = 0x28, 水平同步脉冲起始位置 = 0x29 pixels  
  SSD1963_Write_Data(HPW);             // HPW[6:0] = 0x28, 水平同步脉冲宽度 = HPS + 1 pixels 
  SSD1963_Write_Data((LPS>>8)&0x00FF); // LPS[10:8] = 0x00    
  SSD1963_Write_Data(LPS&0x00FF);             // LPS[7:0]  = 0x06, LPS[10:0] = 0x06, 水平显示时段起始位置 =LPS pixels 
  SSD1963_Write_Data(0x00);            // LPSPP[1:0] = 0x00 ,设置串行TFT接口的水平同步脉冲子像素开始位置
	
	/*设置垂直消隐间隔在最后扫描行和下一个LFRAME脉冲之间*/
	SSD1963_Write_Cmd(0x00B6);
	SSD1963_Write_Data((VT>>8)&0x00FF);	 // VT[10:8] = 0x01 
	SSD1963_Write_Data(VT&0x00FF); 	     // VT[7:0]  = 0x20, VT[10:0]=0x0120, 垂直总数 = VT + 1
	SSD1963_Write_Data((VPS>>8)&0x00FF); // VPS[10:8] = 0x00 
	SSD1963_Write_Data(VPS&0x00FF); 	   // VPS[7:0] = 0x0C, HPS[10:0]= 0x0C, 垂直同步脉冲开始位置 = VPS
	SSD1963_Write_Data(VPW);	           // VPW[6:0] = 0x0B, 垂直同步脉冲宽度 = VPW + 1
	SSD1963_Write_Data((FPS>>8)&0x00FF); // FPS[10:8] = 0x00
	SSD1963_Write_Data(FPS&0x00FF); 	   // FPS[7:0] = 0x08, FPS[10:0]=0x08, 垂直显示时段开始位置 = FPS
	
	/*GPIO为输出时，配置GPIO的值*/
	SSD1963_Write_Cmd(0x00BA);	
	SSD1963_Write_Data(0x0F);  //GPIO0、GPIO1、GPIO2、GPIO3都输出低电平
	
	SSD1963_Write_Cmd(0x00B8);
	SSD1963_Write_Data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	SSD1963_Write_Data(0x0001);    //GPIO0 normal
	
	/*设置从帧缓冲器到显示面板的读取顺序*/
	SSD1963_Write_Cmd(0x0036);
	SSD1963_Write_Data(0x00);   // A[7]=0 行从顶部到底部， A[6]=0 列从左到又，  A[5]=0 数据列传输为正常模式， A[4]=0 从顶部向底部刷新， 
	                            // A[3]=0 RGB数据顺序      A[2]=0 从左到右刷新  A[1]=0 显示图像从左到右翻转   A[0]=0 显示图像从上到下翻转
	
	/*设置像素格式*/
	SSD1963_Write_Cmd(0x00F0);
	SSD1963_Write_Data(0x0003);  //16-bit (565 format)
	
	SSD1963_Write_Cmd(0x0020);	//进入图形颜色不倒置
	
//	SSD1963_Write_Cmd(0x00BC);//重要
//	SSD1963_Write_Data(0x0080);//对比度
//  SSD1963_Write_Data(0x0080);//亮度
//  SSD1963_Write_Data(0x0080);//饱和度值  //
//  SSD1963_Write_Data(0x00);//处理机允许
	
	HAL_Delay(10);
	LCD_Clear(GREEN);
	
	SSD1963_Write_Cmd(0x0026); //display on
	SSD1963_Write_Data(0x0001);
	
  /*在显示板上显示图像*/
	SSD1963_Write_Cmd(0x0029);	
	
	/*Set pwm config*/
	SSD1963_Write_Cmd(0x00BE);	
	SSD1963_Write_Data(0x0006);  //PWM 信号频率= PLL clock/(256*PWMF[7:0])/256 = 116M/(256*6)/256=295HZ
	SSD1963_Write_Data(0x0080);  //PWM 占空比 = PWM[7:0] / 295= 240/295=81.3%(for DBC disable [0xD0] A0 = 0)
	SSD1963_Write_Data(0x0001);  //PWM controlled by host && PWM enable
	SSD1963_Write_Data(0x00F0);
	SSD1963_Write_Data(0x00);  //手动设置最亮  0xFF ：最暗
	SSD1963_Write_Data(0x00);  //最低亮度最亮  0xFF : 最暗
	
	/*动态背光控制*/
	SSD1963_Write_Cmd(0x00D0);
	SSD1963_Write_Data(0x000D);
	HAL_Delay(10);

}

/**
  * 函数功能: 在LCD显示器上开辟一个窗口
  * 输入参数: usX ：在特定扫描方向下窗口的起点X坐标
  *           usY ：在特定扫描方向下窗口的起点Y坐标
  *           usWidth ：窗口的宽度
  *           usHeight ：窗口的高度
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{	
	SSD1963_Write_Cmd(0x2A); 				       /* 设置X坐标 */
	SSD1963_Write_Data(usX>>8);	             /* 设置起始点：先高8位 */
	SSD1963_Write_Data(usX&0xff);	           /* 然后低8位 */
	SSD1963_Write_Data((usX+usWidth-1)>>8);  /* 设置结束点：先高8位 */
	SSD1963_Write_Data((usX+usWidth-1)&0xff);/* 然后低8位 */

	SSD1963_Write_Cmd(0x2B); 			           /* 设置Y坐标*/
	SSD1963_Write_Data(usY>>8);              /* 设置起始点：先高8位 */
	SSD1963_Write_Data(usY&0xff);            /* 然后低8位 */
	SSD1963_Write_Data((usY+usHeight-1)>>8); /* 设置结束点：先高8位 */
	SSD1963_Write_Data((usY+usHeight-1)&0xff);/* 然后低8位 */
}

/**
  * 函数功能: 在指定区域内填充指定颜色
  * 区域大小：(iYend-iYsta)*(xend-iXsta)
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_Fill(unsigned int iXsta,unsigned int iYsta,unsigned int iXend,unsigned int iYend,unsigned int iColor)
{
  unsigned long n;
	n=(unsigned long)(iYend-iYsta+1)*(iXend-iXsta+1);
	SSD1963_Write_Cmd(0x002A);	
	SSD1963_Write_Data(iXsta>>8);	    
	SSD1963_Write_Data(iXsta&0x00ff);
	SSD1963_Write_Data(iXend>>8);	    
	SSD1963_Write_Data(iXend&0x00ff);
  SSD1963_Write_Cmd(0x002B);	
	SSD1963_Write_Data(iYsta>>8);	    
	SSD1963_Write_Data(iYsta&0x00ff);
	SSD1963_Write_Data(iYend>>8);	    
	SSD1963_Write_Data(iYend&0x00ff);
	SSD1963_Write_Cmd(0x002C);
	while(n--)SSD1963_Write_Data(iColor);
} 

/**
  * 函数功能: 在 LCD 显示器上使用 Bresenham 算法画线段
  * 输入参数: usX1 ：在特定扫描方向下窗口的起点X坐标
  *           usY1 ：在特定扫描方向下窗口的起点Y坐标
  *           usX2 ：在特定扫描方向下线段的另一个端点X坐标
  *           usY2 ：在特定扫描方向下线段的另一个端点Y坐标
  *           usColor ：线段的颜色
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_DrawLine(uint16_t usX1,uint16_t usY1,uint16_t usX2,uint16_t usY2,uint16_t usColor)
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	int32_t lError_X=0,lError_Y=0,lDelta_X,lDelta_Y,lDistance; 
	int32_t lIncrease_X, lIncrease_Y;
	
	lDelta_X=usX2-usX1; //计算坐标增量 
	lDelta_Y=usY2-usY1; 
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	if(lDelta_X>0)
  {
    lIncrease_X=1; //设置单步方向 
	}
  else if(lDelta_X==0)
  {
		lIncrease_X=0;//垂直线 
	}
	else 
  { 
    lIncrease_X=-1;
    lDelta_X=-lDelta_X;
  }
  
	if(lDelta_Y>0)
  {
		lIncrease_Y=1;
	}
	else if(lDelta_Y==0)
  {
		lIncrease_Y=0;//水平线 
	}
	else
  {
    lIncrease_Y=-1;
    lDelta_Y=-lDelta_Y;
  }
	
	if(lDelta_X>lDelta_Y)
  {
		lDistance=lDelta_X; //选取基本增量坐标轴 
	}
	else
  {
		lDistance=lDelta_Y; 
  }
	
	for(us=0;us<=lDistance+1;us++)//画线输出 
	{
		LCD_Fill(usX_Current,usY_Current,usX_Current,usY_Current,usColor);//画点 
		lError_X+=lDelta_X; 
		lError_Y+=lDelta_Y;
		if(lError_X>lDistance)
		{ 
			lError_X-=lDistance; 
			usX_Current+=lIncrease_X; 
		}
		if(lError_Y>lDistance) 
		{ 
			lError_Y-=lDistance; 
			usY_Current+=lIncrease_Y; 
		}		
	}
}

/**
  * 函数功能: 校正触摸时画十字专用 
  * 输入参数: x：十字中点x轴
  *           y：十字中点y轴
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_DrawCross(uint16_t x,uint16_t y)
{
  LCD_DrawLine(x-10,y,x+10,y,RED);
  LCD_DrawLine(x,y-10,x,y+10,RED);
}

/**
  * 函数功能: LCD屏背光
  * 输入参数: ENABLE/DISABLE
  * 返 回 值: 无
  * 说    明：无
  */
static void LCD_Back_Light(uint8_t State)
{
	if(State == ENABLE)
		HAL_GPIO_WritePin(LCD_BL_CTR_GPIO_Port,LCD_BL_CTR_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_BL_CTR_GPIO_Port,LCD_BL_CTR_Pin,GPIO_PIN_RESET);
}


void LCD_Init(void)
{
	//低电平复位
	SSD1963_RESET_LOW();
	HAL_Delay(100);
	SSD1963_RESET_HIGH();
	HAL_Delay(100);
	SSD1963_REG_Config();
	
  osDelay(500);	
	LCD_Back_Light(ENABLE);
}

