/****************************************************************************************
* File: main.cpp
*
* Description: 
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES AND CONSTANTS
*--------------------------------------------------------------------------------------*/

#include "glo_os_sys.h"
#include "stm32f4xx.h"
#include "user_leds.h"
#include "mpu6000.h"
#include "tb6612fng.h"
#include "encoder.h"
#include "green_leds.h"
#include "analog_in.h"
#include "complementary_filter.h"
#include "coordinate_conversions.h"
#include "derivative_filter.h"
#include "pid_controller.h"
#include "user_pb.h"
#include "usart.h"
#include "debug_printf.h"
#include "digital_in.h"
#include <math.h>

// uncomment the define below to change orientation to the vertical (balance) orientation
#define VERTICAL_CONFIGURATION

/*---------------------------------------------------------------------------------------
*                                      INTERNAL FUNCTION PROTOTYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                     GLOBAL VARIABLES
*--------------------------------------------------------------------------------------*/
// variables that come from flash_params. Declared global because I am lazy
// these are really meant to be constants that are only changed at the 
// the start by reading the flash params or by some calibration

// wheel_radius * 2_pi / gearratio / counts_per_rev
#define ENCODER2DIST (0.03f*6.283185/29.86f/12.0f)   // distance [m] per count

float ACCEL_SCALES[3] = {1.0f,1.0f,1.0f};
float ACCEL_OFFSETS[3] = {0.0f,0.0f,0.0f};
float ENCODER_SCALES[2]={ ENCODER2DIST, -ENCODER2DIST};
float MOTOR_SIGNS[2]={1.0f,-1.0f};
float CURRENT_SCALES[2]={1.0f,1.0f};
float CURRENT_OFFSETS[2]={0.0f,0.0f};
float BATTERY_SCALE=4.286f;
float BATTERY_OFFSET=0.343f;
float WHEEL_BASE=0.089;

/*---------------------------------------------------------------------------------------
*                                     GLOBAL OBJECTS
*--------------------------------------------------------------------------------------*/

// let these be global for easy tuning with debugger
PidController position_pid(0.05, 0.025, 0.025, -.1, .1, -.35, 35);
PidController pitch_pid(2.0, 80.0, 0.05, -0.3, 0.3, -0.8, 0.8);      //kinda shakey 
PidController yaw_pid(0.1, 0.05, 0.05, -0.3, 0.3, -0.8, 0.8);

// Define any hardware instances here. Statically placing these in a callback function
// causes the constructors to run inside the tasks which seems to be causing issues.
static TB6612FNG hbridge;
static Encoder left_encoder(EncoderA);
static Encoder right_encoder(EncoderB);
static UserPB push_button;
static UserLeds leds;
static GreenLeds gleds;

// Hardware instances used outside of main.
MPU6000 mpu; // used in filter task
AnalogIn analog; // used in high frequency control task
//FlashStorage flash_storage;

/*****************************************************************************
* Function: 
*
* Description: Sets everything up then enters an infinite while loop that is
*              the idle task.
*****************************************************************************/
int main()
{
    DigitalIn left_motor_sign_input(PA0,GPIO_PuPd_UP);
    DigitalIn right_motor_sign_input(PC3,GPIO_PuPd_UP);

    double start_time;
    
    glo_modes_t modes;
    glo_theta_zero_t theta_zero;
    
    modes.op_state = waiting_for_push_button;
    glo_modes.publish(&modes);
    
    configure_glo_os();
    
    if ( left_motor_sign_input.read() ) MOTOR_SIGNS[0] = 1.0f;
    else MOTOR_SIGNS[0] = -1.0f;
    if ( right_motor_sign_input.read() ) MOTOR_SIGNS[1] = 1.0f;
    else MOTOR_SIGNS[1] = -1.0f;
        
    //**************************************************************************
    //************** The idle task
    //**************************************************************************
    while(1)
    {
        if ((push_button.read()) && (modes.op_state == waiting_for_push_button))
        {
            debug_printf("Zero angle reset.");
            modes.op_state = getting_theta_zero;
            glo_modes.publish(&modes);                       // publish mode then wait at
            start_time = glo_os->get_time();                 // least 0.1 seconds before
            while ((glo_os->get_time() - start_time) < 0.1); // zeroing the low pass filt
            theta_zero.theta = 0.0f;                         // to make sure the filter
            glo_theta_zero.publish(&theta_zero);             // starts over at zero
        }
        else if (modes.op_state == getting_theta_zero)
        {
            const double filter_settle_time = 3;  
            debug_printf("Waiting %d sec for filter to settle.", (int32_t)filter_settle_time);
            start_time = glo_os->get_time();                
            while ((glo_os->get_time() - start_time) < filter_settle_time); // wait for the filter to settle
            modes.op_state = normal;
            glo_modes.publish(&modes);
            debug_printf("Finished waiting.");
        }
        else if (modes.op_state == normal)
        {
        }
    }
    
}

