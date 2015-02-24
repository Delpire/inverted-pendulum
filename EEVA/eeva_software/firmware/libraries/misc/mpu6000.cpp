/****************************************************************************************
* File: mpu6000.cpp - MPU6000 Driver Class
*
* Description: Interface to the accelerometer and gyroscope on the InvenSense MPU6000 
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/

#include <cstdio>
#include <cstdint>

#include "mpu6000.h"
#include "usec_timer.h"

UsecTimer usec_timer;

/*---------------------------------------------------------------------------------------
*                                      CONSTANTS
*--------------------------------------------------------------------------------------*/

#define CMD_READ     ( 0x80 )
#define CMD_WRITE    ( 0x00 )
#define ADDR_MASK    ( 0x7F )
#define WHO_I_AM     ( 0x68 )

// MPU 6000 registers
// NOTE: All registers are reset to 0x00 other than the following:
//       MPUREG_PWR_MGMT_1: 0x40 (Sleep mode/bit)
//       MPUREG_WHOAMI    : 0x68 (WHOAMI Answer)
#define MPUREG_PRODUCT_ID       ( 0x0C )
#define MPUREG_SMPLRT_DIV       ( 0x19 )
#define MPUREG_CONFIG           ( 0x1A )
#define MPUREG_GYRO_CONFIG      ( 0x1B )
#define MPUREG_ACCEL_CONFIG     ( 0x1C )
#define MPUREG_FIFO_EN          ( 0x23 )
#define MPUREG_INT_PIN_CFG      ( 0x37 )
#define MPUREG_INT_ENABLE       ( 0x38 )
#define MPUREG_INT_STATUS       ( 0x3A )
#define MPUREG_ACCEL_XOUT_H     ( 0x3B )
#define MPUREG_ACCEL_XOUT_L     ( 0x3C )
#define MPUREG_ACCEL_YOUT_H     ( 0x3D )
#define MPUREG_ACCEL_YOUT_L     ( 0x3E )
#define MPUREG_ACCEL_ZOUT_H     ( 0x3F )
#define MPUREG_ACCEL_ZOUT_L     ( 0x40 )
#define MPUREG_TEMP_OUT_H       ( 0x41 )
#define MPUREG_TEMP_OUT_L       ( 0x42 )
#define MPUREG_GYRO_XOUT_H      ( 0x43 )
#define MPUREG_GYRO_XOUT_L      ( 0x44 )
#define MPUREG_GYRO_YOUT_H      ( 0x45 )
#define MPUREG_GYRO_YOUT_L      ( 0x46 )
#define MPUREG_GYRO_ZOUT_H      ( 0x47 )
#define MPUREG_GYRO_ZOUT_L      ( 0x48 )
#define MPUREG_USER_CTRL        ( 0x6A )
#define MPUREG_PWR_MGMT_1       ( 0x6B )
#define MPUREG_PWR_MGMT_2       ( 0x6C )
#define MPUREG_SIGPATH_RESET    ( 0x68 )
#define MPUREG_FIFO_COUNTH      ( 0x72 )
#define MPUREG_FIFO_COUNTL      ( 0x73 )
#define MPUREG_FIFO_R_W         ( 0x74 )
#define MPUREG_WHOAMI           ( 0x75 )

// Configuration register bits

// MPUREG_PWR_MGMT_1
#define BIT_SLEEP               ( 0x40 )
#define BIT_H_RESET             ( 0x80 )
#define MPU_CLK_SEL_INT8MHZ     ( 0x00 )
#define MPU_CLK_SEL_PLLGYROX    ( 0x01 )
#define MPU_CLK_SEL_PLLGYROY    ( 0x02 )
#define MPU_CLK_SEL_PLLGYROZ    ( 0x03 )
#define MPU_CLK_SEL_PLLEXT32KHZ ( 0x04 )
#define MPU_CLK_SEL_PLLEXT19MHZ ( 0x05 )
#define MPU_CLK_SEL_STOPRESET   ( 0x07 )

#define MASK_CLKSEL             ( 0x07 )

// MPUREG_GYRO_CONFIG
// FS = Full Scale (range), DPS = degrees/sample
#define BITS_FS_250DPS          ( 0x00 )
#define BITS_FS_500DPS          ( 0x08 )
#define BITS_FS_1000DPS         ( 0x10 )
#define BITS_FS_2000DPS         ( 0x18 )
#define MASK_FS                 ( 0x18 )

