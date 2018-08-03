/**
  ******************************************************************************
  * File Name          : FSMC.c
  * Description        : This file provides code for the configuration
  *                      of the FSMC peripheral.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "fsmc.h"

/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FSMC initialization function */
void MX_FSMC_Init(void)
{
 FSMC_NORSRAM_TimingTypeDef readwriteTiming;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;                             //指定使用的Bank的设备存储器
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;          //数据线与地址线不复用
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_NOR;                       //存储器类型 NOFLASH
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;         //数据宽度16位
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;        //使用异步写模式，禁止突发模式
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;      //在突发模式下有效，低电平有效
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;                       //禁止非对其突发模式
  hsram1.Init.WaitSignalActive =FSMC_WAIT_TIMING_BEFORE_WS ;           //突发模式下有效 NWAIT信号在什么时候产生
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;            //写使能
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;                   //突发模式下有效
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;               //禁止扩展模式，扩展模式可以使用独立的读写模式
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;       //异步传输期间禁用或者允许等待信号
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;                   //禁止突发写操作
  hsram1.Init.PageSize=FSMC_PAGE_SIZE_NONE;                          //指定内存页面大小
	
	
  /* Timing */
  readwriteTiming.AddressSetupTime = 0x03;                    //地址建立时间为1个HCLK 1/168M=6ns，最小1ns
  readwriteTiming.AddressHoldTime = 0x00;                     //地址保持时间,最小2ns
  readwriteTiming.DataSetupTime = 0x06;                       //读写数据建立时间为2个HCLK 2/168M=12ns,最小4ns
  readwriteTiming.BusTurnAroundDuration = 0x00;               //总线恢复时间，仅用于复用模式的NOR    
  readwriteTiming.CLKDivision = 0x00;                         //设置时钟分频，仅用于同步类型的存储器
  readwriteTiming.DataLatency = 0x00;                         //数据产生时间
  readwriteTiming.AccessMode = FSMC_ACCESS_MODE_B;            //选择匹配为NOR模式
  HAL_SRAM_Init(&hsram1, &readwriteTiming,&readwriteTiming);
}

 uint32_t FSMC_Initialized = 0;

 void HAL_FSMC_MspInit(void){
  /* USER CODE BEGIN FSMC_MspInit 0 */

  /* USER CODE END FSMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct;
  if (FSMC_Initialized) {
    return;
  }
  FSMC_Initialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD11   ------> FSMC_A16
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin 
                          |LCD_D8_Pin|LCD_D9_Pin|LCD_D10_Pin|LCD_D11_Pin 
                          |LCD_D12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = LCD_D13_Pin|LCD_D14_Pin|LCD_D15_Pin|LCD_RS_Pin 
                          |LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin 
                          |LCD_RD_Pin|LCD_SCL_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
  
  /* USER CODE BEGIN FSMC_MspInit 1 */

  /* USER CODE END FSMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FSMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

uint32_t FSMC_DeInitialized = 0;

 void HAL_FSMC_MspDeInit(void){
  /* USER CODE BEGIN FSMC_MspDeInit 0 */

  /* USER CODE END FSMC_MspDeInit 0 */
  if (FSMC_DeInitialized) {
    return;
  }
  FSMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_DISABLE();
  
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD11   ------> FSMC_A16
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */

  HAL_GPIO_DeInit(GPIOE, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin 
                          |LCD_D8_Pin|LCD_D9_Pin|LCD_D10_Pin|LCD_D11_Pin 
                          |LCD_D12_Pin);

  HAL_GPIO_DeInit(GPIOD, LCD_D13_Pin|LCD_D14_Pin|LCD_D15_Pin|LCD_RS_Pin 
                          |LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin 
                          |LCD_RD_Pin|LCD_SCL_Pin|LCD_CS_Pin);

  /* USER CODE BEGIN FSMC_MspDeInit 1 */

  /* USER CODE END FSMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FSMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}
/**
  * @}
  */






/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
