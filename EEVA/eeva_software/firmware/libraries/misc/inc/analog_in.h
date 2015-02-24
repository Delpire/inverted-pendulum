/****************************************************************************************
* File: analog_in.h
*
* Description: Setup ADC1 to scan the signals on pins below continuously using DMA to
*              write the values to the a buffer.  The channels and the order in which
*              they are scanned and placed in the buffer are listed below.
*
* Created: 
****************************************************************************************/
// Runs at 12032Hz for each 11 channel scan -- verified with testing.
// PIN  EEVA-SIGNAL     ADCCHANNEL
// PA1	QTR_1	        ADC1_IN1
// PA2	QTR_2	        ADC1_IN2
// PA3	QTR_3	        ADC1_IN3
// PA4	QTR_4	        ADC1_IN4
// PA5	QTR_5	        ADC1_IN5
// PA6	QTR_6	        ADC1_IN6
// PA7	QTR_7	        ADC1_IN7
// PC4	QTR_8	        ADC1_IN14
// PC5	BATT_SENS	ADC1_IN15
// PB0	MOTORB_SENS	ADC1_IN8
// PB1	MOTORA_SENS	ADC1_IN9

#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#include "stm32f4xx.h"

class AnalogIn
{
  public:
    //constructors
    AnalogIn(void);

    //destructor
    virtual ~AnalogIn(void);

    //functions
    void getVoltages(float voltages[11]);
    
  private:
    // used for testing speed
    void setupInterrupt(void);
    
    //The raw data   
    __IO uint16_t d_ADCRawValues[11];
};

#endif