// DLPF = Digital Low Pass Filter
// note: BW values listed in constant correspond to gyro
/*******************************************************************
 *           |   Accelerometer     |           Gyroscope           *
 * DLPF_CFG  | BW (Hz) | Delay(ms) | BW (Hz) | Delay(ms) | Fs(khz) *
 *   0x00    |   260   |    0.0    |   256   |   0.98    |    8    *
 *   0x01    |   184   |    2.0    |   188   |   1.90    |    1    *
 *   0x02    |    94   |    3.0    |    98   |   2.80    |    1    *
 *   0x03    |    44   |    4.9    |    42   |   4.80    |    1    *
 *   0x04    |    21   |    8.5    |    20   |   8.30    |    1    *
 *   0x05    |    10   |   13.8    |    10   |  13.40    |    1    *
 *   0x06    |     5   |   19.0    |     5   |  18.60    |    1    *
 *   0x07    |  RESVD  |    RESVD  |   RESVD |   RESVD   |    8    *
 *******************************************************************/
#define BITS_DLPF_CFG_256HZ_NOLPF2  ( 0x00 )
#define BITS_DLPF_CFG_188HZ         ( 0x01 )
#define BITS_DLPF_CFG_98HZ          ( 0x02 )
#define BITS_DLPF_CFG_42HZ          ( 0x03 )
#define BITS_DLPF_CFG_20HZ          ( 0x04 )
#define BITS_DLPF_CFG_10HZ          ( 0x05 )
#define BITS_DLPF_CFG_5HZ           ( 0x06 )
#define BITS_DLPF_CFG_2100HZ_NOLPF  ( 0x07 )
#define MASK_DLPF_CFG               ( 0x07 )

// MPUREG_INT_PIN_CFG
// Any read clears interrupt
#define BIT_INT_ANYRD_2CLEAR        ( 0x10 )

// MPUREG_INT_ENABLE
#define BIT_RAW_RDY_EN              ( 0x01 )

// MPUREG_USER_CTRL
#define BIT_I2C_IF_DIS              ( 0x10 )

// MPUREG_SIGPATH_RESET
#define BIT_SIGPATH_RESET_ALL       ( 0x07 )

// Product ID Description for MPU6000
// high 4 bits     low 4 bits
// Product Name    Product Revision
#define MPU6000ES_REV_C4            ( 0x14 )
#define MPU6000ES_REV_C5            ( 0x15 )
#define MPU6000ES_REV_D6            ( 0x16 )
#define MPU6000ES_REV_D7            ( 0x17 )
#define MPU6000ES_REV_D8            ( 0x18 )
#define MPU6000_REV_C4              ( 0x54 )
#define MPU6000_REV_C5              ( 0x55 )
#define MPU6000_REV_D6              ( 0x56 )
#define MPU6000_REV_D7              ( 0x57 )
#define MPU6000_REV_D8              ( 0x58 )
#define MPU6000_REV_D9              ( 0x59 )
#define MPU6000_REV_D10             ( 0x5A )

#define MPU6000_ONE_G   ( 9.80665f )

#define MPU6000_DEFAULT_ONCHIP_FILTER_FREQ  ( 42 )

/*---------------------------------------------------------------------------------------
*                                        MACROS
*--------------------------------------------------------------------------------------*/

// Chip Selection (CS) Functionality for MPU6000
#define CS_SELECT() GPIOE->BSRRH = GPIO_Pin_4
#define CS_DESELECT() GPIOE->BSRRL = GPIO_Pin_4

/*---------------------------------------------------------------------------------------
*                              SERIAL PERIPHERAL INTERFACE
*--------------------------------------------------------------------------------------*/

#define SPI_MAX_ATTEMPTS ( 100 ) // Timeout count until bus timeout

#define DEFAULT_SPI_FREQUENCY ( 1000000 ) // Default frequency for SPI bus
#define SENSOR_READ_SPI_FREQUENCY ( 11 * DEFAULT_SPI_FREQUENCY ) // Bus frequency when reading sensor data

