#ifndef _TRACK_H_
#define _TRACK_H_

#include "Common.h"

typedef struct RadarData_ts
{
	double Range;			// Range
	double Azimuth;			// Azimuth
	double Elevation; 		// Elevation
	double RangeRate;		// Range Rate
	double AzimuthRate;		// Azimuth Rate
	double ElevationRate; 	// Elevation Rate
}RadarData_ts;

typedef struct CelestialCoordinates_ts
{
	double Ascension;			// Topocentric right ascension
	double Declination;			// Topocentric declination
}CelestialCoordinates_ts;

typedef struct HorizontalCoordinate_ts
{
	double Azimuth;			// Azimuth
	double Elevation; 		// Elevation
}HorizontalCoordinate_ts;

void track(void);

 void track_Start(double aLocalSiderealTime, GeographicCoordinate_ts* aGeographicCoordinate, CelestialCoordinates_ts* aCelestialCoordinates, HorizontalCoordinate_ts* aHorizontalCoordinate);

#endif
