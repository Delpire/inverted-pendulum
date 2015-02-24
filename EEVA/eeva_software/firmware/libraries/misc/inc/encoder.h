/* -----------------------------------------------------------------------
// Copyright: Dale Schinstock, 2013

The Encoder interface is used to implement a quadrature encoder interface with
4x counting, and a signed 32 bit count. To keep things simple, there are only
two options: 1) TIM1 with PE9 and PE11, and 2) TIM2 with PA1 and PA15.

In order to keep a proper 32 bit count the "read" method must be called 
frequently enough that the encoder cannot change more 0x8000 counts between
calls.

NEEDS stm32f4xx_gpio.c, stm32f4xx_rcc.c, and stm32f4xx_tim.c from std periph lib

EXAMPLE: 

#include "Encoder.h"

int main(void)
{
  float angle1, angle2;
  const float res1=0.225; // deg/count resolution of a 400 line with 4x counting
  const float res2=0.18;  // deg/count resolution of a 500 line with 4x counting

  Encoder enc1(EncoderA);
  Encoder enc2(EncoderB);

  enc1.set(0);                   // reset the encoders to zero counts
  enc2.set(0);

  while(1) {
    angle1 = enc1.read()*res1;
    angle2 = enc2.read()*res2;
  }
}

-------------------------------------------------------------------------*/

#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx.h"   // include defs for stm32f4xx including std periph lib

// These are possible settups for an encoder
typedef enum 
{
  EncoderA,           // Uses PC6 and PC7 on TIM3 as encoder channels
  EncoderB            // Uses PD12 and PD13 on TIM4 as encoder channels
} EncoderID_TypeDef;

class Encoder
{
  public:
    //constructor
    explicit Encoder(EncoderID_TypeDef EncoderID);

    //destructor
    virtual ~Encoder();

    //functions
    int32_t read(void);        // read the count and look for overflow/underflow    
    void set(int32_t count32); // set the current count to a value

  private:
    void EncoderInit(EncoderID_TypeDef EncoderID);
    EncoderID_TypeDef ThisEncoderID;
    uint16_t prev_counter;      // counter value from previous read
    int16_t overflows;          // number of overflows/underflows (for 32 bit count)
};

#endif