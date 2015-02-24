/****************************************************************************************
* File: config_glo_os.cpp
*
* Description: Configures the global operating system (GloOS) with all needed tasks and
*              registers all Interrupt Service Routines.
*
* Created:
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os.h" 
#include "complementary_filter_task.h" 
#include "glo_os_references.h"
#include "main_control_task.h" 
#include "communications_task.h" 
#include "leds_task.h" 
#include "telemetry_send_task.h" 
#include "usart.h"
#include "hf_control_task.h"

extern void register_glo_object_callbacks(void);
extern void DMA2_Stream0_ISR(void);


/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

#define SYSTICKPRIORITY 8      // priorities are 0-15, 15=highest priority

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                GLOBAL SYSTEM OBJECTS
*--------------------------------------------------------------------------------------*/

GloOS glo_os_object(SYSTICK_FREQUENCY, SYSTICKPRIORITY);

MainControlTask            main_control_task_object;
ComplementaryFilterTask    complementary_filter_task_object;
CommunicationsTask         communications_task_object;
LedsTask                   leds_task_object;
TelemetrySendTask          telemetry_send_task_object;
HFControlTask              hf_control_task_object;

/*---------------------------------------------------------------------------------------
*                                 OBJECT REFERENCES
*--------------------------------------------------------------------------------------*/

GloOS * glo_os = &glo_os_object;

MainControlTask          * main_control_task         = &main_control_task_object;
ComplementaryFilterTask  * complementary_filter_task = &complementary_filter_task_object;
CommunicationsTask       * communications_task       = &communications_task_object;
LedsTask                 * leds_task                 = &leds_task_object;
TelemetrySendTask        * send_task                 = &telemetry_send_task_object;
HFControlTask            * hf_control_task           = &hf_control_task_object;

/*---------------------------------------------------------------------------------------
                                    MEMORY CONSTANTS
---------------------------------------------------------------------------------------*/

// Defines set of tasks that will get registered with GloOS if enabled column is set to true.
// If set to false the register call will return success but the task won't be registered.
//
// NOTE: Right now it's a requirement that the priority of a task must be below the
// priority 'SYSTICKPRIORITY' or the registration will fail.
//
// ALSO NOTE: Be aware of 'clipping' of the frequency due to the OS working in ticks.
// Example if SYSTICK_FREQUENCY is 1000 and a task is set to have a frequency of 400hz
// then number of ticks would be 2.5 which is not an integer.  So in reality that task
// would try to run at 500hz.
const taskRegistrationType task_registration_info[] =
{
    //  Pointer to task         Enabled    Priority      Type        Frequency       IRQ number            Triggered by ISR
    { main_control_task,          true,       6,        periodic,       1000,        EXTI9_5_IRQn,              false },
    { complementary_filter_task,  true,       5,        periodic,        500,        TIM1_UP_TIM10_IRQn,        false },
    { communications_task,        true,       3,        periodic,        500,        I2C1_EV_IRQn,              false },
    { leds_task,                  true,       2,        periodic,        2,          I2C1_ER_IRQn,              false },
    { send_task,                  true,       4,        queued,          1,          EXTI15_10_IRQn,            false },
    { hf_control_task,            true,       7,        periodic,       5000,        TIM1_BRK_TIM9_IRQn,        false },
};

const uint32_t elements_in_task_table = sizeof(task_registration_info) / sizeof(task_registration_info[0]);

// Defines set of Interrupt Service Routines that will get registered with GloOS.  This will
// essentially re-route the normal IRQHandler to whatever callback function pointer is specified.
// It is important to register all ISRs in case there are conflicts with tasks and so a
// priority can be defined relative to the tasks.
// NOTE: These must have higher priorities than tasks if you expect them to interrupt tasks.

const isrRegistrationType isr_registration_info[] = 
{
    //  IRQ number            Priority           Callback
    { DMA2_Stream0_IRQn,        14,          &DMA2_Stream0_ISR       },
//    { TIM3_IRQn,                13,          &TIM3_ISR               },
//    { TIM4_IRQn,                13,          &TIM4_ISR               },
    { DMA1_Stream6_IRQn,        15,          &Usart::USART2_TX_ISR   },
};

const uint32_t elements_in_isr_table = sizeof(isr_registration_info) / sizeof(isr_registration_info[0]);

// uncomment these if no ISRs
//const isrRegistrationType isr_registration_info[1] = {TIM3_IRQn,0,0}; 
//const uint32_t elements_in_isr_table = 0;

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

// Forward declarations
void register_tasks(void);
void register_isrs(void);

/*****************************************************************************
* Function: configure_glo_os
*
* Description: Configures the global operating system (GloOS) with all needed tasks
*              and registers all Interrupt Service Routines.
*****************************************************************************/
void configure_glo_os(void)
{
    register_glo_object_callbacks();
    
    register_isrs();

    register_tasks();
    
    glo_os->startOS();
}

/*****************************************************************************
* Function: register_tasks
*
* Description: Registers all tasks with GloOS.
*****************************************************************************/
void register_tasks(void)
{
    for (uint32_t i = 0; i < elements_in_task_table; ++i)
    {
        if (!glo_os->registerTask(task_registration_info[i]))
        {
            while(true);       // capture the error by staying here
        }
    }

} 


/*****************************************************************************
* Function: register_isrs
*
* Description: Registers all Interrupt Service Routines.
*****************************************************************************/
void register_isrs(void)
{
    for (uint32_t i = 0; i < elements_in_isr_table; ++i)
    {
        if (!glo_os->registerISR(isr_registration_info[i]))
        {
            while(true);       // capture the error by staying here
        }
    }

} 