/*****************************************************************************
* Function: 
*
* Description: A TASK FUNCTION!!!
*****************************************************************************/
void complementary_filter_task_callback(void)
{
    const float deltaT = 0.002f;                      // 500Hz update rate
    static ComplementaryFilter complementary_filter;
    static DigitalOut watch_bit(PB5);
    
    glo_raw_accels_t raw_accels;
    glo_gyros_accels_t gyros_accels;
    glo_quaternion_t quaternion;
    glo_roll_pitch_yaw_t roll_pitch_yaw;
    float raw_gyros[3];
    
    watch_bit.set();
    
    // get appropriate glo objects
    
    // get raw gyros and accels from MPU6000
    mpu.readGyro(raw_gyros);
    mpu.readAccel(raw_accels.accels);
    
    // switch the axes (dependent on configuration) and apply accel calibration
#ifdef VERTICAL_CONFIGURATION
    gyros_accels.accels[0] = -(raw_accels.accels[2]*ACCEL_SCALES[2] + ACCEL_OFFSETS[2]);
    gyros_accels.accels[1] = -(raw_accels.accels[1]*ACCEL_SCALES[1] + ACCEL_OFFSETS[1]);
    gyros_accels.accels[2] = -(raw_accels.accels[0]*ACCEL_SCALES[0] + ACCEL_OFFSETS[0]);
    gyros_accels.gyros[0] = -raw_gyros[2];
    gyros_accels.gyros[1] = -raw_gyros[1];
    gyros_accels.gyros[2] = -raw_gyros[0];
#else
    gyros_accels.accels[0] = (raw_accels.accels[0]*ACCEL_SCALES[0] + ACCEL_OFFSETS[0]);
    gyros_accels.accels[1] = -(raw_accels.accels[1]*ACCEL_SCALES[1] + ACCEL_OFFSETS[1]);
    gyros_accels.accels[2] = -(raw_accels.accels[2]*ACCEL_SCALES[2] + ACCEL_OFFSETS[2]);
    gyros_accels.gyros[0] = raw_gyros[0];
    gyros_accels.gyros[1] = -raw_gyros[1];
    gyros_accels.gyros[2] = -raw_gyros[2];
#endif
    
    // do the complmentary attitude filter
    complementary_filter.update(deltaT, gyros_accels.gyros, gyros_accels.accels);
    complementary_filter.get_state(quaternion.q);
    quaternion_2_rpy(quaternion.q, roll_pitch_yaw.rpy);
    
    // save the appropriate glo objects
    glo_raw_accels.publish(&raw_accels);
    glo_gyros_accels.publish(&gyros_accels);
    glo_quaternion.publish(&quaternion);
    glo_roll_pitch_yaw.publish(&roll_pitch_yaw);

    watch_bit.clear();
}

