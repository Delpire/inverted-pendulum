/****************************************************************************************
* File: mpu6000.h - MPU6000 Driver Class
*
* Description: Header file for mpu6000.cpp
****************************************************************************************/

#ifndef MPU6000_INCLUDED_H
#define MPU6000_INCLUDED_H

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include "stm32f4xx.h"

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

// SPI bus ID associated with each port.
typedef uint32_t spi_bus_id_t;
enum
{
    SPI_BUS_1 = 0,
    SPI_BUS_2,
    SPI_BUS_3,
    SPI_BUS_4,
    SPI_BUS_COUNT
};

/*---------------------------------------------------------------------------------------
*                                       CLASSES
*--------------------------------------------------------------------------------------*/

class SPI; // forward declaration


/******************************************************************************
* Class: MPU6000
*
* Description: Provides access and use of MPU6000 gyroscope
******************************************************************************/
class MPU6000
{

public: // methods

    // Constructor
    MPU6000();

    // Initializes and verifies the sensor.
    // Returns a non-zero error code on failure.
    int8_t initialize(void);

    // Reads and returns the X, Y, Z data from the gyroscope in rad/sec
    void readGyro
        (
            float * data // gyro data (X,Y,Z) in rad/sec.
        );
    
    // Reads and returns the X, Y, Z data from the accelerometer in m/s/s
    void readAccel
        (
            float * data // accel data (X,Y,Z) in m/s/s
        );

private: // methods

    // Resets the sensor to default config values.
    void reset(void);

    // Probes the sensor for correct WHOAMI and product revision.
    // Returns true if sensor responds correctly.
    bool probe(void);

    // Configures the rate at which the gyroscope is sampled & output.
    void setSamplerate
        (
            uint16_t desired_rate // Desired output/sample rate in [Hz]. 0 for maximum.
        );

    // Configures the Digital Low Pass (DLP) Filter for gyroscope and
    // the accelerometer.
    void setDLPFilter
        (
            uint16_t frequency // Frequency to configure the Low-Pass filter in [Hz]
        );

    // Reads the register and returns the register's value.
    uint8_t readRegister
        (
            uint8_t address // Address of register to read
        );

    // Sets the register value.
    void setRegister
        (
            uint8_t address, // Address of register to set
            uint8_t value    // Value to set register
        );

    // Reads the exisitng register value, clears the clearbits and sets the setbits.
    void modifyRegister
        (
            uint8_t address,   // Address of register to modify
            uint8_t clearbits, // Bits to clear in register
            uint8_t setbits    // Bits to set in register
        );

    // Convert raw gyro reading into rad/sec and return result.
    float convertRawGyro(int16_t raw) { return (float)raw * gyro_range_scale; }
    
    // Convert raw accel reading into m/s/s and return result.
    float convertRawAccel(int16_t raw) { return (float)raw * accel_range_scale; }
    
private: // fields

    // Product ID read from device
    uint8_t product_id;
  
    // SPI bus information
    SPI        * spi;     // SPI bus object
    spi_bus_id_t spi_bus; // SPI bus ID

    // Scale applied to raw gyro/accel readings to convert to useful units.
    float  gyro_range_scale;
    float  accel_range_scale;

};

#endif // MPU6000_INCLUDED_H
