/****************************************************************************************
* File: glo_os_references.h
*
* Description: Provides access to glo_os objects.  
*              If you need to access a task you must also include its header file.
*              Otherwise you will get an 'incomplete type not allowed' error.
*
* Created:
****************************************************************************************/

#ifndef GLO_OS_REFERENCES_INCLUDED_H
#define GLO_OS_REFERENCES_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                  OBJECT REFERENCES
*--------------------------------------------------------------------------------------*/

// Forward declarations
class GloOS;
class MainControlTask;
class ComplementaryFilterTask;
class CommunicationsTask;
class LedsTask;
class TelemetrySendTask;
class HFControlTask;

extern GloOS                   * glo_os;
extern MainControlTask         * main_control_task;
extern ComplementaryFilterTask * complementary_filter_task;
extern CommunicationsTask      * communications_task;
extern LedsTask                * leds_task;
extern TelemetrySendTask       * send_task;
extern HFControlTask           * hf_control_task;

extern void configure_glo_os(void);

#endif // GLO_OS_REFERENCES_INCLUDED_H
