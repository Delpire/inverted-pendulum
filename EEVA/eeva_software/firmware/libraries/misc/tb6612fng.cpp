//-------------------------------------------------------------------------
//  Implementation of TB6612FNG driver
// 
//-------------------------------------------------------------------------
#include "tb6612fng.h"

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
TB6612FNG::TB6612FNG(void):
      AIN1 (PE14),  // Initialize digital outputs connected to the chip
      AIN2 (PE13),
      BIN1 (PE15),
      BIN2 (PB10)
//      STBY (PA2)
{
  AIN1.clear();      // should be off, but just to make sure
  AIN2.clear();
  BIN1.clear();
  BIN2.clear();
//  STBY.Set();       // enable output
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
TB6612FNG::~TB6612FNG()
{
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
void TB6612FNG::setDutyA(float duty)
{
  if (duty > 0.0f)
  {
    AIN1.clear();
    AIN2.set();
    pwm.SetDutyA(duty);
  }
  else
  {
    duty *= -1.0f;
    AIN2.clear();
    AIN1.set();
    pwm.SetDutyA(duty);
  }
}

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
void TB6612FNG::setDutyB(float duty)
{
  if (duty > 0.0f)
  {
    BIN1.clear();
    BIN2.set();
    pwm.SetDutyB(duty);
  }
  else
  {
    duty *= -1.0f;
    BIN2.clear();
    BIN1.set();
    pwm.SetDutyB(duty);
  }
}