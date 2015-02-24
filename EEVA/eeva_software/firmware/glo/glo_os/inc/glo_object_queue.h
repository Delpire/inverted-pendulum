/****************************************************************************************
* File: 
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef GLOOBJECTQUEUE_H
#define GLOOBJECTQUEUE_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/
struct gloObjectQueueType
{
  uint8_t objID; 
  uint16_t instance;
  bool send_all; // if true will send up to and including specified instance
};

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/
class GloObjectQueue
{
  public:
    explicit GloObjectQueue(uint8_t queueSize);
    ~GloObjectQueue(void);
    uint8_t put(uint8_t objID) { return this->put(objID, 1, false); };
    uint8_t put(uint8_t objID, uint16_t instance, bool send_all);
    uint8_t take(uint8_t *objID, uint16_t *instance, bool *send_all);    
    inline uint8_t getnum(void){return num;}          // hopefully inline func
   
  private:
    gloObjectQueueType *queue;
    uint8_t num, top, bottom, size;
};

#endif  //GLOOBJECTQUEUE_H
