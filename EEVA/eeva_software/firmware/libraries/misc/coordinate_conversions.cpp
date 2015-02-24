#include <math.h>
#include "coordinate_conversions.h"

/*****************************************************************************
* Function: lla_2_ecef
*
* Description: converts Lat,Lon,Alt to ECEF
*****************************************************************************/
void lla_2_ecef(const double lla[3], // lat, lon, alt above ellipsoid [rad, rad, m]
                double ecef[3])      // earth centered earth fixed (m)
{
  const double a = 6378137.0;	      // Equatorial Radius (m)
  const double e = 8.1819190842622e-2;  // Eccentricity (m)
  double sinLat, sinLon, cosLat, cosLon;
  double N;

    sinLat = sin(lla[0]);
    sinLon = sin(lla[1]);
    cosLat = cos(lla[0]);
    cosLon = cos(lla[1]);

    N = a / sqrt(1.0 - e * e * sinLat * sinLat);	//prime vertical radius of curvature

    ecef[0] = (N + lla[2]) * cosLat * cosLon;
    ecef[1] = (N + lla[2]) * cosLat * sinLon;
    ecef[2] = ((1 - e * e) * N + lla[2]) * sinLat;
}

/*****************************************************************************
* Function: 
*
* Description: converts ECEF to Lat,Lon,Alt
* Iterative!, but usually takes very few iterations
*****************************************************************************/
uint16_t ecef_2_lla(const double ecef[3], // earth centered earth fixed [m]
                    double lla[3])        // lat, lon, alt above ellipsoid [rad, rad, m]
{
  const double a = 6378137.0;	        // Equatorial Radius
  const double e = 8.1819190842622e-2;	// Eccentricity
  double x = ecef[0], y = ecef[1], z = ecef[2];
  double Lat, N, NplusH, delta, esLat;
  uint16_t iter;

#define MAX_ITER 10		// should not take more than 5 for valid coordinates
#define ACCURACY 1.0e-11

    lla[1] = atan2(y, x);
    Lat = lla[0];
    esLat = e * sin(Lat);
    N = a / sqrt(1 - esLat * esLat);
    NplusH = N + lla[2];
    delta = 1;
    iter = 0;

    while (((delta > ACCURACY) || (delta < -ACCURACY)) && (iter < MAX_ITER))
    {
        delta = Lat - atan(z / (sqrt(x * x + y * y) * (1 - (N * e * e / NplusH))));
        Lat = Lat - delta;
        esLat = e * sin(Lat);
        N = a / sqrt(1 - esLat * esLat);
        NplusH = sqrt(x * x + y * y) / cos(Lat);
        iter += 1;
    }

    lla[0] = Lat;
    lla[2] = NplusH - N;

    return (iter < MAX_ITER);
}

/*****************************************************************************
* Function: 
*
* Description: Gets rotation matrix to rotate ECEF coordinates to NED coordinates
* for the given LLA position on Earth
*****************************************************************************/
void Rne_from_lla(const float lla[3], // lat, lon, alt above ellipsoid [rad, rad, m]
                  float Rne[3][3])    // rotation matrix
{
  float sinLat, sinLon, cosLat, cosLon;

    sinLat = sinf(lla[0]);
    sinLon = sinf(lla[1]);
    cosLat = cosf(lla[0]);
    
    cosLon = cosf(lla[1]);

    Rne[0][0] = -sinLat * cosLon;
    Rne[0][1] = -sinLat * sinLon;
    Rne[0][2] = cosLat;
    Rne[1][0] = -sinLon;
    Rne[1][1] = cosLon;
    Rne[1][2] = 0;
    Rne[2][0] = -cosLat * cosLon;
    Rne[2][1] = -cosLat * sinLon;
    Rne[2][2] = -sinLat;
}

