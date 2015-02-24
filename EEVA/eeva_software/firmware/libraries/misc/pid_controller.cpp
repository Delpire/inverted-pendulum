//-------------------------------------------------------------------------
//  Implementation of PidController
//-------------------------------------------------------------------------
#include "pid_controller.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
PidController::PidController(float Kp,
                             float Ki,
                             float Kd,
                             float integral_lolimit,
                             float integral_hilimit,
                             float lolimit,
                             float hilimit)
{
  // initialize parameters
  this->Kp               = Kp;
  this->Ki               = Ki;
  this->Kd               = Kd;
  this->integral_lolimit = integral_lolimit;
  this->integral_hilimit = integral_hilimit;
  this->lolimit          = lolimit;
  this->hilimit          = hilimit;
  
  //initialize integral
  this->integral         = 0.0f;
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
PidController::~PidController()
{
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
float PidController::calculate(float error, float derivative, float DT)
{
  float output;

  integral += Ki*error*DT;     // integral part
  
  // anti-integral-windup limits
  if (integral < integral_lolimit) integral = integral_lolimit;
  if (integral > integral_hilimit) integral = integral_hilimit;
    
  //PID
  output = Kp*error + integral + Kd*derivative;
  
  // output limits
  if (output < lolimit) output = lolimit;
  if (output > hilimit) output = hilimit;
  
  return(output);
}