/****************************************************************************************
* File: complementary_filter_task.h
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef COMPLEMENTARY_FILTER_TASK_INCLUDED_H
#define COMPLEMENTARY_FILTER_TASK_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os_sys.h"
#include "mpu6000.h"

extern void complementary_filter_task_callback(void);
extern MPU6000 mpu;

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/

/******************************************************************************
* Class: ComplementaryFilterTask
*
* Description:  Simply calls call back in main.cpp
******************************************************************************/
class ComplementaryFilterTask : public GloOSTaskBase
{
public: // methods

    // called before running task for first time.
    void initialize (void)
    {
        while (mpu.initialize() != 0)
        {
            // Sensor failed to initialize properly.  Wait then try again.
            // The stuff below is dangerous because SysTickCounts is 64 bit -- not atomic
            double start = glo_os->get_time();
            while((glo_os->get_time() - start) < 0.5);  // wait 0.5 sec
        }
    };

    // the task function
    void taskRun(void) {complementary_filter_task_callback();};

private: // methods

private: // fields

};

#endif // COMPLEMENTARY_FILTER_TASK_INCLUDED_H
