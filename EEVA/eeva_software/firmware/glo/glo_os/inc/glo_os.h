/****************************************************************************************
* File: glo_os.h
*
* Description:
*
* Created:
****************************************************************************************/

#ifndef GLO_OS_H
#define GLO_OS_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "glo_object_queue.h"
#include <string.h>
class GloOSMutex;      // forward declare
class GloOS;           // forward declare

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/
#define GLOOSMAXNUMTASKS 12

#define NUM_IRQN 90              // number of possible IRQn's on known list of stm32f4xx

#define SYSTICK_FREQUENCY ( 10000 )

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/
// Each task object will contain this structure
// This is set by gloOS - task should not modify
enum taskTypeEnum { notScheduled, periodic, queued};
struct taskInfoType{
    IRQn_Type IRQnum;
    uint8_t taskNum;        // unique task identifier
    taskTypeEnum  taskType; // notScheduled must be ISR or triggered elsewhere
                            // an ISR task could also be periodic or queued
    bool  isIsrTask;        // true for ISR tasks (i.e. triggered by hardware)
    uint32_t delay;         // number of delay ticks used for periodic tasks
                            // 0 or 1=sysTick freq, 2=1/2 sysTick, ...
    uint8_t priority;       // from 0 to SYSTICKPRIORITY-1, 15=highest priority
    uint32_t delayCounter;  // counter used for periodic tasks
    uint32_t runCounts;     // incremented each time thread runs
    bool enabled;           // is task enbled
};

/******************************************************************************
* Class: GloOSTaskBase
*
* Description:  Base class for a task. Each task should inherit from this.
******************************************************************************/
class GloOSTaskBase
{
  public:

    // can be used to tell scheduler to trigger the task
    inline void setPending(void); 
    
    // get task info
    void getTaskInfo(taskInfoType *copy); 
    
    // used if task uses a gloObject queue
    uint8_t putQueue(uint8_t objID, uint16_t instance, bool send_all); 

    // sysTick count set at dispatch of task
    uint64_t tickStamp;    
    
    // allow GloOS complete access
    friend class GloOS;   

  protected:         // accessible in the task and glo_os, but not outside

    // IS the task - must be defined in child. Should only be called by gloOS dispatcher
    virtual void taskRun(void)=0;

    // called by glo_os->startOS() before starting the scheduler. Must be defined in 
    // child.  Only the SysTick interrupt will be running at that point.
    virtual void initialize(void)=0;  

    // pointer to queue if used
    GloObjectQueue *queue;  

    uint32_t get_delay_counts(void) const { return taskInfo.delay; }

    // return the number 
    float get_delay_time(void) const { return (float)taskInfo.delay / (float)SYSTICK_FREQUENCY; }

    float get_run_frequency(void) const { return (float)SYSTICK_FREQUENCY / (float)taskInfo.delay; }

    // Return true if it's time to process a section of code at the specified rate.
    // Usage example:
    // if (ProcessingHz(5))
    // {
    //     some code to process at 5Hz
    // }
    bool ProcessingHz
        (
            uint32_t rate_hz // Rate to process in hertz. If zero then always returns false;
        )
    {
        if (rate_hz == 0) { return false; }
        
        // If the current run counts is equal to a multiple of the mod counts then return
        // true that's it's time to process.  Need to make sure mod counts isn't zero or
        // that's undefined behavior.  Return true in that case since it means you're trying
        // to process faster than the task is running.  
        uint32_t mod_counts = SYSTICK_FREQUENCY / rate_hz / taskInfo.delay;

        if (mod_counts == 0) { return true; }

        return 0 == taskInfo.runCounts % mod_counts;

    } // ProcessingHz()

    // Same as above but if need to wait in the seconds range.
    bool ProcessingSeconds
        (
            uint32_t rate_seconds // Rate to process in seconds. If zero always returns true.
        )
    {
        if (rate_seconds == 0) { return true; }
        
        return (0 == taskInfo.runCounts % (SYSTICK_FREQUENCY * rate_seconds / taskInfo.delay));

    } // ProcessingSeconds()

  private:         // accessible in glo_os and in the GloOSTaskBase member functions
                   // not in the task
    
    taskInfoType taskInfo;
};

// Signiture of pointer to ISR callback function.
typedef void (*isrCallbackType)(void);

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

