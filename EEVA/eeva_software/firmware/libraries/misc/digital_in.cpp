//-------------------------------------------------------------------------
//  Implementation of DigitalIn
//-------------------------------------------------------------------------
#include "digital_in.h"

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
DigitalIn::DigitalIn(DigIn_TypeDef pin, 
                     GPIOPuPd_TypeDef PuPd)
{
    // Set the pin data
    switch(pin) {
      case PA0: 
        d_GPIO_PIN = GPIO_Pin_0;
        d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOA;
        d_PORT = GPIOA;
        break;
      case PC3: 
        d_GPIO_PIN = GPIO_Pin_3;
        d_RCC_AHB1Periph = RCC_AHB1Periph_GPIOC;
        d_PORT = GPIOC;
        break;
    }
    
    // GPIOx Periph clock enable
    RCC_AHB1PeriphClockCmd(d_RCC_AHB1Periph, ENABLE);
    
    // Configure the GPIO pin
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = d_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      // doesn't matter - input 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // doesn't matter - input
    GPIO_InitStructure.GPIO_PuPd = PuPd;
    
    GPIO_Init(d_PORT, &GPIO_InitStructure);
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
DigitalIn::~DigitalIn()
{
}

