
#include "fsmc.h"
#include "SSD1963.h"
#include "touch.h"
#include "cmsis_os.h"
#include <stdio.h> 
#include <string.h>

/**
  * ��������: ������д������
  * �������: cmd
  * �� �� ֵ: ��
  * ˵    ������
  */
 void SSD1963_Write_Cmd(uint16_t Cmd)
{
	
	__IO uint16_t *p = (__IO uint16_t *)(FSMC_LCD_CMD) ;

	*p = Cmd; //Һ������data��������
}


/**
  * ��������: ��Һ����д������
  * �������: data
  * �� �� ֵ: ��
  * ˵    ������
  */
 void SSD1963_Write_Data(uint16_t Data)
{	
	 *(__IO uint16_t *)(FSMC_LCD_DATA) = Data ;
}


/**
  * ��������: ��Һ������ȡ����
  * �������: ��
  * �� �� ֵ: ���ض���
  * ˵    ������
  */
 uint16_t SSD1963_Read_Data(void)
{
	 __IO uint16_t *p = (__IO uint16_t *)(FSMC_LCD_DATA) ;	
	return *p;
}


/**
  * ��������: ����������ʹ����ʾһ����ɫ
  * �������: color������ʾ��ɫ
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: SSD1963��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
 void SSD1963_REG_Config(void)
{
//	SSD1963_Write_Cmd(0x0001);       //��λ֮���������5ms��ʱ
//	HAL_Delay(5);
	/*�������໷*/
	SSD1963_Write_Cmd(0x00E2);    // VCO = Reference input clock x (M + 1)��PLL frequency = VCO / (N + 1)��* Note : 250MHz < VCO < 800MHz
	SSD1963_Write_Data(0x0023);   // M  VCO=10*36=360M 
	SSD1963_Write_Data(0x0002);   // N  PLL=360/3=120M
	SSD1963_Write_Data(0x0004);   // ʹ�˳�����Ч
	
	/*���໷����*/
	SSD1963_Write_Cmd(0x00E0);
	SSD1963_Write_Data(0x0001);
  HAL_Delay(1);
	SSD1963_Write_Cmd(0x00E0);
	SSD1963_Write_Data(0x0003);
	HAL_Delay(5);
	SSD1963_Write_Cmd(0x0001); 
	HAL_Delay(5);
	
	/*����Lshift������Ƶ��*/    
	SSD1963_Write_Cmd(0x00E6);    //������Ƶ��=PLL freq x ((LCDC_FPR + 1) / 2��20�η�) =120M*(209714+1)/1048576 = 23.9MHZ
	SSD1963_Write_Data(0x0003);   //LCDC_FPR = 209714
	SSD1963_Write_Data(0x0033);   
	SSD1963_Write_Data(0x0032);   
	
	/*����Һ�����ģʽ�ͷֱ���*/  
	SSD1963_Write_Cmd(0x00B0);
	SSD1963_Write_Data(0x00);  //TFT�����ݿ�� 18bit , Disable FRC or dithering , �������½������� ��ˮƽ���壬��ֱ����͵�ƽ��Ч
	SSD1963_Write_Data(0x00);  //TFTģʽ
	SSD1963_Write_Data((HDP>>8)&0x00FF); //ˮƽ����С480
	SSD1963_Write_Data(HDP&0x00FF); 	
	SSD1963_Write_Data((VDP>>8)&0x00FF); //��ֱ����С272
  SSD1963_Write_Data(VDP&0x00FF); 	
	SSD1963_Write_Data(0x00);	 //RGB����
	
	/*����ǰ��*/
	SSD1963_Write_Cmd(0x00B4);
  SSD1963_Write_Data((HT>>8)&0x00FF);  // HT[10:8] = 0x02 
  SSD1963_Write_Data(HT&0x00FF);       // HT[7:0]  = 0x0D, HT[10:0]=0x020D, ˮƽ������ = HT + 1 pixels 
  SSD1963_Write_Data((HPS>>8)&0x00FF); // HPS[10:8] = 0x00    
  SSD1963_Write_Data(HPS&0x00FF);      // HPS[7:0]  = 0x28, HPS[10:0] = 0x28, ˮƽͬ��������ʼλ�� = 0x29 pixels  
  SSD1963_Write_Data(HPW);             // HPW[6:0] = 0x28, ˮƽͬ�������� = HPS + 1 pixels 
  SSD1963_Write_Data((LPS>>8)&0x00FF); // LPS[10:8] = 0x00    
  SSD1963_Write_Data(LPS&0x00FF);             // LPS[7:0]  = 0x06, LPS[10:0] = 0x06, ˮƽ��ʾʱ����ʼλ�� =LPS pixels 
  SSD1963_Write_Data(0x00);            // LPSPP[1:0] = 0x00 ,���ô���TFT�ӿڵ�ˮƽͬ�����������ؿ�ʼλ��
	
	/*���ô�ֱ������������ɨ���к���һ��LFRAME����֮��*/
	SSD1963_Write_Cmd(0x00B6);
	SSD1963_Write_Data((VT>>8)&0x00FF);	 // VT[10:8] = 0x01 
	SSD1963_Write_Data(VT&0x00FF); 	     // VT[7:0]  = 0x20, VT[10:0]=0x0120, ��ֱ���� = VT + 1
	SSD1963_Write_Data((VPS>>8)&0x00FF); // VPS[10:8] = 0x00 
	SSD1963_Write_Data(VPS&0x00FF); 	   // VPS[7:0] = 0x0C, HPS[10:0]= 0x0C, ��ֱͬ�����忪ʼλ�� = VPS
	SSD1963_Write_Data(VPW);	           // VPW[6:0] = 0x0B, ��ֱͬ�������� = VPW + 1
	SSD1963_Write_Data((FPS>>8)&0x00FF); // FPS[10:8] = 0x00
	SSD1963_Write_Data(FPS&0x00FF); 	   // FPS[7:0] = 0x08, FPS[10:0]=0x08, ��ֱ��ʾʱ�ο�ʼλ�� = FPS
	
	/*GPIOΪ���ʱ������GPIO��ֵ*/
	SSD1963_Write_Cmd(0x00BA);	
	SSD1963_Write_Data(0x0F);  //GPIO0��GPIO1��GPIO2��GPIO3������͵�ƽ
	
	SSD1963_Write_Cmd(0x00B8);
	SSD1963_Write_Data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	SSD1963_Write_Data(0x0001);    //GPIO0 normal
	
	/*���ô�֡����������ʾ���Ķ�ȡ˳��*/
	SSD1963_Write_Cmd(0x0036);
	SSD1963_Write_Data(0x00);   // A[7]=0 �дӶ������ײ��� A[6]=0 �д����֣�  A[5]=0 �����д���Ϊ����ģʽ�� A[4]=0 �Ӷ�����ײ�ˢ�£� 
	                            // A[3]=0 RGB����˳��      A[2]=0 ������ˢ��  A[1]=0 ��ʾͼ������ҷ�ת   A[0]=0 ��ʾͼ����ϵ��·�ת
	
	/*�������ظ�ʽ*/
	SSD1963_Write_Cmd(0x00F0);
	SSD1963_Write_Data(0x0003);  //16-bit (565 format)
	
	SSD1963_Write_Cmd(0x0020);	//����ͼ����ɫ������
	
