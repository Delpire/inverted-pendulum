/****************************************************************************************
* File: communications_filter_task.h
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef TELEMETRY_SEND_TASK_INCLUDED_H
#define TELEMETRY_SEND_TASK_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os_sys.h"
#include "stm32f4xx.h"
#include "dma_rx.h"
#include "dma_tx.h"
#include "usart.h"
#include "util_assert.h"
#include "glo_tx_link.h"

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
* Class: TelemetrySendTask
*
* Description:  Simply calls call back in main.cpp
******************************************************************************/
class TelemetrySendTask : public GloOSTaskBase
{
public: // methods

    TelemetrySendTask(void) :
        telemetry_queue(new GloObjectQueue(25 /*elements*/)),
        glo_tx_link(NULL),
        bus(USART_BUS_2),
        serial_port(NULL)
    {
        // Can't setup in initializer list due to how protected fields are handled.
        queue = telemetry_queue;
    }
    
    // Provide interface for sending glo objects.  By hiding direct access to queue 
    // will be more flexible to change method of sending.
    // Returns true on sucess.
    bool send
        (
            uint8_t  id,               // ID number associated with glo object to send.
            uint16_t instance = 1,     // Instance number associated with glo object to send.
            bool     send_all = false  // If true will send up all instances up and including to 'instance' parameter.
        )
     {
        return putQueue(id, instance, send_all) != 0;
     }
    
    // Automically called by GloOS before running task for first time.
    void initialize(void)
    {    
        // Setup transfer link. If usart is already initialize will return same instance.
        // Make sure interrupts are disbled for initialization because the
        // first dma_tx error occurs before the fields the dma_tx are populated
        // although current config of glo_os shouldn't have the ISRs enabled yet
        uint32_t context = glo_os->disableInterrupts();
        serial_port = Usart::Instance(bus);
        glo_os->restoreInterrupts(context);
        
        assert(serial_port != NULL, ASSERT_STOP);     
        
        glo_tx_link = new GloTxLink(serial_port);
        
        assert(glo_tx_link != NULL, ASSERT_STOP);
    };

    void taskRun(void) 
    {
        uint8_t object_id;
        uint16_t instance;
        uint16_t num_instance;    
        bool send_all;
        
        while (telemetry_queue->take(&object_id, &instance, &send_all))
        {
            if (instance > 0)
            {
                if (!send_all)
                {
                     // send just single instance
                    glo_tx_link->send(object_id, instance);
                }
                else
                {
                    // send up to and including specified instance.
                    for (uint16_t i = 0; i < instance; i++)
                    {
                        glo_tx_link->send(object_id, i+1);
                    }
                }
            }
            else  // instance = 0 means send all instances
            {
                num_instance = gloObjectPtrs[object_id]->get_num_instances();
                
                for (uint16_t i = 0; i < num_instance; i++)
                {
                    glo_tx_link->send(object_id, i+1);
                }
            }
        }
    };

private: // methods

private: // fields

    // Queue for sending glo object data over glo link.
    GloObjectQueue * telemetry_queue;

    // Transfer link for sending glo object messages.
    GloTxLink * glo_tx_link;
    
    // Serial bus wrapped by glo link.
    usart_bus_t bus;
    Usart * serial_port;    

};

#endif // TELEMETRY_SEND_TASK_INCLUDED_H
