/****************************************************************************************
* File: user_leds.h
*
* Description: Header file for interface to the four user leds on the EEVA board
*              orange(PC13), yellow(PC14), green(PC15), red(PE1)
****************************************************************************************/

#ifndef USER_LEDS_H
#define USER_LEDS_H

#include "stm32f4xx.h"

// These are the names for the green, orange, red, and blue LEDs
// Note that the indicated pins also turn on/off
typedef enum
{
  orange,   // PC13
  yellow,   // PC14
  green,    // PC15
  red       // PH0
} Led_TypeDef;

class UserLeds
{
  public:
    //constructors
    explicit UserLeds(void);

    //destructor
    virtual ~UserLeds();

    //functions
    void set(Led_TypeDef led);    // Turns on 
    void clear(Led_TypeDef led);  // Turns off
    void toggle(Led_TypeDef led); // Toggles

  private:

    // states of the leds
    uint8_t orangeState, yellowState, greenState, redState;
};

#endif  //USER_LEDS_H