/****************************************************************************************
* File: glo_os.cpp
*
* Description: 
*
* Created: 
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os.h"
#include "glo_os_mutex.h"

/*---------------------------------------------------------------------------------------
*                                     Methods - GloOS
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: GloOS constructor
*
* Description:
*****************************************************************************/

GloOS::GloOS(uint32_t sysTimerFrequency, uint8_t sysTickPriority)
{
  __disable_irq();  // disable interrupts  will enable in startOS

  os_started = false; // prevent task scheduler from running

  for (uint8_t i=0; i<NUM_IRQN; i++)
  {
    irq2taskMap[i] = 255;          // no task for most IRQs
    irqFuncPtrs[i] = 0;            // null out all pointers
  }
  for (uint8_t i=0; i<GLOOSMAXNUMTASKS; i++)
  {
    taskPtrs[i] = 0;               // null out points
  }

  numTasks = 0;

  if (sysTickPriority > 14) this->sysTickPriority = 14;
  else if (sysTickPriority < 1) this->sysTickPriority = 1;
  else this->sysTickPriority = sysTickPriority;
  
  sysTimerSetup(sysTimerFrequency, this->sysTickPriority); // Sets up interrupt

  // Use all bits of interrupt priority to be preemption (16 priority levels)
  SCB->AIRCR = 0x05FA0300;

}

//*****************************************************************************
bool GloOS::registerTask(taskRegistrationType const & config)
{
  if (numTasks >= GLOOSMAXNUMTASKS)       return false;
  if (config.priority >= sysTickPriority) return false;
  if (irq2taskMap[config.IRQnum] != 255)  return false;  // need to find a diff IRQnum

  irq2taskMap[config.IRQnum] = numTasks;

  taskPtrs[numTasks] = config.taskObjectPtr;
  taskPtrs[numTasks]->taskInfo.IRQnum = config.IRQnum;
  taskPtrs[numTasks]->taskInfo.taskNum = numTasks;
  taskPtrs[numTasks]->taskInfo.taskType = config.taskType;
  taskPtrs[numTasks]->taskInfo.isIsrTask = config.isIsr;
  taskPtrs[numTasks]->taskInfo.priority = config.priority;
  taskPtrs[numTasks]->taskInfo.enabled = config.enabled;
  
  // prevent divide by zero
  uint32_t delay = 0;
  if (config.frequency != 0)
  {
    delay = (uint32_t)(SYSTICK_FREQUENCY / config.frequency); 
  }

  taskPtrs[numTasks]->taskInfo.delay = delay;
  

  if (config.taskType == queued)
  {
    if(config.taskObjectPtr->queue == 0)
      return(false);  // must have a GloObjectQueue
  }
  else
    config.taskObjectPtr->queue = 0;

  numTasks++;

  return true;
}

//*****************************************************************************
bool GloOS::registerSysTickTask(GloOSTaskBase * taskObjectPtr)
{
  if (sysTickTaskPtr != NULL) return false; // Already registered.

  sysTickTaskPtr = taskObjectPtr;

  sysTickTaskPtr->taskInfo.IRQnum = SysTick_IRQn;
  taskPtrs[numTasks]->taskInfo.taskNum = 254;
  taskPtrs[numTasks]->taskInfo.taskType = periodic;
  taskPtrs[numTasks]->taskInfo.isIsrTask = false;
  taskPtrs[numTasks]->taskInfo.priority = sysTickPriority;
  taskPtrs[numTasks]->taskInfo.delay = 0;

  return true;
}

//*****************************************************************************
bool GloOS::registerISR(isrRegistrationType const & config)
{

  if (config.priority > 15)               return false;
  if (irq2taskMap[config.IRQnum] != 255)  return false;  // IRQnum used for a task
  if (irqFuncPtrs[config.IRQnum] != 0)    return false;  // IRQnum already registered

  irqFuncPtrs[config.IRQnum] = config.callback;
  irqPriorities[config.IRQnum] = config.priority;

  return true;
}

//*****************************************************************************
void GloOS::startOS(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  uint8_t NVICpriority;

  if (os_started) return;    // already started
  
  // Use all bits of interrupt priority to be preemption (16 priority levels)
  // redundant with with constructor, but want to make sure this is done
  // after ST's USB driver is initialized, it screws this up I think
  SCB->AIRCR = 0x05FA0300;

  __enable_irq();  // enable interrupts - at this point only sysTick should be active
  
  // call initialize methods for each enabled task
  for(uint8_t i=0; i<numTasks; i++)
  {
    if (taskPtrs[i]->taskInfo.enabled)
    {
      taskPtrs[i]->initialize();
    }
  }
  
  // start the ISRs
  for (uint8_t i=0; i<NUM_IRQN; i++)
  {
    if (irqFuncPtrs[i] != 0)                     // ISR callback registered
    {
      NVICpriority = 0x0F - irqPriorities[i];
      NVIC_InitStructure.NVIC_IRQChannel = (IRQn_Type)i;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVICpriority;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   // subpriority not used for us
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
    }
  }
  // start the tasks
  for(uint8_t i=0; i<numTasks; i++)
  {
    GloOSTaskBase *taskPtr = taskPtrs[i];
    
    if (taskPtr->taskInfo.enabled)
    {
      NVICpriority = 0x0F - taskPtr->taskInfo.priority;
      NVIC_InitStructure.NVIC_IRQChannel = taskPtr->taskInfo.IRQnum;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVICpriority;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;   // subpriority not used for us
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
    }
  }

  os_started = true; // allow task scheduler to run
}

