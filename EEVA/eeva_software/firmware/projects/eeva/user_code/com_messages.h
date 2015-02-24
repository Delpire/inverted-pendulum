/****************************************************************************************
* File: com_messages.h
*
* Description: defines some things used in communications with terminal over serial port
*
* Created:
****************************************************************************************/

#ifndef COM_MESSAGES_INCLUDED_H
#define COM_MESSAGES_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <string>

/*---------------------------------------------------------------------------------------
*                                      MEMORY CONSTANTS
*--------------------------------------------------------------------------------------*/

const string cal_messages[] = {"\r Place flat and hit c. \r>>",
                               "\r Place nose up and hit c. \r>>",
                               "\r Place nose down and hit c. \r>>",
                               "\r Place on left side and hit c. \r>>",
                               "\r Place on right side and hit c. \r>>",
                               "\r Place on back and hit c. \r>>",
                               "\r\r Calibration successful. \r",
                               "\r\r Unsuccessful calibration !!! \r"};

const string servo_messages[] = {"\r Pitch gimbal up 30 deg and hit c. \r>>",
                                "\r Pitch gimbal down 30 deg and hit c. \r>>",
                                "\r Roll gimbal right side down 30 deg and hit c. \r>>",
                                "\r Roll gimbal right side up 30 deg and hit c. \r>>",
                                "\r Is gimbal pitching up?\r y=yes, other=no. \r>>",
                                "\r Is gimbal rolling right side down?\r y=yes, other=no. \r>>",
                                "\r\r Calibration successful. \r"};

const string menu_message = "\rMENU:\r  a = calibrate accels\r  s = calibrate servos\r  w = write params to flash\r  e = erase params in flash\r>>";
const string no_params_message = "\rNO VALID PARAMS!!!\rShould calibrate accels and servos.\rAnd write params.\r";
const string params_written_message = "\r\r Params written.\r";
const string params_erased_message = "\r\r Params erased.\r";

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/
typedef enum
{
  waiting_for_connection,
  waiting_for_menu_command,
  calibrating,
} coms_state_t;

typedef enum
{
  pitch_up,
  pitch_down,
  roll_right,
  roll_left,
  driving_pitch,
  driving_roll,
  calculating,
} cal_servos_state_t;

typedef enum
{
  looking_for_c1,
  looking_for_o,
  looking_for_n1,
  looking_for_n2,
  looking_for_e,
  looking_for_c2,
  looking_for_t,
} connect_state_t;

#endif  // COM_MESSAGES_INCLUDED_H
