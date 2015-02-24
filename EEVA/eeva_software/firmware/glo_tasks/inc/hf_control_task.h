/****************************************************************************************
* File: hf_control_task.h
*
* Description: 
*
* Created: 
****************************************************************************************/

#ifndef HF_CONTROL_TASK_INCLUDED_H
#define HF_CONTROL_TASK_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_os_sys.h"
#include "digital_out.h"
#include "telemetry_send_task.h"
#include "util_assert.h"
#include "stm32f4xx.h"
#include "analog_in.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

#define AMP_GAIN    ( 90.0f ) // V / V
#define R_SENSE     ( 0.015f ) // Sense Resistor Value (Ohms)
#define VDD_OFFSET  ( 3.3f / 2.0f ) // Account for zero current offset.

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/

extern AnalogIn analog;

/******************************************************************************
* Class: HFControlTask
*
* Description:
******************************************************************************/
class HFControlTask : public GloOSTaskBase
{
public: // methods

    HFControlTask(void) :
        capturing_data(false),
        capture_counter(0)
    {
    }
    
    // called before running task for first time.
    void initialize(void)
    {
        // subtract one since instance 0 isn't used.
        max_samples = gloObjectPtrs[GLO_ID_CAPTURE_DATA]->get_num_instances() - 1;
    };
    
    void taskRun(void) 
    {
        float delta_t = get_delay_time();

        // Set output pin to toggle during function execution for profiling.
        //static DigitalOut watch_bit(PB8);
        //watch_bit.set();
    
        glo_raw_analog_t raw_analog;
        analog.getVoltages(&raw_analog.voltages[0]);
        
        // Raw values are in volts.
        float motor_a_raw = raw_analog.voltages[10];
        float motor_b_raw = raw_analog.voltages[9];
        
        // Convert voltages to amps.       
        float motor_a_current = (motor_a_raw - VDD_OFFSET) / (AMP_GAIN * R_SENSE);
        float motor_b_current = (motor_b_raw - VDD_OFFSET) / (AMP_GAIN * R_SENSE); 
        
        // If most recent capture command was a start command then need to send data.
        glo_capture_command_t capture_command;
        glo_capture_command.read(&capture_command);
        bool sending_capture_data = (capture_command.is_start != 0);
        
        // Reset counter if we just started taking data.
        if (!capturing_data && sending_capture_data)
        {
            capturing_data = true;
            capture_counter = 0;
        }
        
        // Check if we need to stop sending data because our buffer is full or user wants to stop.
        bool buffer_full = (capture_counter >= max_samples);
        bool have_desired_samples = (capture_counter >= capture_command.desired_samples);
        if (sending_capture_data && (buffer_full || have_desired_samples))
        {
            sending_capture_data = false;
            
            // Update glo object so next time we don't try to keep capturing data.
            capture_command.is_start = 0;
            glo_capture_command.publish(&capture_command);
        }
        
        if (!sending_capture_data && capturing_data && (capture_counter > 0))
        {
            // Just stopped taking data so first send send back all captured data instances.
            // then send back packet telling UI how many samples it should've gotten.
            send_task->send(glo_capture_data.get_id(), capture_counter, true); 
            capture_command.total_samples = capture_counter;
            glo_capture_command.publish(&capture_command);
            send_task->send(glo_capture_command.get_id());
        }
       
        capturing_data = sending_capture_data;
        
        if (sending_capture_data)
        {
            if (ProcessingHz(capture_command.frequency))
            {
                // Read current tilt angle to use for transmitting data.
                glo_roll_pitch_yaw_t roll_pitch_yaw;
                glo_roll_pitch_yaw.read(&roll_pitch_yaw);
                float tilt = roll_pitch_yaw.rpy[1];
                
                glo_capture_data_t capture_data;
                capture_data.time = (1.0f / capture_command.frequency) * capture_counter;
                capture_data.tilt_angle = tilt;
                capture_data.motor_a_current = motor_a_current;
                capture_data.motor_b_current = motor_b_current;
                
                // Use capture counter as instance number to publish to.
                // Need to add one because instance numbers are indexed from 1.
                glo_capture_data.publish(&capture_data, capture_counter + 1);
                
                ++capture_counter;
            }
        }
        
        glo_raw_analog.publish(&raw_analog);

        //watch_bit.clear();
    };

private: // methods

private: // fields
    
    // Flag so we know when we first start capturing.
    bool capturing_data;
    
    // Counts up when capturing data to get time of reading.
    uint32_t capture_counter;
    
    // Maximum number of sample data to record before stopping data capture.
    uint16_t max_samples;

};


#endif // HF_CONTROL_TASK_INCLUDED_H