// SPI4
#define SPI4_SCLK_GPIO_PORT ( GPIOE                )
#define SPI4_SCLK_GPIO_CLK  ( RCC_AHB1Periph_GPIOE )
#define SPI4_SCLK_GPIO_PIN  ( GPIO_Pin_2           )
#define SPI4_SCLK_AF_PIN    ( GPIO_PinSource2      )

#define SPI4_MISO_GPIO_PORT ( GPIOE                )
#define SPI4_MISO_GPIO_CLK  ( RCC_AHB1Periph_GPIOE )
#define SPI4_MISO_GPIO_PIN  ( GPIO_Pin_5           )
#define SPI4_MISO_AF_PIN    ( GPIO_PinSource5      )

#define SPI4_MOSI_GPIO_PORT ( GPIOE                )
#define SPI4_MOSI_GPIO_CLK  ( RCC_AHB1Periph_GPIOE )
#define SPI4_MOSI_GPIO_PIN  ( GPIO_Pin_6           )
#define SPI4_MOSI_AF_PIN    ( GPIO_PinSource6      )

/******************************************************************************
* Class: SPI (Serial Peripheral Interface)
*
* Description: Provides access and use of SPI bus
******************************************************************************/
class SPI
{
public: // methods

    // Returns the SPI object associated with the bus.  Initializes the bus if it
    // has not been initialized already. Returns NULL on failure.
    static SPI * Instance
        (
            spi_bus_id_t bus // Which bus to get instance of
        );

    // Sets the SPI bus frequency.
    void SetFrequency
        (
            uint32_t frequency // Desired bus frequency in [Hz]
        );

    // Sends AND receives a byte.  Returns the received byte.
    uint8_t SendByte
        (
            uint8_t byte // Byte to send over SPI bus
        );

private: // fields

    static bool init[SPI_BUS_COUNT]; // Indicates whether each bus is initialized already
    static SPI  objs[SPI_BUS_COUNT]; // Actual SPI bus objects

    // SPI bus information
    spi_bus_id_t bus;    // SPI bus ID
    SPI_TypeDef * base; // Base register for the SPI bus

}; // SPI

// Static class variable definitions
bool SPI::init[SPI_BUS_COUNT];
SPI  SPI::objs[SPI_BUS_COUNT];

/*---------------------------------------------------------------------------------------
*                               MPU6000 CLASS METHODS
*--------------------------------------------------------------------------------------*/

// Busy-waits for a given number of microseconds to pass.
static void usleep
    (
        uint32_t microseconds // Number of microseconds to busy-wait
    );

/*****************************************************************************
* Function: MPU6000
*
* Description: Constructor
*****************************************************************************/
MPU6000::MPU6000() :
    product_id( 0 ),
    spi( NULL ),
    spi_bus( SPI_BUS_4 ),
    gyro_range_scale( 0 ),
    accel_range_scale( 0 )
{
}

/*****************************************************************************
* Function: initialize
*
* Description: Initializes the SPI bus, Clock select, and verifies WHO_AM_I reg.
*              Returns a non-zero error code on failure.
*****************************************************************************/
int8_t MPU6000::initialize(void)
{
    spi = SPI::Instance( spi_bus );

    if ( spi == NULL ) { return -2; } // Unsuccessful bus setup.

    if ( !probe() )
    {
        //assert_always_msg( ASSERT_CONTINUE, "[MPU6000]: Init failed." );
        return -1; // Unsuccessful driver initialization
    }

    reset();

    return 0; // Successful driver initialization

}

/*****************************************************************************
* Function: readGyro
*
* Description: Reads and returns the X, Y, Z data from the gyroscope in rad/sec.
*****************************************************************************/
void MPU6000::readGyro
    (
        float * data // gyro data (X,Y,Z) in rad/sec.
    )
{
    // Setup a union to assist in reading/interpreting register data.
    union {
        int16_t raw;
        uint8_t bytes[2];
    } reading;

    CS_SELECT();

    // High-speed for sensor data
    spi->SetFrequency( SENSOR_READ_SPI_FREQUENCY );

    spi->SendByte( CMD_READ | MPUREG_GYRO_XOUT_H ); // Start here for gyroscope

    reading.bytes[1] = spi->SendByte( 0 );
    reading.bytes[0] = spi->SendByte( 0 );
    data[0] = convertRawGyro( reading.raw );

    reading.bytes[1] = spi->SendByte( 0 );
    reading.bytes[0] = spi->SendByte( 0 );
    data[1] = convertRawGyro( reading.raw );
    
    reading.bytes[1] = spi->SendByte( 0 );
    reading.bytes[0] = spi->SendByte( 0 );
    data[2] = convertRawGyro( reading.raw );
    
    CS_DESELECT();
    
    // Restore regular SPI bus speeds
    spi->SetFrequency( DEFAULT_SPI_FREQUENCY );

}

