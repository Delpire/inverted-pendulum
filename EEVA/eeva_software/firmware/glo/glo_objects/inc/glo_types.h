/****************************************************************************************
* File: glo_types.h
*
* Description: Defines types of all glo objects. Instances are defined in glo_objects.cpp
*              To use glo objects include glo_objects.h.
*              To define new objects you must modify this file and glo_objects.h
*
*              If a callback is needed -- then write it, and modify write_callbacks.h and
*              glo_objects.cpp.
*
* Created:
****************************************************************************************/

#ifndef GLO_TYPES_INCLUDED_H
#define GLO_TYPES_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "telemetry_text_size.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/


/****************************************************************************************/
// commands to the guidance.  updated by communications task and/or the idle task.
typedef struct 
{
     float distance;       // [m] linear distance
     float speed;          // [m/s] linear speed
     float yaw;            // [rad] yaw angle
     float omega;          // [rad/s] angular rate
} glo_motion_commands_t;

/****************************************************************************************/
// uncalibrated accel data.  Updated by complementary filter task.
typedef struct 
{
     float accels[3];       // [m/sec/sec]
} glo_raw_accels_t;

/****************************************************************************************/
// raw voltages from adc.  Updated by main control task.
typedef struct 
{
    float voltages[11];
} glo_raw_analog_t;

/****************************************************************************************/
// gyro and accel data.  Updated by complementary filter task.
typedef struct 
{
     float gyros[3];        // [rad/sec]
     float accels[3];       // [m/sec/sec]
} glo_gyros_accels_t;

/****************************************************************************************/
// roll pitch yaw estimates.  Updated by complementary filter task.
typedef struct 
{
     float rpy[3];           // [radians]
} glo_roll_pitch_yaw_t;

/****************************************************************************************/
// quaternion representation of orientation.  Updated by complementary filter task.
typedef struct 
{
     float q[4];                // [dimensionless]
} glo_quaternion_t;

/****************************************************************************************/
// zero angle when balanced.  Updated by main_control task. Zerod in idle task
typedef struct 
{
    float theta;       // [rad]
} glo_theta_zero_t;

/****************************************************************************************/
// things calculated from odometry.  Updated by main control task.
typedef struct 
{
    float left_distance;    // [m] distance left wheel has traveled   
    float right_distance;   // [m] distance right wheel has traveled
    float avg_distance;     // [m] average of left and right wheel distances
    float yaw;              // [rad] heading angle as found from odometry
    float left_speed;       // [m/s] left wheel speed
    float right_speed;      // [m/s] right whell speed
    float avg_speed;        // [m/s] average of left and right wheel speeds
} glo_odometry_t;

/****************************************************************************************/
// pid type used for several objects.  TODO: Updated by serial coms?
typedef struct 
{
    float Kp;
    float Ki;
    float Kd;
    float integral_lolimit;
    float integral_hilimit;
    float lolimit;
    float hilimit;
} glo_pid_params_t;

/****************************************************************************************/
// operation state of the gimbal. NOT ACTUALLY A GLO_OBJECT. Used in operation_modes.
typedef enum 
{
    normal,
    waiting_for_push_button,
    getting_theta_zero,
    debugging,
} glo_op_state_t;

/****************************************************************************************/
// sets operation modes. Initialized and updated by communications_task
typedef struct 
{
    glo_op_state_t op_state;
    bool params_set;
} glo_modes_t;

/****************************************************************************************/
// set of parameters that can be stored in flash.
// Initialized and updated by communications_task.
// USE ONLY 32 BIT DATA TYPES for simplicity and to remove worries about data alignment.
// A write callback should make sure all things get populated with correct params.
typedef struct
{
  float accel_scales[3];    // [m/s/s / m/s/s]  calibration scales for accelerometers
  float accel_offsets[3];   // [m/s/s]  calibration offsets for accelerometers
  float encoder_scales[2];  // [m/count] scaling for encoder counts to distance
  float motor_signs[2];     // -1 or 1 for direction of motors due to PWM
  float current_scales[2];  // [Amp/Volt] scaling for current sensors
  float current_offsets[2]; // [Amp] offset for current sensors
  float battery_scale;      // [V/V] scaling for battery sensing
  float battery_offset;     // [V] offset for battery sensing
  float wheel_base;         // [m]
} glo_flash_params_t;

/****************************************************************************************/
// Outgoing telemetry assert message for user feedback. 
// Automatically sent when assert fails.
typedef struct 
{
    uint32_t action; // corresponds to enum in util_assert.h (ie continue, restart, stop, etc)
    char text[telemetry_text_size];
    
} glo_assert_message_t;

/****************************************************************************************/
// Outgoing telemetry debug log/status message.  Separate message to prevent overwriting 
// assert message in telemetry queue. Use debug_printf() to send message.
typedef struct 
{
    char text[telemetry_text_size];
    
} glo_debug_message_t;

/****************************************************************************************/
// Data that is transmitted when a capture command is received.
typedef struct 
{
    float time; // seconds 
    float tilt_angle; // radians
    float motor_a_current; // amps
    float motor_b_current; // amps
    
} glo_capture_data_t;


/****************************************************************************************/
// List of supported driving commands IDs. Not actually a glo object.
typedef uint32_t driving_command_id_t;
enum
{
    DRIVING_COMMAND_FORWARD,
    DRIVING_COMMAND_REVERSE,
    DRIVING_COMMAND_RIGHT,
    DRIVING_COMMAND_LEFT,
    DRIVING_COMMAND_STOP,
};

/****************************************************************************************/
// Command that is received by user for driving robot.
typedef struct 
{
    driving_command_id_t movement_type;
    
} glo_driving_command_t;

/****************************************************************************************/
// Command that is received by user for starting/stopping data logging (glo_capture_data_t)
typedef struct 
{
    uint8_t is_start; // false (0) if should stop sending data
    uint8_t pad0;
    uint16_t frequency; // Hz
    uint32_t desired_samples; // How many samples to collect before stopping.
    uint32_t total_samples; // Used to notify UI samples are done being sent and how many there should be.
    
} glo_capture_command_t;

/****************************************************************************************/
// For sending low rate periodic status data for user feedback.
typedef struct 
{
    float tilt_angle; // radians
    
} glo_status_data_t;


#endif // GLO_TYPES_INCLUDED_H
