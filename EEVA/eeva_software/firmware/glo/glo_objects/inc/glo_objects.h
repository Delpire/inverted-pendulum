/****************************************************************************************
* File: glo_objects.h
*
* Description: Header file for glo_objects.cpp.
*              This is the only header file you should include to use glo objects.
*              To define new objects you must modify this file and glo_types.h
*
*              If a callback is needed -- then write it, and modify write_callbacks.h and
*              glo_objects.cpp.
*
* Created:
****************************************************************************************/

#ifndef GLO_OBJECTS_INCLUDED_H
#define GLO_OBJECTS_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <stdint.h>

#include "glo_object_template.h"
#include "glo_types.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

// Unique IDs given to glo objects are defined in this enumeration.
// Add an ID name to the enum for each new object.
// The bottom member of the enum should always be NUM_GLO_OBJECTS which
// keeps track of the total number of objects (max number = 255)
typedef uint8_t glo_object_id_t;
enum
{
    GLO_ID_MOTION_COMMANDS,
    GLO_ID_RAW_ACCELS,
    GLO_ID_RAW_ANALOG,
    GLO_ID_GYROS_ACCELS,
    GLO_ID_ROLL_PITCH_YAW,
    GLO_ID_QUATERNION,
    GLO_ID_THETA_ZERO,
    GLO_ID_ODOMETRY,
    GLO_ID_ROLL_PID_PARAMS,
    GLO_ID_PITCH_PID_PARAMS,
    GLO_ID_MODES,
    GLO_ID_FLASH_PARAMS,
	GLO_ID_ASSERT_MESSAGE,
    GLO_ID_DEBUG_MESSAGE,
    GLO_ID_CAPTURE_DATA,
    GLO_ID_DRIVING_COMMAND,
    GLO_ID_CAPTURE_COMMAND,
    GLO_ID_STATUS_DATA,
	
    NUM_GLO_OBJECTS,
};

/*---------------------------------------------------------------------------------------
*                                        MACROS
*--------------------------------------------------------------------------------------*/

// Macro used to allow glo_objects.cpp to define the objects and avoid duplicate maintenance.
// In glo_objects.cpp DEFINE_GLO_OBJECTS forces the macro to define the objects.
// Elswhere it only declares the objects.
#ifndef DEFINE_GLO_OBJECTS
#define GLO_OBJECT(var_name, struct_type, id, num_instances) \
    extern GloObjectTemplate<struct_type, num_instances> var_name
#else
#define GLO_OBJECT(var_name, struct_type, id, num_instances) \
    GloObjectTemplate<struct_type, num_instances> var_name(id)
#endif

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

// Macro use:
// Argument 1: Glo object variable name
// Argument 2: Name of struct (defined in glo_types.h)
// Argument 3: ID (must be unique, see enumeration above in CONSTANTS section above)
// Argument 4: How many instances of struct to hold (usually just one)
GLO_OBJECT(glo_motion_commands,      glo_motion_commands_t,     GLO_ID_MOTION_COMMANDS,      1);
GLO_OBJECT(glo_raw_accels,           glo_raw_accels_t,          GLO_ID_RAW_ACCELS,           1);
GLO_OBJECT(glo_raw_analog,           glo_raw_analog_t,          GLO_ID_RAW_ANALOG,           1);
GLO_OBJECT(glo_gyros_accels,         glo_gyros_accels_t,        GLO_ID_GYROS_ACCELS,         1);
GLO_OBJECT(glo_roll_pitch_yaw,       glo_roll_pitch_yaw_t,      GLO_ID_ROLL_PITCH_YAW,       1);
GLO_OBJECT(glo_quaternion,           glo_quaternion_t,          GLO_ID_QUATERNION,           1);
GLO_OBJECT(glo_theta_zero,           glo_theta_zero_t,          GLO_ID_THETA_ZERO,           1);
GLO_OBJECT(glo_odometry,             glo_odometry_t,            GLO_ID_ODOMETRY,             1);
GLO_OBJECT(glo_roll_pid_params,      glo_pid_params_t,          GLO_ID_ROLL_PID_PARAMS,      1);
GLO_OBJECT(glo_pitch_pid_params,     glo_pid_params_t,          GLO_ID_PITCH_PID_PARAMS,     1);
GLO_OBJECT(glo_modes,                glo_modes_t,               GLO_ID_MODES,                1);
GLO_OBJECT(glo_flash_params,         glo_flash_params_t,        GLO_ID_FLASH_PARAMS,         1);
GLO_OBJECT(glo_assert_message,       glo_assert_message_t,      GLO_ID_ASSERT_MESSAGE,       1);
GLO_OBJECT(glo_debug_message,        glo_debug_message_t,       GLO_ID_DEBUG_MESSAGE,        1);
GLO_OBJECT(glo_capture_data,         glo_capture_data_t,        GLO_ID_CAPTURE_DATA,         2001); // 64K available. Using around half for this glo object (assuming 16 byte data).  Add 1 since instance 0 isn't used.
GLO_OBJECT(glo_driving_command,      glo_driving_command_t,     GLO_ID_DRIVING_COMMAND,      1);
GLO_OBJECT(glo_capture_command,      glo_capture_command_t,     GLO_ID_CAPTURE_COMMAND,      1);
GLO_OBJECT(glo_status_data,          glo_status_data_t,         GLO_ID_STATUS_DATA,          1);


// Array of glo objects meta data.
extern GloObjectBase * gloObjectPtrs[NUM_GLO_OBJECTS];

#endif // GLO_OBJECTS_INCLUDED_H