/*****************************************************************************
* Function: 
*
* Description: A TASK FUNCTION!!!
*****************************************************************************/
void main_control_task_callback(void)
{
    const float deltaT = 0.0002f;                      // 1000Hz update rate
                                                       // TODO THIS DELTA T DEFINITION IS
                                                       // WRONG BUT IS THE ONE USED FOR 
                                                       // ALL THE TUNING!!!!! SHOULD BE
                                                       // 0.001
    static DerivativeFilter left_deriv(deltaT, 20.0f , 0.707f);
    static DerivativeFilter right_deriv(deltaT, 20.0f , 0.707f);
    static DerivativeFilter pos_cmd_deriv(deltaT, 50.0f , 0.707f);
    static DerivativeFilter theta_cmd_deriv(deltaT, 100.0f , 0.707f);
    static DigitalOut watch_bit(PB4);
    
    glo_modes_t modes;
    glo_gyros_accels_t gyros_accels;
    glo_roll_pitch_yaw_t roll_pitch_yaw;
    glo_theta_zero_t theta_zero;
    glo_odometry_t odometry;
    glo_motion_commands_t motion_commands;
    
    float pos_cmd, yaw_cmd;
    float theta_cmd;
    float balance_duty,delta_duty;
    float left_duty,right_duty;
    
    watch_bit.set();

    // get appropriate glo objects
    glo_modes.read(&modes);
    glo_gyros_accels.read(&gyros_accels);
    glo_roll_pitch_yaw.read(&roll_pitch_yaw);
    glo_theta_zero.read(&theta_zero);
    glo_motion_commands.read(&motion_commands);
    
    //**************************************************************************
    //************** handle all the sensors read in this function
    //**************************************************************************
    
    odometry.left_distance = ENCODER_SCALES[0]*left_encoder.read();
    odometry.right_distance = ENCODER_SCALES[1]*right_encoder.read();
    odometry.avg_distance = (odometry.left_distance + odometry.right_distance)/2.0f;
    odometry.yaw = (odometry.left_distance - odometry.right_distance)/WHEEL_BASE;
    odometry.left_speed = left_deriv.calculate(odometry.left_distance);
    odometry.right_speed = right_deriv.calculate(odometry.right_distance);
    odometry.avg_speed = (odometry.left_speed + odometry.right_speed)/2.0f;
    
    //**************************************************************************
    //************** do the control calculations
    //**************************************************************************
    pos_cmd = motion_commands.distance;
    yaw_cmd = motion_commands.yaw;
    
    // position controller
    float pos_cmd_d = pos_cmd_deriv.calculate(pos_cmd);
    theta_cmd = -position_pid.calculate(pos_cmd-odometry.avg_distance,
                                        pos_cmd_d-odometry.avg_speed,
                                        deltaT);
    
    // angle controller
    float theta_cmd_d = -theta_cmd_deriv.calculate(theta_cmd);                //TODO: why minus-understand thoroughly
    float theta_error = theta_cmd - roll_pitch_yaw.rpy[1] + theta_zero.theta;
    balance_duty = pitch_pid.calculate(theta_error,
                                       theta_cmd_d-gyros_accels.gyros[1],
                                       deltaT);
    
    // yaw constroller
    float yaw_error = (yaw_cmd - odometry.yaw)*180.0f/3.141593f;   //TODO: choose deg or rad
    delta_duty = yaw_pid.calculate(yaw_error,
                                   -gyros_accels.gyros[2],
                                   deltaT);
    
    //**************************************************************************
    //************** set duty cycles based on mode
    //**************************************************************************
    if (modes.op_state ==  normal)
    {
        if ((theta_error > 0.8)||(theta_error < -0.8))  // it has probably fallen
        {
            left_duty = right_duty = 0.0f;
            position_pid.resetIntegral();
            pitch_pid.resetIntegral();
            yaw_pid.resetIntegral();
            motion_commands.distance = odometry.avg_distance;
            motion_commands.yaw = odometry.yaw;
            glo_motion_commands.publish(&motion_commands);
        }
        else
        {
            left_duty = right_duty = balance_duty;
            left_duty += delta_duty;
            right_duty -= delta_duty;
        }
    }
    else if (modes.op_state == waiting_for_push_button)          
    {
        left_duty = right_duty = 0.0f;
    }
    else if (modes.op_state == getting_theta_zero)
    {
        // first order low_pass_filter with time constant of 2.5 seconds
        const float alpha = deltaT/2.5f;
        theta_zero.theta += alpha * (roll_pitch_yaw.rpy[1] - theta_zero.theta);
        glo_theta_zero.publish(&theta_zero);
        
        left_duty = right_duty = 0.0f;
    }
    else if (modes.op_state == debugging)
    {
        left_duty = right_duty = 0.0f;
    }
    
    left_duty *= MOTOR_SIGNS[0];
    right_duty *= MOTOR_SIGNS[1];
    hbridge.setDutyA(left_duty);
    hbridge.setDutyB(right_duty);
    
    // save the appropriate glo objects
    glo_odometry.publish(&odometry);

    watch_bit.clear();
}

/*****************************************************************************
* Function: 
*
* Description: A TASK FUNCTION!!!
*****************************************************************************/
void leds_task_callback(void)
{
    static DigitalOut watch_bit(PB9);

    static uint8_t count = 0;
    float batt_voltage;
    glo_modes_t modes;
    glo_raw_analog_t analog;
    
    watch_bit.set();

    // get appropriate glo objects
    glo_modes.read(&modes);
    glo_raw_analog.read(&analog);
    
    batt_voltage = BATTERY_SCALE*analog.voltages[8] + BATTERY_OFFSET;
    
    if ((!modes.params_set)
        && (modes.op_state == normal))
    {
        leds.toggle(orange);
        leds.toggle(red);
    }
    
    //use green & yellow for batt voltage. Solid green = good, solid yellow = bad!
    if (batt_voltage > 8.1f)       {leds.set(green);    leds.clear(yellow); }
    else if (batt_voltage > 7.4f)  {leds.toggle(green); leds.clear(yellow); }
    else if (batt_voltage > 6.7f)  {leds.toggle(green); leds.toggle(yellow);}
    else if (batt_voltage > 6.0f)  {leds.toggle(green); leds.set(yellow);   }
    else                           {leds.clear(green);  leds.set(yellow);   }
    
    
    count++;
    gleds.set(count);
    
    watch_bit.clear();
}