/*****************************************************************************
* Function: 
*
* Description: finds roll, pitch, yaw from quaternion
*****************************************************************************/
void quaternion_2_rpy(const float q[4], // quaternion
                      float rpy[3])     // roll, pitch, yaw [rad]
{
  float R13, R11, R12, R23, R33;
  float q0s = q[0] * q[0];
  float q1s = q[1] * q[1];
  float q2s = q[2] * q[2];
  float q3s = q[3] * q[3];

    R13 = 2.0f * (q[1] * q[3] - q[0] * q[2]);
    R11 = q0s + q1s - q2s - q3s;
    R12 = 2.0f * (q[1] * q[2] + q[0] * q[3]);
    R23 = 2.0f * (q[2] * q[3] + q[0] * q[1]);
    R33 = q0s - q1s - q2s + q3s;

    rpy[1] = asinf(-R13);	// pitch always between -pi/2 to pi/2
    rpy[2] = atan2f(R12, R11);
    rpy[0] = atan2f(R23, R33);

    // Note: Because of the mathematical singularity at pitch = +/- pi/2, the 
    // roll and yaw will be very sensitive near those positions. This is 
    // actually very difficult to deal with in a manner that considers all
    // scenerios for the use of this function because it is a true flaw
    // in roll-pitch-yaw (or any Euler angle set). But, if well implemented the
    // atan2 should return results.
}

/*****************************************************************************
* Function: 
*
* Description: finds quaternion from roll, pitch, yaw
*****************************************************************************/
void rpy_2_quaternion(const float rpy[3], // roll, pitch, yaw [rad]
                      float q[4])         // quaternion
{
  float phi, theta, psi;
  float cphi, sphi, ctheta, stheta, cpsi, spsi;

    phi = rpy[0] / 2;
    theta = rpy[1] / 2;
    psi = rpy[2] / 2;
    cphi = cosf(phi);
    sphi = sinf(phi);
    ctheta = cosf(theta);
    stheta = sinf(theta);
    cpsi = cosf(psi);
    spsi = sinf(psi);

    q[0] = cphi * ctheta * cpsi + sphi * stheta * spsi;
    q[1] = sphi * ctheta * cpsi - cphi * stheta * spsi;
    q[2] = cphi * stheta * cpsi + sphi * ctheta * spsi;
    q[3] = cphi * ctheta * spsi - sphi * stheta * cpsi;

    if (q[0] < 0) 		// q0 always positive for uniqueness
    {
        q[0] = -q[0];
        q[1] = -q[1];
        q[2] = -q[2];
        q[3] = -q[3];
    }
}

/*****************************************************************************
* Function: 
*
* Description: Finds rotation matrix from quaternion.  If quaternion is rotoation
* from frame e to frame b, Rbe rotates a vector from e to b
*****************************************************************************/
void quaternion_2_R(const float q[4],     // quaternion
                    float Rbe[3][3])      // rotation matrix
{
  float q0s = q[0]*q[0], q1s = q[1]*q[1], q2s = q[2]*q[2], q3s = q[3]*q[3];

    Rbe[0][0] = q0s + q1s - q2s - q3s;
    Rbe[0][1] = 2 * (q[1] * q[2] + q[0] * q[3]);
    Rbe[0][2] = 2 * (q[1] * q[3] - q[0] * q[2]);
    Rbe[1][0] = 2 * (q[1] * q[2] - q[0] * q[3]);
    Rbe[1][1] = q0s - q1s + q2s - q3s;
    Rbe[1][2] = 2 * (q[2] * q[3] + q[0] * q[1]);
    Rbe[2][0] = 2 * (q[1] * q[3] + q[0] * q[2]);
    Rbe[2][1] = 2 * (q[2] * q[3] - q[0] * q[1]);
    Rbe[2][2] = q0s - q1s - q2s + q3s;
}

/*****************************************************************************
* Function: 
*
* Description:Converts a vector from one ref frame to another using a quaternion.
* If q describes the rotation from e to b, it transforms vec from e to b. Reverse
* transforms from vec from b to e.
*****************************************************************************/
void quaternion_rot(const float q[4],     // quaternion
                    const float vec[3],   // input vector
                    bool reverse,         // reverseses the rotation
                    float out_vec[3])    // output vector
{
  float q0,q1,q2,q3;
  float q11,q22,q33,q01,q02,q03,q12,q13,q23;
  
    if (reverse)
    {
        q0=q[0]; q1=-q[1]; q2=-q[2]; q3=-q[3];  // invert the quaternion
    }
    else
    {
        q0=q[0]; q1=q[1]; q2=q[2]; q3=q[3];
    }
    q11=q1*q1; q22=q2*q2; q33=q3*q3;
    q01=q0*q1; q02=q0*q2; q03=q0*q3; q12=q1*q2; q13=q1*q3; q23=q2*q3;

    out_vec[0] = (1-2*(q22 + q33))*vec[0] + 2*(q12 + q03)*vec[1] + 2*(q13 - q02)*vec[2];
    out_vec[1] = 2*(q12 - q03)*vec[0] + (1-2*(q11 + q33))*vec[1] + 2*(q23 + q01)*vec[2];
    out_vec[2] = 2*(q13 + q02)*vec[0] + 2*(q23 - q01)*vec[1] + (1-2*(q11 + q22))*vec[2];
}