// Information needed to register a task with the OS.
typedef struct
{
    GloOSTaskBase * taskObjectPtr; // Pointer to task instance.
    bool            enabled;       // If set to false task won't register.
    uint8_t         priority;      // Priority from 0 (low) - 15 (high)
    taskTypeEnum    taskType;      // Defines how task will be called / used
    float           frequency;     // For periodic tasks this the frequency to call task.
    IRQn_Type       IRQnum;        // Number of unused IRQ to map task to.
    bool            isIsr;         // If the task can be triggered from hardware than
                                   // this should be set to true.
} taskRegistrationType;

// Information needed to register an ISR with the OS.
typedef struct
{
    IRQn_Type       IRQnum;     // Interrupt request number of ISR.
    uint8_t         priority;   // Priority from 0 (low) - 15 (high)
    isrCallbackType callback;   // Pointer to function that should get called when ISR occurs.
} isrRegistrationType;


/******************************************************************************
* Class: GloOS
*
* Description:  The OS
******************************************************************************/
class GloOS
{
  public: // methods
    explicit GloOS(uint32_t sysTimerFrequency, uint8_t sysTickPriority);
    bool registerTask(taskRegistrationType const & config);

    bool registerSysTickTask(GloOSTaskBase * taskObjectPtr);

    bool registerISR(isrRegistrationType const & config);

    void startOS(void);

    void dispatcher(IRQn_Type IRQnum);     // called only from stm32f4xx_iterrupts.cpp

    uint64_t get_ticks(void) const
    {
        uint64_t curr_ticks;
        uint32_t disabled = __get_PRIMASK();
        __disable_irq();
        curr_ticks = this->ticks;
        if (!disabled)
            __enable_irq();
        return curr_ticks;
    }

    double get_time(void) const
    {
        double curr_time;
        uint32_t disabled = __get_PRIMASK();
        __disable_irq();
        curr_time = (double)ticks/(double)freq;
        if (!disabled)
            __enable_irq();
        return curr_time;
    }

    uint32_t get_frequency(void) const
    {
        return freq;
    }

    bool mutexTake(GloOSTaskBase *requester, GloOSMutex *mutex);
    void mutexGive(GloOSTaskBase *requester, GloOSMutex *mutex);
    uint32_t disableInterrupts( void );
    void     restoreInterrupts( uint32_t context );

  private: // methods
    void sysTimerSetup(uint32_t SysTickFrequency, uint8_t priority);
    void taskScheduler(void);                      // called by dispatcher for systick
    GloOSTaskBase *taskPtrs[GLOOSMAXNUMTASKS];     // pointers to task Objects
    GloOSTaskBase *sysTickTaskPtr;                 // pointer to the sysTick task

  private: // fields
    uint8_t irq2taskMap[NUM_IRQN];            // task numbers associated with IRQn's
    void  (* irqFuncPtrs[NUM_IRQN])(void);    // pointers to all irqs that not tasks
    uint8_t irqPriorities[NUM_IRQN];          // Priorities of irqs used for isrs
    uint8_t  numTasks;
    uint64_t ticks;
    uint32_t freq;
    uint8_t sysTickPriority;
    bool     os_started;
};

#endif   // GLO_OS_H

// Below is a list of possible IRQnum's that might be used for tasks.
// Any IRQ not in use by any of the external IRQs not being used by an ISR
// can be used.  The gloOS registration process will check to make sure they
// are not being reused. gloOS should set up the NVIC for ALL interrupts.
// The interrupt should not be enabled on the peripherals unless the task is an
// ISR task.  See stm32f4xx.h "typedef enum IRQn" for list. There are 90 possible
// IRQns on current list of stm32f4xx processors.
//    EXTI9_5_IRQn,
//    EXTI15_10_IRQn,
//    TIM8_BRK_TIM12_IRQn,
//    TIM8_UP_TIM13_IRQn,
//    TIM8_TRG_COM_TIM14_IRQn,
//    TIM1_BRK_TIM9_IRQn,
//    TIM1_UP_TIM10_IRQn,
//    TIM1_TRG_COM_TIM11_IRQn,
//    OTG_HS_EP1_OUT_IRQn,
//    OTG_HS_EP1_IN_IRQn,
//    OTG_HS_WKUP_IRQn,
//    OTG_HS_IRQn
