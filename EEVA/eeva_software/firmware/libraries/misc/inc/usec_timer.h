/* -----------------------------------------------------------------------

-------------------------------------------------------------------------*/

#ifndef USECTIMER_H
#define USECTIMER_H

#include "stdint.h"

class UsecTimer
{
  public:
    //constructor
    explicit UsecTimer(void);

    //destructor
    virtual ~UsecTimer();

    //functions
    void UsecDelay(uint16_t usecs);   // delays or usecs microseconds ( 0 to 32767)    

  private:

};

#endif