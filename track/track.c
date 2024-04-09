#include <stdio.h>
#include <math.h>

#include "track.h"

void track(void) 
{
	printf("track\n");
}

/**
* @brief 
* \param[in] aLocalSiderealTime TBD.
* \param[in] aCelestialCoordinates TBD.
* \param[in,out] aHorizontalCoordinate TBD.
*/
void track_Start(double aLocalSiderealTime, GeographicCoordinate_ts* aGeographicCoordinate, CelestialCoordinates_ts* aCelestialCoordinates, HorizontalCoordinate_ts* aHorizontalCoordinate)
{	
	//ASSERT(aCelestialCoordinates != NULL, "ERR: Invalid Celestial Coordinates");
	//ASSERT(aHorizontalCoordinate != NULL, "ERR: Invalid Horizontal Coordinate");
	
	double HourAngle = aLocalSiderealTime - aCelestialCoordinates->Ascension;
	
	aHorizontalCoordinate->Azimuth = atan( -sin(HourAngle) / (tan(aCelestialCoordinates->Declination) * cos(aGeographicCoordinate->Latitude) - cos(HourAngle) * sin(aGeographicCoordinate->Latitude) ) );
	
	aHorizontalCoordinate->Elevation = asin(sin(aCelestialCoordinates->Declination) * sin(aGeographicCoordinate->Latitude) - cos(aCelestialCoordinates->Declination) * cos(aGeographicCoordinate->Latitude) * cos(HourAngle));

}