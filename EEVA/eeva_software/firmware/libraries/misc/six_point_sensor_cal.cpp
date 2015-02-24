/****************************************************************************************
* File: six_point_sensor_cal.cpp   
*
* Description: Algorithm that can be used to calibrate 3-axis, constant field, sensors
*              such as magnetometers and accelerometers.
*
* Created: 2/13/2014, by Dale Schinstock
****************************************************************************************/

/*---------------------------------------------------------------------------------------
*                                       INCLUDES
*--------------------------------------------------------------------------------------*/
#include <math.h>
#include "stdint.h"

#include "six_point_sensor_cal.h"

/*---------------------------------------------------------------------------------------
*                             Local Function Declarations
*--------------------------------------------------------------------------------------*/
bool guass_solve(double **a, int8_t n, double *x);

/*---------------------------------------------------------------------------------------
*                                 Functions
*--------------------------------------------------------------------------------------*/

/*****************************************************************************
* Function: six_point_sensor_cal   -  the exposed funtion.
*
* Description: 3 axis sensor cal from six measurements taken in a constant field.
* field_mag: field magnitude in which the measurements were made e.g. 9.81 for accels
* x, y, z are vectors of six measurements from different orientations
* returns, S[3] and b[3], that are the scale and offsett for the axes
* i.e. Measurementx = S[0]*Sensorx + b[0]
*****************************************************************************/
bool six_point_sensor_cal(double field_mag, const double x[6], const double y[6],
                          const double z[6], double S[3], double b[3] )
{
    int16_t i;
    double A[5][5];
    double *Aptrs[5] = {A[0], A[1], A[2], A[3], A[4]};
    double c[5];
    double xp, yp, zp, Sx;

    // Fill in matrix A -
    // write six difference-in-magnitude equations of the form
    // Sx^2(x2^2-x1^2) + 2*Sx*bx*(x2-x1) + Sy^2(y2^2-y1^2) + 2*Sy*by*(y2-y1) +
    //        Sz^2(z2^2-z1^2) + 2*Sz*bz*(z2-z1) = 0
    // or in other words
    // 2*Sx*bx*(x2-x1)/Sx^2  + Sy^2(y2^2-y1^2)/Sx^2  + 2*Sy*by*(y2-y1)/Sx^2  +
    //        Sz^2(z2^2-z1^2)/Sx^2  + 2*Sz*bz*(z2-z1)/Sx^2  = (x1^2-x2^2)
    for (i=0;i<5;i++)
    {
        A[i][0] = 2.0 * (x[i+1] - x[i]);
        A[i][1] = y[i+1]*y[i+1] - y[i]*y[i];
        A[i][2] = 2.0 * (y[i+1] - y[i]);
        A[i][3] = z[i+1]*z[i+1] - z[i]*z[i];
        A[i][4] = 2.0 * (z[i+1] - z[i]);
        c[i]    = x[i]*x[i] - x[i+1]*x[i+1];
    }

    // solve for c0=bx/Sx, c1=Sy^2/Sx^2; c2=Sy*by/Sx^2, c3=Sz^2/Sx^2, c4=Sz*bz/Sx^2
    if (  !guass_solve(Aptrs, 5, c) ) return 0;
  
    // use one magnitude equation and c's to find Sx - all give the same answer
    xp = x[0]; yp = y[0]; zp = z[0];
    Sx = sqrt(field_mag*field_mag / (xp*xp + 2*c[0]*xp + c[0]*c[0] + c[1]*yp*yp +
              2*c[2]*yp + c[2]*c[2]/c[1] + c[3]*zp*zp + 2*c[4]*zp + c[4]*c[4]/c[3]));

    S[0] = Sx;
    b[0] = Sx*c[0];
    S[1] = sqrt(c[1]*Sx*Sx);
    b[1] = c[2]*Sx*Sx/S[1];
    S[2] = sqrt(c[3]*Sx*Sx);
    b[2] = c[4]*Sx*Sx/S[2];

    return 1;
}

/*****************************************************************************
* Function: guass_sovle 
*
* Description: solves a set of n linear equations in the form A*x=b, using guassian
* elimination. b is passed in through *x, which is overwritten with the solution.
* A is passed in through **a: e.g.  double A[3][3]; double *a[e] = {A[0], A[1], A[2]};
* A is overwritten.
*****************************************************************************/
bool guass_solve(double **a, int8_t n, double *x)
{
    int8_t i,j,k,maxrow;
    double tmp;

    for (i=0;i<n;i++)   // Step through the columns creating and upper triangular
    {
        // Find the row with the largest first value in ith column
        maxrow = i;
        for (j=i+1;j<n;j++)
        {
            if (fabs(a[j][i]) > fabs(a[maxrow][i]))
                maxrow = j;
        }

        // Swap the maxrow and ith row 
        for (k=i;k<n;k++)
        {
            tmp = a[i][k];
            a[i][k] = a[maxrow][k];
            a[maxrow][k] = tmp;
        }
        tmp = x[i];
        x[i]= x[maxrow];
        x[maxrow]=tmp;
      
        // check for singular matrix
        if (fabs(a[i][i]) < 1e-30)
            return(false);

        // Eliminate the rest of the column. Note: doesn' actually zero them.
        // Just operates in the upper triangle
        for (j=i+1;j<n;j++)
        {
            tmp = a[j][i]/a[i][i];
            x[j] -= x[i] * tmp;
            for (k=i+1;k<n;k++)
                a[j][k] -= a[i][k] * tmp;
        }
    }

    // Do the back substitution 
    x[n-1]=x[n-1]/a[n-1][n-1];
    for (i=n-2;i>=0;i--)
    {
        for (j=n-1;j>i;j--)
            x[i] -= a[i][j] * x[j];
        x[i] = x[i]/a[i][i];
    }

    return(true);
}

/*
//  Simple test of calibration code. Compute the calibration values for
// each series and then process each data point.

// data from matlab with field = 9.81, S = 1.3, 1.2, 1.1, b = 0.4, 0.5, 0.6, orientations = six cube faces
double X[] = {   -0.307692307692308,  -0.307692307692308,  -0.307692307692308,  -0.307692307692308,  -7.853846153846154,   7.238461538461538};
double Y[] = {   -0.416666666666667,   7.758333333333334,  -8.591666666666667,  -0.416666666666666,  -0.416666666666667,  -0.416666666666667};
double Z[] = {    8.372727272727273,  -0.545454545454543,  -0.545454545454543,  -9.463636363636363,  -0.545454545454543,  -0.545454545454543};

// same data with noise added
double Xn[] = {   -0.304630729506626,  -0.314229657855888,  -0.316691808365736,  -0.311790159839709,  -7.847743101670414,   7.222203505797575};
double Yn[] = {   -0.428390015342200,   7.746039396843342,  -8.594523528042686,  -0.421702056507984,  -0.416075945116140,  -0.436314190820576};
double Zn[] = {    8.363117607091896,  -0.548164196754176,  -0.550078760898081,  -9.451303392509921,  -0.560124012758826,  -0.519402587945804};

void main()
{
  double Scale[3], Offset[3];

  SixPointInConstFieldCal( 9.81, X, Y, Z, Scale, Offset );
  SixPointInConstFieldCal( 9.81, Xn, Yn, Zn, Scale, Offset );
}
*/
