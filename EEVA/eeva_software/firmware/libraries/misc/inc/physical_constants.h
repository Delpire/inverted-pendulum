#ifndef PHYSICAL_CONSTANTS_H_
#define PHYSICAL_CONSTANTS_H_

// Physical constants
#define GRAVITY 9.805f // [m/s^2]

// Trigonometry
#define PI 3.141593f 
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

#define PI_DOUBLE 3.14159265358979323846 // [-]
#define DEG2RAD_DOUBLE (PI_DOUBLE / 180.0)
#define RAD2DEG_DOUBLE (180.0 / PI_DOUBLE)

// Temperature and pressure conversions
#define CELSIUS2KELVIN     273.15f
#define INCHES_MERCURY2KPA  3.386f
#define KPA2HECTAPASCAL     0.001f
#define HECTAPASCAL2KPA     100.0f

// Distance conversions
#define FEET2MILES                       0.3048f
#define KNOTS2M_PER_SECOND          0.514444444f
#define FEET_PER_SECOND2CM_PER_SECOND     30.48f
#define METERS_PER_SECOND2KM_PER_HOUR       3.6f
#define KM_PER_HOUR2METERS_PER_SECOND (1.0f/3.6f)
#define NM2DEG_LAT                         60.0f  // 60 nautical miles per degree latitude
#define DEG_LAT2NM                     (1.0/60.f) // 1 degree latitude per 60 nautical miles

// Standard atmospheric constants
#define UNIVERSAL_GAS_CONSTANT           8.31447f // [J/(mol·K)]
#define DRY_AIR_CONSTANT                 287.058f // [J/(kg*K)]
#define STANDARD_AIR_DENSITY               1.225f // [kg/m^3]
#define STANDARD_AIR_LAPSE_RATE           0.0065f // [deg/m]
#define STANDARD_AIR_MOLS2KG           0.0289644f // [kg/mol]
#define STANDARD_AIR_RELATIVE_HUMIDITY      20.0f // [%]
#define STANDARD_AIR_SEA_LEVEL_PRESSURE 101325.0f // [Pa]
#define STANDARD_AIR_TEMPERATURE (15.0f + CELSIUS2KELVIN) // Standard temperatue in [K]
#define STANDARD_AIR_MACH_SPEED           340.27f //speed of sound at standard sea level in [m/s]

// WGS-84 definitions (from http://home.online.no/~sigurdhu/WGS84_Eng.html)
#define WGS84_RADIUS_EARTH_KM          6371.008f  // Earth's radius in km
#define WGS84_A                        6378.137f  // semi-major axis of the ellipsoid in km
#define WGS84_B                    6356.7523142f  // semi-minor axis of the ellipsoid in km
#define WGS84_FLATTENING     3.35281066474748e-3f // flattening, i.e. (1 / 298.257223563)
#define WGS84_EPS             8.1819190842622e-2f // first eccentricity, i.e. sqrtf(1-WGS84_B^2/WGS84_A^2)
#define WGS84_EPS2                6.694379990e-3f // first eccentricity squared, i.e. WGS84_EPS^2

#endif /* PHYSICAL_CONSTANTS_H_ */

/**
 * @}
 * @}
 */
