/* -----------------------------------------------------------------------
* The DigitalOut interface is used to configure and control a digital output pin.

-------------------------------------------------------------------------*/

#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H

#include "stm32f4xx_gpio.h"

// These are the names for the input pins. 

typedef enum 
{
  PA0,  // left motor sign 
  PC3,  // right motor sign
} DigIn_TypeDef;

// You can define the operating Pull-up/Pull down to be:     
//  GPIO_PuPd_NOPULL
//  GPIO_PuPd_UP
//  GPIO_PuPd_DOWN
// Default is NOPULL  

class DigitalIn
{
  public:
    //constructors
    explicit DigitalIn(DigIn_TypeDef pin, 
                       GPIOPuPd_TypeDef PuPd = GPIO_PuPd_NOPULL);

    //destructor
    virtual ~DigitalIn();

    //functions
    bool read(void) {return ( (d_PORT->IDR & d_GPIO_PIN) == d_GPIO_PIN ); };
    
  private:
    
    // The pin details
    uint16_t d_GPIO_PIN;
    uint32_t d_RCC_AHB1Periph;
    GPIO_TypeDef* d_PORT;    
};

#endif