/*****************************************************************************
* Function: 
*
* Description: Finds rotation matrix from roll, pitch, yaw. If rpy are the rotations
* from frame e to frame b, then Rbe rotates a vector from e to b
*****************************************************************************/
void rpy_2_R(const float rpy[3], // roll, pitch, yaw [rad]
             float Rbe[3][3])    // rotation matrix
{
  float sF = sinf(rpy[0]), cF = cosf(rpy[0]);
  float sT = sinf(rpy[1]), cT = cosf(rpy[1]);
  float sP = sinf(rpy[2]), cP = cosf(rpy[2]);
	
    Rbe[0][0] = cT*cP;
    Rbe[0][1] = cT*sP;
    Rbe[0][2] = -sT;
    Rbe[1][0] = sF*sT*cP - cF*sP;
    Rbe[1][1] = sF*sT*sP + cF*cP;
    Rbe[1][2] = cT*sF;
    Rbe[2][0] = cF*sT*cP + sF*sP;
    Rbe[2][1] = cF*sT*sP - sF*cP;
    Rbe[2][2] = cT*cF;
}

/*****************************************************************************
* Function: 
*
* Description: Finds quaternion from rotation matrix.  If quaternion is rotoation
* from frame e to frame b, Rbe rotates a vector from e to b
*****************************************************************************/
void R_2_quaternion(const float Rbe[3][3], // rotation matrix
                    float q[4])            // quaternion
{
  float m[4], mag;
  uint8_t index,i;

    m[0] = 1 + Rbe[0][0] + Rbe[1][1] + Rbe[2][2];
    m[1] = 1 + Rbe[0][0] - Rbe[1][1] - Rbe[2][2];
    m[2] = 1 - Rbe[0][0] + Rbe[1][1] - Rbe[2][2];
    m[3] = 1 - Rbe[0][0] - Rbe[1][1] + Rbe[2][2];

    // find maximum divisor
    index = 0;
    mag = m[0];
    for (i=1;i<4;i++)
    {
        if (m[i] > mag)
        {
            mag = m[i];
            index = i;
        }
    }
    mag = 2*sqrtf(mag);

    if (index == 0)
    {
        q[0] = mag/4;
        q[1] = (Rbe[1][2]-Rbe[2][1])/mag;
        q[2] = (Rbe[2][0]-Rbe[0][2])/mag;
        q[3] = (Rbe[0][1]-Rbe[1][0])/mag;
    }
    else if (index == 1)
    {
        q[1] = mag/4;
        q[0] = (Rbe[1][2]-Rbe[2][1])/mag;
        q[2] = (Rbe[0][1]+Rbe[1][0])/mag;
        q[3] = (Rbe[0][2]+Rbe[2][0])/mag;
     }
     else if (index == 2)
     {
        q[2] = mag/4;
        q[0] = (Rbe[2][0]-Rbe[0][2])/mag;
        q[1] = (Rbe[0][1]+Rbe[1][0])/mag;
        q[3] = (Rbe[1][2]+Rbe[2][1])/mag;
      }
      else
      {
          q[3] = mag/4;
          q[0] = (Rbe[0][1]-Rbe[1][0])/mag;
          q[1] = (Rbe[0][2]+Rbe[2][0])/mag;
          q[2] = (Rbe[1][2]+Rbe[2][1])/mag;
       }

      // q0 positive, i.e. angle between pi and -pi
      if (q[0] < 0)
      {
        q[0] = -q[0];
        q[1] = -q[1];
        q[2] = -q[2];
        q[3] = -q[3];
      }
}

