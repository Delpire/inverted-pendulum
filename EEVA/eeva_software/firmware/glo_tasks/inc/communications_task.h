/****************************************************************************************
* File: communications_task.h
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef COMMUNICATIONS_TASK_INCLUDED_H
#define COMMUNICATIONS_TASK_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os_sys.h"
#include "glo_rx_link.h"
#include "digital_out.h"
#include "telemetry_send_task.h"
#include "util_assert.h"
#include "stm32f4xx.h"

//#include "flash_storage.h"
//extern FlashStorage flash_storage;

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
* Class: CommunicationsTask
*
* Description:  Simply calls call back in main.cpp
******************************************************************************/
class CommunicationsTask : public GloOSTaskBase
{
public: // methods

    CommunicationsTask(void) :
        glo_rx_link(NULL),
        bus(USART_BUS_2),
        serial_port(NULL)
    {
    }
    
    // called before running task for first time.
    void initialize(void)
    {
//        glo_flash_params_t flash_params = {{{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}},
//                                          {{58.0f,58.0f},{-100.0f,-100.0f},{-1.0f,1.0f}}};
//        glo_modes_t modes;
//        
//        if (flash_storage.IsInitialized())
//        {
//          flash_storage.Read(&flash_params);
//          glo_flash_params.publish(&flash_params);
//          modes.params_set = true;
//        }
//        else
//        {
//          modes.params_set = false;
//        }
//        modes.op_state = normal;
//        modes.drive_roll_pwm = false;
//        modes.drive_pitch_pwm = false;
//      
//        glo_modes.publish(&modes);
        
        // Setup receive link. If usart is already initialize will return same instance.
        // Make sure interrupts are disbled for initialization because the
        // first dma_tx error occurs before the fields the dma_tx are populated
        // although current config of glo_os shouldn't have the ISRs enabled yet
        uint32_t context = glo_os->disableInterrupts();
        serial_port = Usart::Instance(bus);
        glo_os->restoreInterrupts(context);
        
        assert(serial_port != NULL, ASSERT_STOP);     
        
        glo_rx_link = new GloRxLink(serial_port);
        
        assert(glo_rx_link != NULL, ASSERT_STOP);
    };
    
    void taskRun(void) 
    {
        float delta_t = get_delay_time();
    
        // Set output pin to toggle during function execution for profiling.
        static DigitalOut watch_bit(PB8);
        watch_bit.set();
        
        // Parse any received data to try to form a complete message.
        // If a message is received then it is published.
        glo_rx_link->parse();
        
        // Integrate speed command and publish distance command.
        // Actual driving commands (ie turn left) are handled in glo callback.
        glo_motion_commands_t motion_commands;
        glo_motion_commands.read(&motion_commands);
        motion_commands.distance += motion_commands.speed * delta_t;
        glo_motion_commands.publish(&motion_commands);
            
        // Read current tilt angle to use for transmitting data.
        glo_roll_pitch_yaw_t roll_pitch_yaw;
        glo_roll_pitch_yaw.read(&roll_pitch_yaw);
        float tilt = roll_pitch_yaw.rpy[1];
        
        // Periodically send back status data to display to user.
        if (ProcessingHz(4))
        {
            glo_status_data_t status_data;
            status_data.tilt_angle = tilt;
            
            glo_status_data.publish(&status_data);
            send_task->send(glo_status_data.get_id());
        }

        watch_bit.clear();
    };

private: // methods

private: // fields
   
    // Receive link for parsing incoming glo object messages.
    GloRxLink * glo_rx_link;
    
    // Serial bus wrapped by glo link.
    usart_bus_t bus;
    Usart * serial_port;    

};


#endif // COMMUNICATIONS_TASK_INCLUDED_H