//	SSD1963_Write_Cmd(0x00BC);//��Ҫ
//	SSD1963_Write_Data(0x0080);//�Աȶ�
//  SSD1963_Write_Data(0x0080);//����
//  SSD1963_Write_Data(0x0080);//���Ͷ�ֵ  //
//  SSD1963_Write_Data(0x00);//���������
	
	HAL_Delay(10);
	LCD_Clear(GREEN);
	
	SSD1963_Write_Cmd(0x0026); //display on
	SSD1963_Write_Data(0x0001);
	
  /*����ʾ������ʾͼ��*/
	SSD1963_Write_Cmd(0x0029);	
	
	/*Set pwm config*/
	SSD1963_Write_Cmd(0x00BE);	
	SSD1963_Write_Data(0x0006);  //PWM �ź�Ƶ��= PLL clock/(256*PWMF[7:0])/256 = 116M/(256*6)/256=295HZ
	SSD1963_Write_Data(0x0080);  //PWM ռ�ձ� = PWM[7:0] / 295= 240/295=81.3%�f(for DBC disable [0xD0] A0 = 0)
	SSD1963_Write_Data(0x0001);  //PWM controlled by host && PWM enable
	SSD1963_Write_Data(0x00F0);
	SSD1963_Write_Data(0x00);  //�ֶ���������  0xFF ���
	SSD1963_Write_Data(0x00);  //�����������  0xFF : �
	
	/*��̬�������*/
	SSD1963_Write_Cmd(0x00D0);
	SSD1963_Write_Data(0x000D);
	HAL_Delay(10);

}

