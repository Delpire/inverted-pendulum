/****************************************************************************************
* File: green_leds.h
*
* Description: Interface to the eight green leds on the EEVA board
*              PB12-PB15, and PD8-PD11
****************************************************************************************/

#ifndef GREEN_LEDS_H
#define GREEN_LEDS_H

#include "stm32f4xx_gpio.h"

class GreenLeds
{
  public:
    //constructors
    explicit GreenLeds(void);
      
    //destructor
    virtual ~GreenLeds();

    //functions
    void set(uint8_t); 
    
  private:
    
    // The state of the leds
    uint8_t state;
};

#endif