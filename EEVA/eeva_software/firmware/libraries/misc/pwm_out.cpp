#include "stm32f4xx.h"
#include "pwm_out.h"
#include "math.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
PwmOut::PwmOut(void)
{
  uint32_t frequency = 20000;    // Set frequency to 20 kHz.

  // TIM1 clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  // GPIOA  clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // GPIO Configuration: TIM1 CH1 (PA8), CH2 (PA9)
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Connect TIM1 pins using alternate functions  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);

  // DO SOME MESSY MATH TO TRY TO OPTIMIZE THE PRESCALAR AND AUOTO RELOAD REGISTER
  // The timer freq = timer_input_clock / ((prescalar +1)*(auto_reload_reg +1))
  // We want a large auto_reload_reg for good resolution on the timer
  uint32_t prescalar;
  
  // This should be set to the timer's clock freq (Hz) from the RCC
  // Could maybe write a function to determine this but it is very messy 
  uint32_t timer_input_clock = 72000000;    
  // This should be 0x1<<16 (16 bit timers) or 0x1<<32 (32 bit timers)
  uint64_t arr_max = (uint64_t)0x1<<32;
  // max prescalar + 1
  uint64_t prescale_max = 0x1<<16;
  
  // This is a total scaling divisor that will give the nearest freq to the
  // the desired, accounting for rounding
  uint64_t scaling_product = uint64_t((double)timer_input_clock/frequency +0.5);
  
  if (scaling_product < 100)  // desired freq is too high for good resolution
  {
    auto_reload_reg = 99;     // so decide to ensure resolution is at least 1%
    prescalar = 0;            // of period
  }
  else if (scaling_product > (arr_max*prescale_max))  // freq is too low
  {
    auto_reload_reg = arr_max-1;                     // so set freq to min
    prescalar = prescale_max-1;
  }
  else if (scaling_product <= arr_max)   // scaling can all be done in auto_reload_reg
  {
    auto_reload_reg = scaling_product-1; // so get best resolution possible by
    prescalar = 0;                       // doing no prescaling of the clock
  }
  else                                   // only get here if arr_max > scaling_product
  {
    prescalar = SmallestDivisor(scaling_product);
    auto_reload_reg = scaling_product/prescalar - 1;
    prescalar -= 1;
  }
    
  // Time base configuration
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = auto_reload_reg;
  TIM_TimeBaseStructure.TIM_Prescaler = prescalar;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  // PWM Mode configuration
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
  // Channel 1
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  // Channel 2
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);    
  
  TIM_ARRPreloadConfig(TIM1, ENABLE);

  TIM1->BDTR |= 0x8000; // Make sure the MOE bit is set to enable outputs
                        // on advanced timers.  Note TIM1 is an advanced timer.

  // TIM1 enable counter
  TIM_Cmd(TIM1, ENABLE);  
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
PwmOut::~PwmOut()
{
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
void PwmOut::SetDutyA(float duty)
{
  if (duty > 1.0f) duty = 1.0f;
  if (duty < 0.0f) duty = 0.0f;
  TIM1->CCR2 = (uint32_t) (duty*auto_reload_reg);
}
/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
void PwmOut::SetDutyB(float duty)
{
  if (duty > 1.0f) duty = 1.0f;
  if (duty < 0.0f) duty = 0.0f;
  TIM1->CCR1 = (uint32_t) (duty*auto_reload_reg);
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
uint32_t PwmOut::SmallestDivisor (uint32_t num)
{
  if ( num % 2 == 0)  return(2);

  uint16_t sqrt_of_num = (uint16_t)sqrtf((float)num);
  for (uint16_t i = 3; i <= sqrt_of_num; i += 2)
  {
    if ( num % i == 0) return (i);
  }
  return(num);
}