//*****************************************************************************
void GloOS::dispatcher(IRQn_Type IRQnum)  //TODO: check thorough for reentrant
{                                         //Called only by stm32f4xx_interrupts
  if (IRQnum == SysTick_IRQn)      // The special timer tick task
  {
    ticks++;

    if (sysTickTaskPtr != 0)       // Call the task if exists
    {
      sysTickTaskPtr->tickStamp = ticks;
      sysTickTaskPtr->taskInfo.runCounts++;
      sysTickTaskPtr->taskRun();
    }

    if( os_started )            // only call task scheduler after os started
        taskScheduler();        // look at other tasks that may need to run
  }
  else
  {
    uint8_t taskNum = irq2taskMap[IRQnum];

    if (taskNum < 255)           // this is a task, not a simple ISR
    {
      GloOSTaskBase *taskPtr = taskPtrs[taskNum];
      if(taskPtr == 0)             // no pointer for this task
        while(1);                  // trap this

      if ((taskPtr->taskInfo.taskType != periodic)|| // periodic counters zeroed
          (taskPtr->taskInfo.isIsrTask))             // in scheduler for best
      {                                              // timing, but periodic ISR
        taskPtr->taskInfo.delayCounter = 0;          // and other tasks should
      }                                              // should zeroed every run

      taskPtr->tickStamp = ticks;
      taskPtr->taskInfo.runCounts++;
      taskPtr->taskRun();                     // run it
    }
    else                                      // simple ISR
    {
      if(irqFuncPtrs[IRQnum] == 0)   // no pointer for this IRQ
        while(1);                    // trap this

      irqFuncPtrs[IRQnum]();         // run it
    }
  }
}

//*****************************************************************************
void GloOS::taskScheduler(void)      // not reentrant
{                                    // called only by sysTick
  for(uint8_t i=0; i<numTasks; i++)
  {
    taskPtrs[i]->taskInfo.delayCounter++; // do for all, only used for periodic
                                          // tasks in the scheduler, although
                                          // we might add a "postponed" type task

    switch (taskPtrs[i]->taskInfo.taskType)
    {
      case notScheduled:                  // only triggered by hardware IRQ, or
        break;                            // an external call to setPending
      case periodic:
        if (taskPtrs[i]->taskInfo.delayCounter >= taskPtrs[i]->taskInfo.delay)
        {
          taskPtrs[i]->taskInfo.delayCounter = 0;
          NVIC_SetPendingIRQ(taskPtrs[i]->taskInfo.IRQnum);
        }
        break;
      case queued:
        if (taskPtrs[i]->queue->getnum())  // Maybe redundant with queue.put()
        {                                  // if the task clears the queue.  But
          NVIC_SetPendingIRQ(taskPtrs[i]->taskInfo.IRQnum); // shouldn't hurt
        }                                              // and prevents lockouts
        break;
    }
  }
}

//*****************************************************************************
void GloOS::sysTimerSetup(uint32_t SysTickFrequency, uint8_t priority)
{
  RCC_ClocksTypeDef RCC_Clocks;

  ticks = 0;
  freq = SysTickFrequency;

  // Use all bits of interrupt priority to be preemption (16 priority levels)
  SCB->AIRCR = 0x05FA0300;

  // SysTick configuration
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / SysTickFrequency);
  SCB->SHP[11]=(15-priority)<<4;
}

//*****************************************************************************
bool GloOS::mutexTake(GloOSTaskBase *requester, GloOSMutex *mutex)
{
  bool success = true;
  uint32_t disabled = __get_PRIMASK();
  __disable_irq();
  if (!(mutex->mutexTaken))
  {
    mutex->owner = requester;
    mutex->mutexTaken = true;
  }
  else
    success = false;
  if( !disabled )
    __enable_irq();
  return (success);
}

//*****************************************************************************
void  GloOS::mutexGive(GloOSTaskBase *requester, GloOSMutex *mutex)
{
  if (requester == mutex->owner)
    mutex->mutexTaken = false;
}

//*****************************************************************************
uint32_t GloOS::disableInterrupts( void )
{
    uint32_t disabled = __get_PRIMASK();
    __disable_irq();
    return disabled;
}

//*****************************************************************************
void GloOS::restoreInterrupts( uint32_t context )
{
    if( !context )
    {
        __enable_irq();
    }
}

/*---------------------------------------------------------------------------------------
*                                     Methods - GloOSTaskBase
*--------------------------------------------------------------------------------------*/

void GloOSTaskBase::setPending(void)
{
  NVIC_SetPendingIRQ(taskInfo.IRQnum);
}

//*****************************************************************************
void GloOSTaskBase::getTaskInfo(taskInfoType *copy)
{
  memcpy((void *)copy, (void const *)&taskInfo, sizeof(taskInfoType));
}

//*****************************************************************************
uint8_t GloOSTaskBase::putQueue(uint8_t objID, uint16_t instance, bool send_all)
{
  if (queue != 0)
  {
    if (queue->put(objID, instance, send_all))
    {
      NVIC_SetPendingIRQ(taskInfo.IRQnum);
      return(1);
    }
  }
  return(0);
}

