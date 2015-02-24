
#ifndef COORDINATECONVERSIONS_H_
#define COORDINATECONVERSIONS_H_

#include <stdbool.h>
#include <stdint.h>

// Converts Lat,Lon,Alt to ECEF
void lla_2_ecef(const double lla[3], // lat, lon, alt above ellipsoid [rad, rad, m]
                double ecef[3]);     // earth centered earth fixed [m]

//  Converts ECEF to Lat,Lon,Alt (iterative!) 
uint16_t ecef_2_lla(const double ecef[3], // earth centered earth fixed [m]
                    double lla[3]);      // lat, lon, alt above ellipsoid [rad, rad, m]

// Gets rotation matrix to rotate ECEF coordinates to NED coordinates for the given 
// LLA position on Earth
void Rne_from_lla(const float lla[3], // lat, lon, alt above ellipsoid [rad, rad, m]
                  float Rne[3][3]);   // rotation matrix

// Finds rotation matrix from rotation vector. If the vec is the rotation from
// frame 1 to frame 2 then R converts coordinates in frame 1 to frame 2
void rvec_2_R(const float rvec[3], // rotation vector [rad]
              float R[3][3]);      // rotation matrix

// Finds roll, pitch, yaw from quaternion
void quaternion_2_rpy(const float q[4], // quaternion
                      float rpy[3]);    // roll, pitch, yaw [rad]

// Finds quaternion from roll, pitch, yaw
void rpy_2_quaternion(const float rpy[3], // roll, pitch, yaw [rad]
                      float q[4]);        // quaternion

// Finds rotation matrix from quaternion.  If quaternion is rotoation from frame
// e to frame b, Rbe rotates a vector from e to b
void quaternion_2_R(const float q[4],     // quaternion
                    float Rbe[3][3]);     // rotation matrix

// Converts a vector from one ref frame to another using a quaternion. If q describes
// the rotation from e to b. It transforms vec from e to b. Reverse=true transforms
// vec from b to e.
void quaternion_rot(const float q[4],     // quaternion
                    const float vec[3],   // input vector
                    bool reverse,         // reverseses the rotation
                    float out_vec[3]);    // output vector

// Finds rotation matrix from roll, pitch, yaw. If rpy are the rotations from 
// frame e to frame b, then Rbe rotates a vector from e to b
void rpy_2_R(const float rpy[3], // roll, pitch, yaw [rad]
             float Rbe[3][3]);   // rotation matrix

// Finds quaternion from rotation matrix.  If quaternion is rotoation from frame
// e to frame b, Rbe rotates a vector from e to b
void R_2_quaternion(const float Rbe[3][3], // rotation matrix
                    float q[4]);           // quaternion

// Finds rotation matrix from two vector directions known in both frames.
// Given two vectors (v1 and v2) with directions known to be the same in two
// frames (b and e) find Rbe that rotates a vector from e to b.
// The solution is approximate if can't be exact.
uint8_t R_from_two_vectors(const float v1b[3], // v1 in b, will be nornalized
                           const float v1e[3], // v1 in e, will be normalized
                           const float v2b[3], // v2 in b, will be normalized
                           const float v2e[3], // v2 in e, will be normlized
                           float Rbe[3][3]);   // rotation matrix

// Finds vector cross product
void cross_product(const float v1[3],
                   const float v2[3],
                   float result[3]);  // result = v1 x v2

// Finds the components of v1 that are perpendicular to v2.
// result = v1 - (v1 dot v2)*v2/|v2|^2
void perpendicular_component(const float v1[3],
                             const float v2[3],
                             float result[3]);

// Finds vector magnitude
float vector_magnitude(const float v[3]);

// Overwrites quaternion, q, with it's inverse
void quat_inverse(float q[4]);

// Does quaternion multiplication qout=q1*q2 (not commutative)
void quat_mult(const float q1[4], const float q2[4], float qout[4]);

// Calculates the quaternion, qca describing rotation
// from a to c, much like for rotation matrices Rca=Rcb*Rba.
// Note however the way "quaternion multiplication" is defined this
// actually performs the operation qba*qcb if the * is the "quaternion
// multiplication" operator.
void quat_series(const float qcb[4], const float qba[4], float qca[4]); 

// Description: Multiplies a rotation matrix by a vector. Transpose multiplies by
// transpose which is the inverse.  vec_out = R*vec or R'*vec
void rot_mult(const float R[3][3], const float vec[3], float vec_out[3], bool transpose);

// Convets gps time (week, seconds of week) to year, month, day, hour, minutes,
// and milliseconds.  Ignores leap seconds, so could be off from UTC by several seconds
void gps_2_ymdhms(uint16_t gps_week, uint32_t msec_of_week, uint16_t *year,
                  uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute,
                  uint16_t *msec);

// Convets lla (deg*1e-7,cm) to NED (m) using a home lla and the slopes (deg/m)
void lla_2_ned_linear(const int32_t lla[3], const int32_t home_lla[3],
                      float delta_lat_north, float delta_lon_east, float ned[3]);

// Convets NED (m) to lla (deg*1e-7,cm) using a home lla and the slopes (deg/m)
void ned_2_lla_linear(const float ned[3], const int32_t home_lla[3],
                      float delta_lat_north, float delta_lon_east, int32_t lla[3]);

// Converts a gravity vector in accels to a rotation matrix, Rbe,
// assuming yaw is zero. Rbe is the matrix that converts a vector in the earth
// fixed frame to the body fixed frame. Accels is the gravity vector measured
// in the body frame.
void accels_2_zeroyaw_rotm(const float accels[3], float Rbe[3][3]);

#endif /* COORDINATECONVERSIONS_H_ */