/*****************************************************************************
* Function: readAccel
*
* Description: Reads and returns the X, Y, Z data from the accelerometer in m/s/s
*****************************************************************************/
void MPU6000::readAccel
    (
        float * data // accel data (X,Y,Z) in m/s/s
    )
{
    // Setup a union to assist in reading/interpreting register data.
    union {
        int16_t raw;
        uint8_t bytes[2];
    } reading;

    CS_SELECT();

    // High-speed for sensor data
    spi->SetFrequency( SENSOR_READ_SPI_FREQUENCY );

    spi->SendByte( CMD_READ | MPUREG_INT_STATUS ); // start here for interrupt status, accelerometer data & temperature
    
    spi->SendByte( 0 ); // read interrupt status
    
    reading.bytes[1] = spi->SendByte( 0 );  // ACCEL_XOUT_H
    reading.bytes[0] = spi->SendByte( 0 );  // ACCEL_XOUT_L
    data[0] = convertRawAccel( reading.raw );
    
    reading.bytes[1] = spi->SendByte( 0 );  // ACCEL_YOUT_H
    reading.bytes[0] = spi->SendByte( 0 );  // ACCEL_YOUT_L
    data[1] = convertRawAccel( reading.raw );

    reading.bytes[1] = spi->SendByte( 0 );  // ACCEL_ZOUT_H
    reading.bytes[0] = spi->SendByte( 0 );  // ACCEL_ZOUT_L
    data[2] = convertRawAccel( reading.raw );

    // NOTE Convert temperature count to temperature via equation:
    //      temp in degrees C = ( TEMP_OUT (signed value) / 340 ) + 36.53
    //reading.bytes[1] = spi->SendByte( 0 );
    //reading.bytes[0] = spi->SendByte( 0 );
    // temperature = reading.raw;
    
    CS_DESELECT();
    
    // Restore regular SPI bus speeds
    spi->SetFrequency( DEFAULT_SPI_FREQUENCY );

}

/*****************************************************************************
* Function: reset
*
* Description: Resets the sensor to default config values.
*****************************************************************************/
void MPU6000::reset(void)
{
    // Reset sensor
    setRegister( MPUREG_PWR_MGMT_1, BIT_H_RESET );
    usleep( 10000 );

    // Datasheet says SPI should reset the signal path
    setRegister( MPUREG_SIGPATH_RESET, BIT_SIGPATH_RESET_ALL );
    usleep( 10000 );

    // Wake up device and select GyroZ clock. Note that the
    // MPU6000 starts up in sleep mode, and it can take some time
    // for it to come out of sleep
    setRegister( MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ );
    usleep( 1000 );

    // Disable I2C bus since we're using SPI
    setRegister( MPUREG_USER_CTRL, BIT_I2C_IF_DIS );
    usleep( 1000 );

    setSamplerate( 500 /*Hz*/ );
    usleep( 1000 );

    // Set DLPF (low pass filter) to default of 42Hz
    // **was 90 Hz, but this ruins quality and does not improve the system response
    setDLPFilter( MPU6000_DEFAULT_ONCHIP_FILTER_FREQ );
    usleep( 1000 );

    // Set gyroscope scale to 2000 deg/s 
    gyro_range_scale = (0.0174532f / 16.4f); // 1/(2^15)*(2000/180)*PI
    setRegister( MPUREG_GYRO_CONFIG, BITS_FS_2000DPS );
    usleep( 1000 );

    // Product-specific scaling for accelerometer.
    switch (product_id)
    {
        case MPU6000ES_REV_C4:
        case MPU6000ES_REV_C5:
        case MPU6000_REV_C4:
        case MPU6000_REV_C5:
            // Accel scale 8g (4096 LSB/g)
            // Rev C has different scaling than rev D
            setRegister( MPUREG_ACCEL_CONFIG, 1 << 3 );
            break;

        case MPU6000ES_REV_D6:
        case MPU6000ES_REV_D7:
        case MPU6000ES_REV_D8:
        case MPU6000_REV_D6:
        case MPU6000_REV_D7:
        case MPU6000_REV_D8:
        case MPU6000_REV_D9:
        case MPU6000_REV_D10:
        // default case to cope with new chip revisions, which
        // presumably won't have the accel scaling bug        
        default:
            // Accel scale 8g (4096 LSB/g)
            setRegister( MPUREG_ACCEL_CONFIG, 2 << 3 );
            break;
    }

    // Correct accel scale factors of 4096 LSB/g
    // scale to m/s^2 ( 1g = 9.81 m/s^2)
    accel_range_scale = ( MPU6000_ONE_G / 4096.0f );
    
    // Enable read interrupts.
    setRegister( MPUREG_INT_ENABLE, BIT_RAW_RDY_EN );
    usleep( 1000 );

    // Configure interrupt pin to clear on any read.
    setRegister( MPUREG_INT_PIN_CFG, BIT_INT_ANYRD_2CLEAR );
    usleep( 1000 );

} 