/**
  * ��������: ��LCD��ʾ���Ͽ���һ������
  * �������: usX �����ض�ɨ�跽���´��ڵ����X����
  *           usY �����ض�ɨ�跽���´��ڵ����Y����
  *           usWidth �����ڵĿ��
  *           usHeight �����ڵĸ߶�
  * �� �� ֵ: ��
  * ˵    ������
  */
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{	
	SSD1963_Write_Cmd(0x2A); 				       /* ����X���� */
	SSD1963_Write_Data(usX>>8);	             /* ������ʼ�㣺�ȸ�8λ */
	SSD1963_Write_Data(usX&0xff);	           /* Ȼ���8λ */
	SSD1963_Write_Data((usX+usWidth-1)>>8);  /* ���ý����㣺�ȸ�8λ */
	SSD1963_Write_Data((usX+usWidth-1)&0xff);/* Ȼ���8λ */

	SSD1963_Write_Cmd(0x2B); 			           /* ����Y����*/
	SSD1963_Write_Data(usY>>8);              /* ������ʼ�㣺�ȸ�8λ */
	SSD1963_Write_Data(usY&0xff);            /* Ȼ���8λ */
	SSD1963_Write_Data((usY+usHeight-1)>>8); /* ���ý����㣺�ȸ�8λ */
	SSD1963_Write_Data((usY+usHeight-1)&0xff);/* Ȼ���8λ */
}

/**
  * ��������: ��ָ�����������ָ����ɫ
  * �����С��(iYend-iYsta)*(xend-iXsta)
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: �� LCD ��ʾ����ʹ�� Bresenham �㷨���߶�
  * �������: usX1 �����ض�ɨ�跽���´��ڵ����X����
  *           usY1 �����ض�ɨ�跽���´��ڵ����Y����
  *           usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
  *           usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y����
  *           usColor ���߶ε���ɫ
  * �� �� ֵ: ��
  * ˵    ������
  */
void LCD_DrawLine(uint16_t usX1,uint16_t usY1,uint16_t usX2,uint16_t usY2,uint16_t usColor)
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	int32_t lError_X=0,lError_Y=0,lDelta_X,lDelta_Y,lDistance; 
	int32_t lIncrease_X, lIncrease_Y;
	
	lDelta_X=usX2-usX1; //������������ 
	lDelta_Y=usY2-usY1; 
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	if(lDelta_X>0)
  {
    lIncrease_X=1; //���õ������� 
	}
  else if(lDelta_X==0)
  {
		lIncrease_X=0;//��ֱ�� 
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
		lIncrease_Y=0;//ˮƽ�� 
	}
	else
  {
    lIncrease_Y=-1;
    lDelta_Y=-lDelta_Y;
  }
	
	if(lDelta_X>lDelta_Y)
  {
		lDistance=lDelta_X; //ѡȡ�������������� 
	}
	else
  {
		lDistance=lDelta_Y; 
  }
	
	for(us=0;us<=lDistance+1;us++)//������� 
	{
		LCD_Fill(usX_Current,usY_Current,usX_Current,usY_Current,usColor);//���� 
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
  * ��������: У������ʱ��ʮ��ר�� 
  * �������: x��ʮ���е�x��
  *           y��ʮ���е�y��
  * �� �� ֵ: ��
  * ˵    ������
  */
void LCD_DrawCross(uint16_t x,uint16_t y)
{
  LCD_DrawLine(x-10,y,x+10,y,RED);
  LCD_DrawLine(x,y-10,x,y+10,RED);
}

/**
  * ��������: LCD������
  * �������: ENABLE/DISABLE
  * �� �� ֵ: ��
  * ˵    ������
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
	//�͵�ƽ��λ
	SSD1963_RESET_LOW();
	HAL_Delay(100);
	SSD1963_RESET_HIGH();
	HAL_Delay(100);
	SSD1963_REG_Config();
	
  osDelay(500);	
	LCD_Back_Light(ENABLE);
}

