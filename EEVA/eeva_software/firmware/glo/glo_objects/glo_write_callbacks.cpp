/****************************************************************************************
* File: glo_write_callbacks.cpp
*
* Description: Provides functions to be called whenever the glo object is published.
*
* Created:
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "glo_objects.h"
#include "glo_write_callbacks.h"

/*---------------------------------------------------------------------------------------
*                                       extern global objects
*--------------------------------------------------------------------------------------*/

extern volatile bool sending_data;

extern float ACCEL_SCALES[3];
extern float ACCEL_OFFSETS[3];
extern float ENCODER_SCALES[2];
extern float MOTOR_SIGNS[2];
extern float CURRENT_SCALES[2];
extern float CURRENT_OFFSETS[2];
extern float BATTERY_SCALE;
extern float BATTERY_OFFSET;
extern float WHEEL_BASE;

// extern PidController roll_pid;

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Description:
*
* Note: This function should be called with interrupts disabled so higher priority
*       tasks can't interrupt and also so data can't change.
*****************************************************************************/
bool driving_command_callback
    (
        glo_driving_command_t & data  // New data.
    )
{
    glo_motion_commands_t motion_commands;
    glo_motion_commands.read(&motion_commands);
    
    switch (data.movement_type)
    {
      case DRIVING_COMMAND_LEFT:
        motion_commands.yaw -= 2.5f * 3.14159f / 180.0f;
        break;
      case DRIVING_COMMAND_RIGHT:
        motion_commands.yaw += 2.5f * 3.14159f / 180.0f;
        break;
      case DRIVING_COMMAND_FORWARD:
        motion_commands.speed += 0.01f;
        if (motion_commands.speed > 1.0f) { motion_commands.speed = 1.0f; }
        break;
      case DRIVING_COMMAND_REVERSE:
        motion_commands.speed -= 0.01f;
        if (motion_commands.speed < -1.0f) { motion_commands.speed = -1.0f; }
        break;
      case DRIVING_COMMAND_STOP:
        motion_commands.speed = 0.0f;
        break;
    }
                  
    // Save the appropriate glo objects.  This should be fine as long as it 
    // doesn't have a callback which publishes to this glo object.
    glo_motion_commands.publish(&motion_commands);
    
    return true; // data should be published
}

/*****************************************************************************
* Function:
*
* Description:
*
* Note: This function should be called with interrupts disabled so higher priority
*       tasks can't interrupt and also so data can't change.
*****************************************************************************/
bool flash_params_callback
    (
        glo_flash_params_t & data  // New data.
    )
{
  ACCEL_SCALES[0]=data.accel_scales[0];
  ACCEL_SCALES[1]=data.accel_scales[1];
  ACCEL_SCALES[2]=data.accel_scales[2];
  ACCEL_OFFSETS[0]=data.accel_offsets[0];
  ACCEL_OFFSETS[1]=data.accel_offsets[1];
  ACCEL_OFFSETS[2]=data.accel_offsets[2];

  ENCODER_SCALES[0]=data.encoder_scales[0];
  ENCODER_SCALES[1]=data.encoder_scales[1];

  MOTOR_SIGNS[0]=data.motor_signs[0];
  MOTOR_SIGNS[1]=data.motor_signs[1];

  CURRENT_SCALES[0]=data.current_scales[0];
  CURRENT_SCALES[1]=data.current_scales[1];
  CURRENT_OFFSETS[0]=data.current_offsets[0];
  CURRENT_OFFSETS[1]=data.current_offsets[1];

  BATTERY_SCALE=data.battery_scale;
  BATTERY_OFFSET=data.battery_offset;

  WHEEL_BASE=data.wheel_base;
  
  return true; // data should be published
  
}

/*****************************************************************************
* Function: roll_pid_params_callback
*
* Description: Syncs PID instance to new data
*
* Note: This function should be called with interrupts disabled so higher priority
*       tasks can't interrupt and also so data can't change.
*****************************************************************************/
/*
bool roll_pid_params_callback
    (
        glo_pid_params_t & data  // New data.
    )
{
    roll_pid.set_Kp(data.Kp);
    roll_pid.set_Ki(data.Ki);
    roll_pid.set_Kd(data.Kd);
    roll_pid.set_integral_lolimit(data.integral_lolimit);
    roll_pid.set_integral_hilimit(data.integral_hilimit);
    roll_pid.set_lolimit(data.lolimit);
    roll_pid.set_hilimit(data.hilimit);
	
	return true; // data should be published

}
*/