/*****************************************************************************
* Function: probe
*
* Description: Probes the sensor for correct WHOAMI and product revision.
*              Returns true if sensor responds correctly.
*****************************************************************************/
bool MPU6000::probe(void)
{
    bool ret = true;
    product_id = readRegister( MPUREG_PRODUCT_ID );

    switch ( product_id )
    {
        case MPU6000ES_REV_C4:
        case MPU6000ES_REV_C5:
        case MPU6000_REV_C4:
        case MPU6000_REV_C5:
        case MPU6000ES_REV_D6:
        case MPU6000ES_REV_D7:
        case MPU6000ES_REV_D8:
        case MPU6000_REV_D6:
        case MPU6000_REV_D7:
        case MPU6000_REV_D8:
        case MPU6000_REV_D9:
        case MPU6000_REV_D10:
            //debug_printf( "[MPU6000]: Sensor found.\r\n" );
            break;

        default:
            ret = false;
            //debug_printf( "[MPU6000]: Invalid product id (0x%x)\r\n", temp );
            break;
    }

    return ret;

}

/*****************************************************************************
* Function: setSamplerate
*
* Description: Configures the rate at which the sensor is sampled & output.
*****************************************************************************/
void MPU6000::setSamplerate
    (
        uint16_t desired_rate // Desired output/sample rate in [Hz]. 0 for maximum.
    )
{
    if ( desired_rate == 0 )
    {
        desired_rate = 1000;
    }

    uint8_t div = 1000 / desired_rate;

    if (div > 200)
    {
        div = 200;
    }
    else if (div < 1)
    {
        div = 1;
    }

    setRegister( MPUREG_SMPLRT_DIV, div-1 );

}

/*****************************************************************************
* Function: setDLPFilter
*
* Description: Configures the Digital Low Pass (DLP) Filter for gyroscope and
*              the accelerometer.
*****************************************************************************/
void MPU6000::setDLPFilter
    (
        uint16_t frequency // Frequency to configure the Low-Pass filter in [Hz]
    )
{
    uint8_t filter;

    // Choose next highest filter frequency available
    if ( frequency <= 5 )
    {
        filter = BITS_DLPF_CFG_5HZ;
    }
    else if ( frequency <= 10 )
    {
        filter = BITS_DLPF_CFG_10HZ;
    }
    else if ( frequency <= 20 )
    {
        filter = BITS_DLPF_CFG_20HZ;
    }
    else if ( frequency <= 42 )
    {
        filter = BITS_DLPF_CFG_42HZ;
    }
    else if ( frequency <= 98 )
    {
        filter = BITS_DLPF_CFG_98HZ;
    }
    else if ( frequency <= 188 )
    {
        filter = BITS_DLPF_CFG_188HZ;
    }
    else if ( frequency <= 256 )
    {
        filter = BITS_DLPF_CFG_256HZ_NOLPF2;
    }
    else
    {
        filter = BITS_DLPF_CFG_2100HZ_NOLPF;
    }

    setRegister( MPUREG_CONFIG, filter );

}

