//-------------------------------------------------------------------------
//  Implementation of Encoder.
//-------------------------------------------------------------------------
#include "encoder.h"

// Constructor
Encoder::Encoder(EncoderID_TypeDef EncoderID)
{
 ThisEncoderID = EncoderID;
 prev_counter = 0;
 overflows = 0;
 EncoderInit(ThisEncoderID);
}

void Encoder::EncoderInit(EncoderID_TypeDef EncoderID)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  switch(ThisEncoderID){
    case EncoderA:                   // CONFIGURE TIM1 AS ENCODER INTERFACE
    
      // GPIOC clock enable
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
      // TIM3 clock enable
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;    // no pull up or pull down resistor
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      TIM_SetAutoreload(TIM3, 0xffff);
      TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
      
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);   // Assign PC6 and PC7
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);   // to TIM3
       
      TIM_Cmd(TIM3, ENABLE);
      
      TIM3->CCMR1 |= 0xF0F0;  // Implements digital filter
      
      break;
  
    case EncoderB:                   // CONFIGURE TIM2 AS ENCODER INTERFACE
      // GPIOD clock enable
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
      // TIM4 clock enable
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // Alternate function mode
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // Speed doesn't matter (input)
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;       // Open Drain - but shouldn't matter (input)
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;    // no pull up or pull down resistor
      GPIO_Init(GPIOD, &GPIO_InitStructure);
      
      TIM_SetAutoreload(TIM4, 0xffff);
      TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
      
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);   // Assign PD12 and PD13
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);   // to TIM4
       
      TIM_Cmd(TIM4, ENABLE);
      
      TIM4->CCMR1 |= 0xF0F0;   // Implements digital filter

      break;
  }
}

// Destructor
Encoder::~Encoder()
{
}

int32_t Encoder::read(void)
{
  uint16_t counter;
  int32_t count32;
  
  switch(ThisEncoderID){
    case EncoderA:                   // Using TIM3
      counter = TIM3->CNT;
      break;
    case EncoderB:                   // Using TIM4
      counter = TIM4->CNT;
      break;
  }
  
  // The following assumes this function is called frequently enough that
  // the encoder cannot change more 0x8000 counts between calls, and that
  // the counter overflows from 0xffff to 0 and underflows from 0 to 0xffff
  
  if ((prev_counter > 0xc000)&&(counter < 0x4000))     // overflow
    overflows += 1;
  else if ((prev_counter < 0x4000)&&(counter > 0xc000))  // underflow
     overflows -= 1;
 
  count32 = overflows*0x10000 + counter;
  prev_counter = counter;

  return (count32);
}

void Encoder::set(int32_t count32)
{
  uint16_t counter;
  
  if (count32 <0)
    overflows = count32/((int32_t)0x10000) - 1;
  else 
    overflows = count32/((int32_t)0x10000);
  
  counter = (uint16_t)(count32 - overflows*0x10000);
  prev_counter = counter;

  switch(ThisEncoderID){
    case EncoderA:                   // Using TIM3
      TIM3->CNT = counter;
      break;
    case EncoderB:                   // Using TIM4
      TIM4->CNT = counter;
      break;
  }
}
