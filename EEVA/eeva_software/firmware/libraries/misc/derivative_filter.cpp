//-------------------------------------------------------------------------
//  Implementation of DerivativeFilter
//-------------------------------------------------------------------------
#include "derivative_filter.h"

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
DerivativeFilter::DerivativeFilter(float sampleT, float CuttoffFrequency, float DampingRatio)
{
  float wn=2*3.14159*CuttoffFrequency;
  float T=sampleT;
  float zeta=DampingRatio;
  float b0;
        
  //set filter coefficients
  b0=(T*T*wn*wn + 4*zeta*T*wn + 4);
  b1=(2*T*T*wn*wn - 8)/b0;
  b2=(T*T*wn*wn - 4*zeta*T*wn + 4)/b0;
  a0=2*T*wn*wn/b0;
  a1=0/b0;
  a2=(-2*T*wn*wn)/b0;

  // init previous values
  Vk_1 = Vk_2 = Pk_1 = Pk_2 = 0;
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
DerivativeFilter::~DerivativeFilter()
{
}

/*****************************************************************************
* Function:
*
* Description: 
*****************************************************************************/
float DerivativeFilter::calculate(float Position)
{
  float Speed;
	
  Speed = a0*Position + a1*Pk_1 + a2*Pk_2 - b1*Vk_1 - b2*Vk_2;
    
  Pk_2=Pk_1; Pk_1=Position; Vk_2=Vk_1; Vk_1=Speed;

  return(Speed);
}