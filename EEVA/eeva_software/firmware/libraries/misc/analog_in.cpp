/****************************************************************************************
* File: analog_in.cpp
*
* Description: Setup ADC1 to scan the signals on pins below continuously using DMA to
*              write the values to the a buffer.  The channels and the order in which
*              they are scanned and placed in the buffer are listed below.
*
* Created: 
****************************************************************************************/
// Runs at 12032Hz for each 11 channel scan -- verified with testing.
// PIN  EEVA-SIGNAL     ADCCHANNEL
// PA1	QTR_1	        ADC1_IN1
// PA2	QTR_2	        ADC1_IN2
// PA3	QTR_3	        ADC1_IN3
// PA4	QTR_4	        ADC1_IN4
// PA5	QTR_5	        ADC1_IN5
// PA6	QTR_6	        ADC1_IN6
// PA7	QTR_7	        ADC1_IN7
// PC4	QTR_8	        ADC1_IN14
// PC5	BATT_SENS	ADC1_IN15
// PB0	MOTORB_SENS	ADC1_IN8
// PB1	MOTORA_SENS	ADC1_IN9

#include "analog_in.h"

/*****************************************************************************
* Function: Constructor
*
* Description: 
*****************************************************************************/
AnalogIn::AnalogIn(void)
{
  
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  uint32_t ComponentsUsed;
  
  //* Enable clocks ****************************************
  ComponentsUsed =  RCC_AHB1Periph_DMA2
                    |RCC_AHB1Periph_GPIOA
                    |RCC_AHB1Periph_GPIOB
                    |RCC_AHB1Periph_GPIOC; // AHB1 components
  RCC_AHB1PeriphClockCmd(ComponentsUsed, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  //* DMA2 Stream0 channel0 configuration **************************************
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)d_ADCRawValues;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 11;                                  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  //* Configure pins on GPIOs as analog input ******************************
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
                                |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6
                                |GPIO_Pin_7;            // PA pins
  GPIO_Init(GPIOA, &GPIO_InitStructure);                  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  // PB pins
  GPIO_Init(GPIOB, &GPIO_InitStructure);                  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;  // PC pins
  GPIO_Init(GPIOC, &GPIO_InitStructure);                  

  //* ADC Common Init **********************************************************
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;           // 72Mhz(APB2)/8=9Mhz
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  //* ADC1 Init ****************************************************************
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;           
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // no external trigger
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;     // Shouldn't matter
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      // 12 bit right aligned
  ADC_InitStructure.ADC_NbrOfConversion = 11;                                 // 11 channels
  ADC_Init(ADC1, &ADC_InitStructure);

  //  ADC1 regular channels configuration *************************************
  //  Tconv = (56 +12)/9MHz, so 11 channels conversion scan at 12032 Hz
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 8, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 9, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 10, ADC_SampleTime_56Cycles);  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 11, ADC_SampleTime_56Cycles);  

  //  Enable DMA request after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  //  Enable ADC1 DMA 
  ADC_DMACmd(ADC1, ENABLE);
  
  //  Enable ADC1 
  ADC_Cmd(ADC1, ENABLE);
  
  ADC1->CR2 |= 0x40000000;  //Start Conversion of regular channels
  
//  setupInterrupt();  // only for testing
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
AnalogIn::~AnalogIn()
{
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
void AnalogIn::getVoltages(float voltages[11])
{
  for (uint8_t i=0; i<11; i++)
  {
    voltages[i] = d_ADCRawValues[i]*3.3f/0xFFF;
  }
} 

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
void AnalogIn::setupInterrupt(void)  // not usually called - just for testing
{
  // enable the Transfer Complete interrupt on DMA
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);    
// Handled in glo_os
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************
* Function:
*
* Description: only enable when testing the speed of ADC
*****************************************************************************/
//extern "C" void DMA2_Stream0_IRQHandler(void)  
void DMA2_Stream0_ISR(void)
{                                              
 static uint32_t count;

  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);  
    count++;
    if (count>1203200)
      count--;           // Set break point here to time 100 seconds
  }
}
