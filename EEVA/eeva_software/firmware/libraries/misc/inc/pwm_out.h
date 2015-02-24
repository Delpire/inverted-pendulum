/* -----------------------------------------------------------------------

-------------------------------------------------------------------------*/

#ifndef PWMOUT_H
#define PWMOUT_H

#include "stdint.h"

class PwmOut
{
  public:
    //constructor
    PwmOut(void);

    //destructor
    virtual ~PwmOut();

    //functions
    void SetDutyA(float duty);   //Set duty cycle( 0 to 1)    
    void SetDutyB(float duty);   //Set duty cycle( 0 to 1)    

  private:
    uint32_t SmallestDivisor(uint32_t num);
    
    uint32_t auto_reload_reg;       // ARR clock counts to get period  
};

#endif