/*****************************************************************************
* Function: readRegister
*
* Description: Reads the register and returns the register's value.
*****************************************************************************/
uint8_t MPU6000::readRegister
    (
        uint8_t address // Address of register to read
    )
{
    address &= ADDR_MASK;

    CS_SELECT();

    spi->SendByte( CMD_READ | address );
    uint8_t value = spi->SendByte( 0 );

    CS_DESELECT();

    return value;

} 

/*****************************************************************************
* Function: setRegister
*
* Description: Sets the register value.
*****************************************************************************/
void MPU6000::setRegister
    (
        uint8_t address, // Address of register to set
        uint8_t value    // Value to set register
    )
{
    address &= ADDR_MASK;

    CS_SELECT();
    usleep(1);
    spi->SendByte( CMD_WRITE | address );
    spi->SendByte( value );
    usleep(1);
    CS_DESELECT();

} 

/*****************************************************************************
* Function: modifyRegister
*
* Description: Reads the exisitng register value, clears the clearbits and
*              sets the setbits.
*****************************************************************************/
void MPU6000::modifyRegister
    (
        uint8_t address,   // Address of register to modify
        uint8_t clearbits, // Bits to clear in register
        uint8_t setbits    // Bits to set in register
    )
{
    uint8_t value = readRegister(address);
    value &= ~clearbits;
    value |=  setbits;
    setRegister(address, value);

}

/*****************************************************************************
* Function: usleep
*
* Description: Busy-waits for a given number of microseconds to pass.
*****************************************************************************/
static void usleep
    (
        uint32_t microseconds // Number of microseconds to busy-wait
    )
{
  if (microseconds > 32767)  microseconds = 32767;
  
  usec_timer.UsecDelay( (uint16_t)microseconds );
  
}

/*---------------------------------------------------------------------------------------
*                                   SPI CLASS METHODS
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: Instance
*
* Description: Returns the SPI object associated with the bus.  Initializes
*              the bus if it has not been initialized already.
*              Returns NULL on failure.
*****************************************************************************/
SPI * SPI::Instance
    (
        spi_bus_id_t bus // Which bus to get instance of
    )
{
    SPI_InitTypeDef  SPI_InitStructure;

    if ( bus >= SPI_BUS_COUNT )
    {
        //assert_always_msg( ASSERT_CONTINUE, "[SPI]: Invalid SPI bus ID" );
        return NULL;
    }

    if ( init[bus] )
    {
        return &objs[bus];
    }

    switch ( bus )
    {
        case SPI_BUS_1:
            return NULL;

        case SPI_BUS_2:
            return NULL;

        case SPI_BUS_3:
            return NULL;
            
        case SPI_BUS_4:
            objs[bus].base = SPI4;

            // Deselect and Initialize GPIO chip select for MPU
            GPIO_InitTypeDef GPIO_InitStructure;
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIOE, &GPIO_InitStructure);
            GPIOE->BSRRL = GPIO_Pin_4;

            // Enable Required clocks for SPI
            RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI4, ENABLE );
            RCC_AHB1PeriphClockCmd( SPI4_SCLK_GPIO_CLK, ENABLE );
            RCC_AHB1PeriphClockCmd( SPI4_MISO_GPIO_CLK, ENABLE );
            RCC_AHB1PeriphClockCmd( SPI4_MOSI_GPIO_CLK, ENABLE );

            // Set GPIO Alternate Function for MOSI, MISO, SCLK
            GPIO_PinAFConfig( SPI4_SCLK_GPIO_PORT, SPI4_SCLK_AF_PIN, GPIO_AF_SPI4 );
            GPIO_PinAFConfig( SPI4_MOSI_GPIO_PORT, SPI4_MOSI_AF_PIN, GPIO_AF_SPI4 );
            GPIO_PinAFConfig( SPI4_MISO_GPIO_PORT, SPI4_MISO_AF_PIN, GPIO_AF_SPI4 );

            // Configure GPIO for MOSI, MISO, SCLK
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

            // SPI SCK pin configuration
            GPIO_InitStructure.GPIO_Pin = SPI4_SCLK_GPIO_PIN;
            GPIO_Init(SPI4_SCLK_GPIO_PORT, &GPIO_InitStructure);

            // SPI MOSI pin configuration
            GPIO_InitStructure.GPIO_Pin =  SPI4_MOSI_GPIO_PIN;
            GPIO_Init(SPI4_MOSI_GPIO_PORT, &GPIO_InitStructure);

            // SPI MISO pin configuration
            GPIO_InitStructure.GPIO_Pin =  SPI4_MISO_GPIO_PIN;
            GPIO_Init(SPI4_MISO_GPIO_PORT, &GPIO_InitStructure);

            // Configure SPI port
            SPI_I2S_DeInit( SPI4 );
            SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
            SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
            SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
            SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
            SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
            SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // set below by setFrequency
            SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
            SPI_Init( SPI4, &SPI_InitStructure );

            objs[bus].SetFrequency( DEFAULT_SPI_FREQUENCY );

            SPI_Cmd( SPI4, ENABLE );

            // Set bus as initialized and return SPI object
            init[bus] = true;
            return( &objs[bus] );

    } // switch( bus )

    return NULL; // Invalid BUS ID - should never reach here as it's checked above

}

