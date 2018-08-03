/**
  ******************************************************************************
  * @file    main.c
  * @author  Qing Lixia
  * @version V3.1.1
  * @date    18-May-2018
  * @brief   this file contains main() of the project   
  * @endverbatim
  ******************************************************************************
  */
	
	/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_IWDG.h"
#include "sys_Init.h"
#include "sys_Tasks.h"	

static void SystemClock_Config(void);
	
int main()
{	
	 
    /*����������ǰ�� Ϊ�˷�ֹ��ʼ������ʱ���жϷ������ִ�У��������ֹȫ���жϣ�����HMI��HardFault����
		  �������ĺô��ǣ�
			1.��ִֹ�е��жϷ����������FreeRTOS��API������
			2.��֤ϵͳ���������� ���ܱ���ж�Ӱ�졣
			����ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�ͨ��ָ��cpsie i������ __set_PRIMASK(1)��cpsie i��Ч		
		 */
	  __set_PRIMASK(1);
		
		/* Ӳ�����Ź���ʼ�� */
    IWDG_Init();
		
	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
		
	  /* Configure the system clock */
    SystemClock_Config();
		
	  /* ���輰������ʼ�� */
	  sys_Init();
		
//	  /* freeRTOS������ʱ�� */
//    InfoTestTimeInit(2);
	
	  MX_FREERTOS_Init();
	  osKernelStart();
		
    while(1)
		{ }
}
	
	
/** System Clock Configuration
*/
static void SystemClock_Config(void)
{

  RCC_OscInitTypeDef  RCC_OscInitStruct;
  RCC_ClkInitTypeDef  RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
	
	return;
}	
	
	/************************ (C) COPYRIGHT Qing Lixia *****END OF FILE****/












