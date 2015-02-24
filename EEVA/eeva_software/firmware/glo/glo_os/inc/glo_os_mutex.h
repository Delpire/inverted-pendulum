/****************************************************************************************
* File: 
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef GLOOSMUTEX_H
#define GLOOSMUTEX_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

class GloOSTaskBase;

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/
class GloOSMutex
{
  public:
    explicit GloOSMutex(void)
    {
      mutexTaken = false;
    }

    friend class GloOS;   // allow gloOS complete access
    
  private:
    bool mutexTaken;
    GloOSTaskBase *owner;
};

#endif  //GLOOSMUTEX_H