/*****************************************************************************
* Function: 
*
* Description: Finds rotation matrix from two vector directions known in both frames.
* Given two vectors (v1 and v2) with directions known to be the same in two
* frames (b and e) find Rbe that rotates a vector from e to b.
* The solution is approximate if can't be exact.
*****************************************************************************/
uint8_t R_from_two_vectors(const float v1b[3], // v1 in b, will be nornalized
                           const float v1e[3], // v1 in e, will be normalized
                           const float v2b[3], // v2 in b, will be normalized
                           const float v2e[3], // v2 in e, will be normlized
                           float Rbe[3][3])    // rotation matrix
{
  float Rib[3][3], Rie[3][3];
  float mag;
  uint8_t i,j,k;

    // identity rotation in case of error
    for (i=0;i<3;i++)
    {
        for (j=0;j<3;j++)
            Rbe[i][j]=0;
        Rbe[i][i]=1;
    }

    // The first rows of rot matrices chosen in direction of v1
    mag = vector_magnitude(v1b);
    if (fabsf(mag) < 1e-30)
        return (0);
    for (i=0;i<3;i++)
        Rib[0][i]=v1b[i]/mag;

    mag = vector_magnitude(v1e);
    if (fabs(mag) < 1e-30)
        return (0);
    for (i=0;i<3;i++)
        Rie[0][i]=v1e[i]/mag;

    // The second rows of rot matrices chosen in direction of v1xv2
    cross_product(v1b,v2b,&Rib[1][0]);
    mag = vector_magnitude(&Rib[1][0]);
    if (fabsf(mag) < 1e-30)
        return (0);
    for (i=0;i<3;i++)
        Rib[1][i]=Rib[1][i]/mag;

    cross_product(v1e,v2e,&Rie[1][0]);
    mag = vector_magnitude(&Rie[1][0]);
    if (fabsf(mag) < 1e-30)
        return (0);
    for (i=0;i<3;i++)
        Rie[1][i]=Rie[1][i]/mag;

    // The third rows of rot matrices are XxY (Row1xRow2)
    cross_product(&Rib[0][0],&Rib[1][0],&Rib[2][0]);
    cross_product(&Rie[0][0],&Rie[1][0],&Rie[2][0]);

    // Rbe = Rbi*Rie = Rib'*Rie
    for (i=0;i<3;i++)
        for(j=0;j<3;j++)
        {
            Rbe[i][j]=0;
            for(k=0;k<3;k++)
                Rbe[i][j] += Rib[k][i]*Rie[k][j];
        }

    return 1;
}

/*****************************************************************************
* Function: 
*
* Description: Finds rotation matrix from rotation vector. If the vec describes the
* orientation of frame 2 in frame 1 then R converts coordinates in frame 1 to frame 2
*****************************************************************************/
void rvec_2_R(const float rvec[3], // rotation vector [rad]
              float R[3][3])       // rotation matrix
{
  float q[4];

    float angle = vector_magnitude(rvec);
    if (angle <= 0.00048828125f) 
    {
        // angle < sqrt(2*machine_epsilon(float)), so flush cos(x) to 1.0f
        q[0] = 1.0f;

        // and flush sin(x/2)/x to 0.5
        q[1] = 0.5f*rvec[0];
        q[2] = 0.5f*rvec[1];
        q[3] = 0.5f*rvec[2];
        // This prevents division by zero, while retaining full accuracy
    }
    else 
    {
        q[0] = cosf(angle*0.5f);
        float scale = sinf(angle*0.5f) / angle;
        q[1] = scale*rvec[0];
        q[2] = scale*rvec[1];
        q[3] = scale*rvec[2];
    }

    quaternion_2_R(q, R);
}

/*****************************************************************************
* Function: 
*
* Description: Finds vector cross product
*****************************************************************************/
void cross_product(const float v1[3],
                   const float v2[3],
                   float result[3])   // result = v1 x v2
{
    result[0] = v1[1]*v2[2] - v2[1]*v1[2];
    result[1] = v2[0]*v1[2] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v2[0]*v1[1];
}

/*****************************************************************************
* Function: 
*
* Description: Finds the components of v1 that are perpendicular to v2.
* result = v1 - (v1 dot v2)*v2/|v2|^2
*****************************************************************************/
void perpendicular_component(const float v1[3],
                             const float v2[3],
                             float result[3])
{
    float dot_mag_squared;
    
    dot_mag_squared = (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])/
                      (v2[0]*v2[0] + v2[1]*v2[1] + v2[2]*v2[2]);
    
    result[0] = v1[0] - dot_mag_squared*v2[0];
    result[1] = v1[1] - dot_mag_squared*v2[1];
    result[2] = v1[2] - dot_mag_squared*v2[2];
}

