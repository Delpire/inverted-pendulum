// Copyright: Dale Schinstock, 2013

#include "glo_object_queue.h"
#include "glo_objects.h"

//*****************************************************************************
GloObjectQueue::GloObjectQueue(uint8_t queueSize):
  num(0), top(0), bottom(0), size(queueSize)
{
  queue = new gloObjectQueueType[queueSize];
}

//*****************************************************************************
GloObjectQueue::~GloObjectQueue(void)
{
  delete(queue);
}

//*****************************************************************************
uint8_t GloObjectQueue::put(uint8_t objID, uint16_t instance, bool send_all)  // reentrant and threadsafe
{
  uint32_t disabled;
  if ((objID <= NUM_GLO_OBJECTS)&&(instance <= gloObjectPtrs[objID]->get_num_instances()))
  {
     disabled = __get_PRIMASK();
     __disable_irq();
     num++;
     if (num > size)
     {
       num--;
       if( !disabled )
         __enable_irq();
       return(0);
     }
     top++;
     if (top == size) top = 0;
     queue[top].objID = objID;
     queue[top].instance = instance;
     queue[top].send_all = send_all;

     if( !disabled )
       __enable_irq();
     return(1);
  }
  else return(0);
}

//*****************************************************************************
uint8_t GloObjectQueue::take(uint8_t *objID, uint16_t *instance, bool *send_all) // reentrant and threadsafe
{
  uint32_t disabled;
  if(num)
  {
    disabled = __get_PRIMASK();
    __disable_irq();
    bottom++;
    if (bottom == size) bottom = 0;
    num--;
    *objID = queue[bottom].objID;
    *instance = queue[bottom].instance;
    *send_all = queue[bottom].send_all;
    if( !disabled )
      __enable_irq();
    return(1);
  }
  else return(0);
}
