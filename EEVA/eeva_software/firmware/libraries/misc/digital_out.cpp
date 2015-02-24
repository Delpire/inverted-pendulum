//-------------------------------------------------------------------------
//  Implementation of DigitalOut
//-------------------------------------------------------------------------
#include "digital_out.h"

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
DigitalOut::DigitalOut(DigOut_TypeDef pin, 
                       GPIOOType_TypeDef Otype,
                       GPIOPuPd_TypeDef PuPd,
                       uint8_t initValue)
{
  // Set the pin data
  switch(pin) {
    case PE13: 
      d_GPIO_PIN = GPIO_Pin_13;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOE;
      d_PORT = GPIOE;
      break;
    case PE14: 
      d_GPIO_PIN = GPIO_Pin_14;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOE;
      d_PORT = GPIOE;
      break;
    case PE15: 
      d_GPIO_PIN = GPIO_Pin_15;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOE;
      d_PORT = GPIOE;
      break;
    case PB10: 
      d_GPIO_PIN = GPIO_Pin_10;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
      d_PORT = GPIOB;
      break; 
    case PB4: 
      d_GPIO_PIN = GPIO_Pin_4;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
      d_PORT = GPIOB;
      break; 
    case PB5: 
      d_GPIO_PIN = GPIO_Pin_5;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
      d_PORT = GPIOB;
      break; 
    case PB8: 
      d_GPIO_PIN = GPIO_Pin_8;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
      d_PORT = GPIOB;
      break; 
    case PB9: 
      d_GPIO_PIN = GPIO_Pin_9;
      d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
      d_PORT = GPIOB;
      break; 
  }
  
  // GPIOx Periph clock enable
  RCC_AHB1PeriphClockCmd(d_RCC_AHB1Periph, ENABLE);

  // Configure the GPIO pin
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = d_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = Otype;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = PuPd;
  
  GPIO_Init(d_PORT, &GPIO_InitStructure);

  if(initValue) {
    set();
  }
  else {
    clear();
  }
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
DigitalOut::~DigitalOut()
{
}

