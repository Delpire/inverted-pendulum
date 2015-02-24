/****************************************************************************************
* File: user_leds.cpp
*
* Description: Interface to the four user leds on the EEVA board
*              orange(PC13), yellow(PC14), green(PC15), red(PE1)
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "user_leds.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
UserLeds::UserLeds(void)
{
  // GPIOD Periph clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  // Configure the GPIO_LED pins
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  orangeState = yellowState = greenState  = redState = 1;
  //set bits low - turn on leds
  GPIOC->BSRRH = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIOE->BSRRH = GPIO_Pin_1;
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
UserLeds::~UserLeds()
{
}

/*****************************************************************************
* Function:
*
* Description: clear the bit -- the LEDs are active low
* reentrant and thread safe
*****************************************************************************/
void UserLeds::set(Led_TypeDef led)
{
  switch(led)
  {
    case orange:
      GPIOC->BSRRH = GPIO_Pin_13;
      break;
    case yellow:
      GPIOC->BSRRH = GPIO_Pin_14;
      break;
    case green:
      GPIOC->BSRRH = GPIO_Pin_15;
      break;
    case red:
      GPIOE->BSRRH = GPIO_Pin_1;
      break;
  }
}

/*****************************************************************************
* Function:
*
* Description: set the bit -- the LEDs are active low
* reentrant and thread safe
*****************************************************************************/
void UserLeds::clear(Led_TypeDef led)
{
  switch(led)
  {
    case orange:
      GPIOC->BSRRL = GPIO_Pin_13;
      break;
    case yellow:
      GPIOC->BSRRL = GPIO_Pin_14;
      break;
    case green:
      GPIOC->BSRRL = GPIO_Pin_15;
      break;
    case red:
      GPIOE->BSRRL = GPIO_Pin_1;
      break;
  }
}

/*****************************************************************************
* Function:
*
* Description: maybe not reentrant due to  ^=
*****************************************************************************/
void UserLeds::toggle(Led_TypeDef led)
{
  switch(led)
  {
    case orange:
      GPIOC->ODR ^= GPIO_Pin_13;
      break;
    case yellow:
      GPIOC->ODR ^= GPIO_Pin_14;
      break;
    case green:
      GPIOC->ODR ^= GPIO_Pin_15;
      break;
    case red:
      GPIOE->ODR ^= GPIO_Pin_1;
      break;
  }
}
