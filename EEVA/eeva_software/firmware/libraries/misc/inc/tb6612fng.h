/* -----------------------------------------------------------------------

-------------------------------------------------------------------------*/

#ifndef TB6612FNG_H
#define TB6612FNG_H

#include "digital_out.h"
#include "pwm_out.h"

class TB6612FNG
{
  public:
    //constructor
    TB6612FNG(void);

    //destructor
    virtual ~TB6612FNG();

    //functions
    void setDutyA(float duty); //Set duty( -1 to 1)    
    void setDutyB(float duty); //Set duty( -1 to 1)    

  private:
    DigitalOut AIN1, AIN2, BIN1, BIN2;// STBY;
    PwmOut pwm;
};

#endif