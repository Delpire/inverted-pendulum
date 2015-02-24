/* -----------------------------------------------------------------------
* The DigitalOut interface is used to configure and control a digital output pin.

-------------------------------------------------------------------------*/

#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include "stm32f4xx_gpio.h"

// These are the names for the output pins. 

typedef enum 
{
  PE13,  // TB6612FNG Ain2 
  PE14,  // TB6612FNG Ain1
  PE15,  // TB6612FNG Bin1
  PB10,  // TB6612FNG Bin2

  PB4,  // main control task watch 
  PB5,  // complementary filter task watch
  PB8,  // communications task watch
  PB9,  // leds task watch
} DigOut_TypeDef;

// You can specify the operating output Otype to be:
//  GPIO_OType_PP
//  GPIO_OType_OD
// Default is PP
    
// You can define the operating Pull-up/Pull down to be:     
//  GPIO_PuPd_NOPULL
//  GPIO_PuPd_UP
//  GPIO_PuPd_DOWN
// Default is NOPULL  

class DigitalOut
{
  public:
    //constructors
    //DigitalOut(myDigitalOut_TypeDef pin);
    explicit DigitalOut(DigOut_TypeDef pin, 
               GPIOOType_TypeDef Otype = GPIO_OType_PP,
               GPIOPuPd_TypeDef PuPd = GPIO_PuPd_NOPULL,
               uint8_t initValue=0);

    //destructor
    virtual ~DigitalOut();

    //functions
    bool read(void) {return ( (d_PORT->ODR & d_GPIO_PIN) == d_GPIO_PIN ); };
    void set(void) { d_PORT->BSRRL = d_GPIO_PIN; };
    void clear(void) { d_PORT->BSRRH = d_GPIO_PIN; };
    
  private:
    
    // The pin details
    uint16_t d_GPIO_PIN;
    uint32_t d_RCC_AHB1Periph;
    GPIO_TypeDef* d_PORT;    
};

#endif