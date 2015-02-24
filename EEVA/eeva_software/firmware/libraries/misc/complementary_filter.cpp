/****************************************************************************************
* File: 
*
* Description:
*
* Created: 
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>

#include "complementary_filter.h"
#include "coordinate_conversions.h"
#include "physical_constants.h"

/*---------------------------------------------------------------------------------------
*                                   LITERAL CONSTANTS
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                        TYPES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
                                    MEMORY CONSTANTS
---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                      VARIABLES
*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
*                                     PROCEDURES
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function:
*
* Description:
*****************************************************************************/
ComplementaryFilter::ComplementaryFilter(void)
{
    float cuttoff = 6.28f/6.0f;
    
    Kpa = cuttoff;
    Kia = cuttoff*cuttoff;
    
    quat[0]=1.0f;
    quat[1]=quat[2]=quat[3]=0.0f;
}

/*****************************************************************************
* Function: 
*
* Description:
*****************************************************************************/
void ComplementaryFilter::update(float dT,
                                 const float gyros[3],
                                 const float accels[3])
{
    static float att_integral[3] = {0.0f, 0.0f, 0.0f};

    float error[3];
    float accels_mag, predicted_accels_mag;
    float accels_direction[3];
    float accels_predicted[3];
    float Rbe[3][3];
    float up[3];
    
    quaternion_2_R(quat,Rbe);  // find the current rotation matrix from state quaternion
    up[0] = -Rbe[0][2];        // Up is the negative NED z-axis.
    up[1] = -Rbe[1][2];        // In the body frame this is neg
    up[2] = -Rbe[2][2];        // of the last column of Rbe.

    //*************************************
    // DO THE COMPLEMENTARY ATTITUDE FILTER
    //*************************************
    
    // predicted accels = corrections + gravity, corrections should be zero if not used
    accels_predicted[0] = up[0]*GRAVITY;
    accels_predicted[1] = up[1]*GRAVITY;
    accels_predicted[2] = up[2]*GRAVITY;

    // find magnitudes and unit vectors, if either is small avoid division by zero
    predicted_accels_mag = vector_magnitude(accels_predicted);
    accels_mag = vector_magnitude(accels);
    
    if ( (fabsf(predicted_accels_mag) < 1e-30) || (fabsf(accels_mag) < 1e-30) )
    {
        // skip the correction this time by zeroing one of the vectors
        accels_direction[0]=accels_direction[1]=accels_direction[2]=0.0f;
    }
    else
    {
        // calculate direction of accels and predicted accels
        accels_direction[0] = accels[0]/accels_mag;
        accels_direction[1] = accels[1]/accels_mag;
        accels_direction[2] = accels[2]/accels_mag;
        accels_predicted[0] /= predicted_accels_mag;
        accels_predicted[1] /= predicted_accels_mag;
        accels_predicted[2] /= predicted_accels_mag;
    }
    
    // find rotation (error term for PI) in direction from predicted towards measured
    cross_product(accels_direction, accels_predicted, error);

    // do the integrals
    att_integral[0] += error[0]*dT;    // Do we need to limit windup?
    att_integral[1] += error[1]*dT;
    att_integral[2] += error[2]*dT;

    // add PI corrections to gyros
    float wx, wy, wz;
    wx = gyros[0] + Kpa*error[0] + Kia*att_integral[0];
    wy = gyros[1] + Kpa*error[1] + Kia*att_integral[1];
    wz = gyros[2] + Kpa*error[2] + Kia*att_integral[2];
    
    // Do numerical integration, angular rates = gryos + corrections
    float qdot[4];
    qdot[0] = (-quat[1]*wx - quat[2]*wy - quat[3]*wz) / 2.0f;
    qdot[1] = ( quat[0]*wx - quat[3]*wy + quat[2]*wz) / 2.0f;
    qdot[2] = ( quat[3]*wx + quat[0]*wy - quat[1]*wz) / 2.0f;
    qdot[3] = (-quat[2]*wx + quat[1]*wy + quat[0]*wz) / 2.0f;

    // Take a time step
    quat[0] += qdot[0]*dT;
    quat[1] += qdot[1]*dT;
    quat[2] += qdot[2]*dT;
    quat[3] += qdot[3]*dT;
    
    // Clean-up, assure quat[0] is pos and make sure unit vector
    if(quat[0] < 0)
    {
        quat[0] = -quat[0];
        quat[1] = -quat[1];
        quat[2] = -quat[2];
        quat[3] = -quat[3];
    }
    float qmag;
    qmag = sqrtf(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
    quat[0] = quat[0] / qmag;
    quat[1] = quat[1] / qmag;
    quat[2] = quat[2] / qmag;
    quat[3] = quat[3] / qmag;
}

/*****************************************************************************
* Function: 
*
* Description:
*****************************************************************************/
void ComplementaryFilter::set_attitude(const float quaternion[4])
{
    quat[0]=quaternion[0];
    quat[1]=quaternion[1];
    quat[2]=quaternion[2];
    quat[3]=quaternion[3];
   
    // Clean-up, to make sure, assure quat[0] is pos and make sure unit vector
    if(quat[0] < 0)
    {
        quat[0] = -quat[0];
        quat[1] = -quat[1];
        quat[2] = -quat[2];
        quat[3] = -quat[3];
    }
    float qmag;
    qmag = vector_magnitude(quat);
    quat[0] = quat[0] / qmag;
    quat[1] = quat[1] / qmag;
    quat[2] = quat[2] / qmag;
    quat[3] = quat[3] / qmag;
}

/*****************************************************************************
* Function: 
*
* Description:
*****************************************************************************/
void ComplementaryFilter::get_state(float quaternion[4])
{
   quaternion[0]=quat[0];
   quaternion[1]=quat[1];
   quaternion[2]=quat[2];
   quaternion[3]=quat[3];
}
