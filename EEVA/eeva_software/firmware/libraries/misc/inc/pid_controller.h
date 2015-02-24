/* -----------------------------------------------------------------------
implement a discrete time pid controller.
-------------------------------------------------------------------------*/

#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PidController
{
  public:
    //constructor
    PidController(float Kp,
                  float Ki,
                  float Kd,
                  float integral_lolimit,
                  float integral_hilimit,
                  float lolimit,
                  float hilimit);

    //destructor
    virtual ~PidController(void);

    //functions
    float calculate(float error, float derivative, float DT);
    void resetIntegral(void) { integral = 0.0f;};

    void setKp(float Kp) {this->Kp = Kp;};
    void setKi(float Ki) {this->Kp = Kp;};
    void setKd(float Kd) {this->Kp = Kp;};
    void setIntegralLolimit(float integral_lolimit) {this->integral_lolimit = integral_lolimit;};
    void setIntegralHilimit(float integral_hilimit) {this->integral_hilimit = integral_hilimit;};
    void setLolimit(float lolimit) {this->lolimit = lolimit;};
    void setHilimit(float hilimit) {this->hilimit = hilimit;};

  private:

    // parameters
    float Kp, Ki, Kd;                          // controller gains
    float integral_hilimit, integral_lolimit;  // integrator antiwindup limits
    float hilimit, lolimit;                    // output limits
    
    // integral portion of the controller
    float integral;
};

#endif