/*****************************************************************************
* Function: 
*
* Description: Finds vector magnitude
*****************************************************************************/
float vector_magnitude(const float v[3])
{
    return(sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
}

/*****************************************************************************
* Function: 
*
* Description: Overwrites quaternion, q, with it's inverse
*****************************************************************************/
void quat_inverse(float q[4]) 
{
    q[1] = -q[1];
    q[2] = -q[2];
    q[3] = -q[3];
}

/*****************************************************************************
* Function: 
*
* Description: Does quaternion multiplication qout=q1*q2 (not commutative)
*****************************************************************************/
void quat_mult(const float q1[4], const float q2[4], float qout[4]) 
{
    qout[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
    qout[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
    qout[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
    qout[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
}

/*****************************************************************************
* Function: 
*
* Description: Calculates the quaternion, qca describing rotation
* from a to c, much like for rotation matrices Rca=Rcb*Rba.
* Note however the way "quaternion multiplication" is defined this
* actually performs the operation qba*qcb if the * is the "quaternion
* multiplication" operator.
*****************************************************************************/
void quat_series(const float qcb[4], const float qba[4], float qca[4]) 
{
    qca[0] = qba[0]*qcb[0] - qba[1]*qcb[1] - qba[2]*qcb[2] - qba[3]*qcb[3];
    qca[1] = qba[0]*qcb[1] + qba[1]*qcb[0] + qba[2]*qcb[3] - qba[3]*qcb[2];
    qca[2] = qba[0]*qcb[2] - qba[1]*qcb[3] + qba[2]*qcb[0] + qba[3]*qcb[1];
    qca[3] = qba[0]*qcb[3] + qba[1]*qcb[2] - qba[2]*qcb[1] + qba[3]*qcb[0];
}

/*****************************************************************************
* Function: 
*
* Description: Multiplies a rotation matrix by a vector. Transpose multiplies by
* transpose which is the inverse.  vec_out = R*vec or R'*vec
*****************************************************************************/
void rot_mult(const float R[3][3], const float vec[3], float vec_out[3], bool transpose) 
{
    if (!transpose)
    {
        vec_out[0] = R[0][0] * vec[0] + R[0][1] * vec[1] + R[0][2] * vec[2];
        vec_out[1] = R[1][0] * vec[0] + R[1][1] * vec[1] + R[1][2] * vec[2];
        vec_out[2] = R[2][0] * vec[0] + R[2][1] * vec[1] + R[2][2] * vec[2];
    }
    else
    {
        vec_out[0] = R[0][0] * vec[0] + R[1][0] * vec[1] + R[2][0] * vec[2];
        vec_out[1] = R[0][1] * vec[0] + R[1][1] * vec[1] + R[2][1] * vec[2];
        vec_out[2] = R[0][2] * vec[0] + R[1][2] * vec[1] + R[2][2] * vec[2];
    }
}

/*****************************************************************************
* Function: 
*
* Description: Convets gps time (week, seconds of week) to year, month, day,
* hour, minutes, and milliseconds.
* Ignores leap seconds, so could be off from UTC by several seconds
*****************************************************************************/

#define JAN61980 44244
#define JAN11901 15385
void gps_2_ymdhms(uint16_t gps_week, uint32_t msec_of_week, uint16_t *year,
                  uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute,
                  uint16_t *msec)
{
  static const uint16_t month_day[2][13] = {
     {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
     {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
  };
  uint8_t leap, guess, more;
  uint32_t mod_julian_day, msec_of_day, days_fr_jan1_1901, yday; 
  uint32_t delta_yrs, num_four_yrs, years_so_far, days_left;

    msec_of_day = msec_of_week - (msec_of_week/86400000)*86400000; 
    *hour = msec_of_day/3600000;
    *minute = (msec_of_day-*hour*3600000)/60000;
    *msec = msec_of_day - *hour*3600000 - *minute*60000;

    mod_julian_day = gps_week*7 + msec_of_week/86400000 + JAN61980;
    days_fr_jan1_1901 = mod_julian_day - JAN11901;
    num_four_yrs = days_fr_jan1_1901/1461;
    years_so_far = 1901 + 4*num_four_yrs;
    days_left = days_fr_jan1_1901 - 1461*num_four_yrs;
    delta_yrs = days_left/365 - days_left/1460;
    *year = years_so_far + delta_yrs;
    yday = days_left - 365*delta_yrs + 1;
    leap = ( *year%4 == 0 );
    guess = (uint8_t)((float)yday*0.032f);
    more = (  ( (int32_t)yday - month_day[leap][guess+1] )  > 0  );
    *month = guess + more + 1;
    *day = yday - month_day[leap][guess+more];
}
/*****************************************************************************
* Function: 
*
* Description: Convets lla (deg*1e-7,cm) to NED (m) using a home lla and
* the slopes (deg/m)
*****************************************************************************/
void lla_2_ned_linear(const int32_t lla[3], const int32_t home_lla[3],
                      float delta_lat_north, float delta_lon_east, float ned[3])
{
    ned[0] = (float)(lla[0]-home_lla[0])*1e-7/delta_lat_north;
    ned[1] = (float)(lla[1]-home_lla[1])*1e-7/delta_lon_east;
    ned[2] = -(float)(lla[2]-home_lla[2])/100.0f;
}
/*****************************************************************************
* Function: 
*
* Description: Convets NED (m) to lla (deg*1e-7,cm) using a home lla and
* the slopes (deg/m)
*****************************************************************************/
void ned_2_lla_linear(const float ned[3], const int32_t home_lla[3],
                      float delta_lat_north, float delta_lon_east, int32_t lla[3])
{
    lla[0] = (int32_t)(ned[0]*delta_lat_north*1e7) + home_lla[0];
    lla[1] = (int32_t)(ned[1]*delta_lon_east*1e7) + home_lla[1];
    lla[2] = -(int32_t)(ned[2]*100.0f) + home_lla[2];
}

/*****************************************************************************
* Function: 
*
* Description: Converts a gravity vector in accels to a rotation matrix, Rbe,
* assuming yaw is zero. Rbe is the matrix that converts a vector in the earth
* fixed frame to the body fixed frame. Accels is the gravity vector measured
* in the body frame.
*****************************************************************************/
void accels_2_zeroyaw_rotm(const float accels[3], float Rbe[3][3])
{ 
    float mag = sqrtf(accels[0]*accels[0] + accels[1]*accels[1] + accels[2]*accels[2]);

    // z-axis is the negative of the gravity direction
    Rbe[0][2]= -accels[0]/mag;
    Rbe[1][2]= -accels[1]/mag;
    Rbe[2][2]= -accels[2]/mag;

    // body x-axis is in the earth's x-z plane along positive x
    Rbe[0][1] = 0.0f;
    Rbe[0][0] = sqrtf( 1.0f - Rbe[0][2]*Rbe[0][2] );

    // If pitch is close to +/- 90 deg then instead choose roll=0, to avoid
    // numerical issues.
    if (Rbe[0][0] < 0.01f)
    {
        // body y-axis is in the earth's x-y plane along positive y
        Rbe[1][0] = 0.0f;
        Rbe[1][1] = sqrt( 1.0f - Rbe[1][2]*Rbe[1][2] );

        // body y-axis dot x-axis = 0
        Rbe[0][1] = -Rbe[0][2]*Rbe[1][2]/Rbe[1][1];
        // body x-axis dot z-axis = 0
        Rbe[2][1] = -Rbe[2][2]*Rbe[1][2]/Rbe[1][1];
        
        // earth x-axis = y cross z
        Rbe[0][0] =  Rbe[1][1]*Rbe[2][2] - Rbe[2][1]*Rbe[1][2];
        Rbe[2][0] =  Rbe[0][1]*Rbe[1][2] - Rbe[1][1]*Rbe[0][2];
    }
    else            // finish computation if no singularity
    {
        // body x-axis dot y-axis = 0
        Rbe[1][0] = -Rbe[0][2]*Rbe[1][2]/Rbe[0][0];
        // body x-axis dot z-axis = 0
        Rbe[2][0] = -Rbe[0][2]*Rbe[2][2]/Rbe[0][0];

        // earth y-axis = y cross z
        Rbe[1][1] =  Rbe[2][2]*Rbe[0][0] - Rbe[0][2]*Rbe[2][0];
        Rbe[2][1] =  Rbe[0][2]*Rbe[1][0] - Rbe[1][2]*Rbe[0][0];
    }
}
