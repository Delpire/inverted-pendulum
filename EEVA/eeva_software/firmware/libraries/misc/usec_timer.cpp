#include "stm32f4xx.h"
#include "usec_timer.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
UsecTimer::UsecTimer(void)
{
  // TIM11 clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);

  // This should be set to the timer's clock freq (Hz) from the RCC
  // Could maybe write a function to determine this but it is very messy 
  uint32_t timer_input_clock = 72000000;

  // Set the timer clock to 1 MHz
  uint16_t prescalar = timer_input_clock/1000000 - 1;
    
  // Time base configuration
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = prescalar;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);
  
  TIM_ARRPreloadConfig(TIM11, ENABLE);

  // TIM2 enable counter
  TIM_Cmd(TIM11, ENABLE);  
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
UsecTimer::~UsecTimer(void)
{
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
void UsecTimer::UsecDelay(uint16_t usecs)
{
  // limit time delay to 32767 usecs (half of counter) to make things easy
  if (usecs > 0x7FFF) usecs = 0x7FFF; 
  
  uint16_t begin = TIM11->CNT;
  uint32_t end = begin + usecs;
  uint32_t count=begin;
    
  if (end < 0x10000)                         // counter should not turn over
  {
    while ((count < end)&&(count >= begin)) // && should prevent missing counter reset
      count = TIM11->CNT;                   // unless interrupted for > 32768 usec 
  }
  else                                      // counter will turn over;
  {  
    end -= 0x10000;
    while ((count < end)||(count >= begin))
      count = TIM11->CNT;
  }
}
  