/*****************************************************************************
* Function: SetFrequency
*
* Description: Sets the SPI bus frequency.
*****************************************************************************/
void SPI::SetFrequency
    (
        uint32_t frequency // Desired bus frequency in [Hz]
    )
{
    // Obtain PCLK2
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq( &RCC_Clocks );
    uint32_t pclk = RCC_Clocks.PCLK2_Frequency;

    // Get current CR1 settings
    uint16_t reg = base->CR1 & ~(0x0038);

    // Find closest frequency that does not exceed the given frequency
    if ( frequency >= ( pclk >> 1 ) )
    {
        reg |= (0x00 << 3);
    }
    else if ( frequency >= ( pclk >> 2 ) )
    {
        reg |= (0x01 << 3);
    }
    else if ( frequency >= ( pclk >> 3 ) )
    {
        reg |= (0x02 << 3);
    }
    else if ( frequency >= ( pclk >> 4 ) )
    {
        reg |= (0x03 << 3);
    }
    else if ( frequency >= ( pclk >> 5 ) )
    {
        reg |= (0x04 << 3);
    }
    else if ( frequency >= ( pclk >> 6 ) )
    {
        reg |= (0x05 << 3);
    }
    else if ( frequency >= ( pclk >> 7 ) )
    {
        reg |= (0x06 << 3);
    }
    else if ( frequency >= ( pclk >> 8 ) )
    {
        reg |= (0x07 << 3);
    }
    else
    {
        //assert_always_msg( ASSERT_CONTINUE, "[SPI]: Invalid frequency attempted to be set." );
        return;
    }

    base->CR1 = reg;

}

/*****************************************************************************
* Function: SendByte
*
* Description: Sends AND receives a byte.  Returns the received byte.
*****************************************************************************/
uint8_t SPI::SendByte
    (
        uint8_t byte // Byte to send over SPI bus
    )
{
    uint32_t attempts = 0;

    // Loop while DR register in not empty
    while (SPI_I2S_GetFlagStatus(this->base, SPI_I2S_FLAG_TXE) == RESET)
    {
        attempts++;
        if ( attempts >= SPI_MAX_ATTEMPTS )
        {
            // TODO: Reset SPI, possibly not just return 0
            //assert_always_msg( ASSERT_CONTINUE, "[SPI] bus timeout on TX." );
            return 0;
        }
    }

    // Send byte through the SPI peripheral
    SPI_I2S_SendData(this->base, byte);

    // Wait to receive a byte
    attempts = 0;
    while (SPI_I2S_GetFlagStatus(this->base, SPI_I2S_FLAG_RXNE) == RESET)
    {
        attempts++;
        if ( attempts >= SPI_MAX_ATTEMPTS )
        {
            // TODO: Reset SPI, possibly not just return 0 -MDR
            //assert_always_msg( ASSERT_CONTINUE, "[SPI] bus timeout on RX." );
            return 0;
        }
    }

    // Return the byte read from the SPI bus
    return ( SPI_I2S_ReceiveData(this->base) & 0x00FF );

}
