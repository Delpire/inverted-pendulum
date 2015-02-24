/* -----------------------------------------------------------------------
implement a discrete time filter for speed/derivative of the following form
V(k) = a0*P(k) + a1*P(k-1) + a2*P(k-2) - b1*V(k-1) - b2*V(k-2);
that approximates a 2nd order low-pass with a derivative
V(s)/P(s) = s*wn^2/(s^2 + 2*zeta*wn*s +wn^2)

Calculate() function must be called periodically at the specified sample rate,
passing in the position as the argument.
-------------------------------------------------------------------------*/

#ifndef DERIVATIVEFILTER_H
#define DERIVATIVEFILTER_H

class DerivativeFilter
{
  public:
    //constructor
    DerivativeFilter(float sampleT, float CuttoffFrequency, float DampingRatio);

    //destructor
    virtual ~DerivativeFilter(void);

    //functions
    float calculate(float Input);

  private:
    // filter coefficients
    float a0,a1,a2,b1,b2;

    // previous values
    float Vk_1, Vk_2, Pk_1, Pk_2;
};

#endif