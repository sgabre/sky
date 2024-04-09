#include <stdio.h>

#include "track.h"


/** Usage
 *	track -start -lat 45°12'15'' N -lng 30°07'78'' W -dec +55° 57′ 35,4″ -asc 12h 54m 01,6s
 *    Azimuth: 30°
 *	Elevation: 45,5°
 *
 *	track -object -lat 45°12'15'' N -lng 30°07'78'' W -rng 1000 -azm 30 -elv 45 -rrate 1 -arate 2 -erate 3
*    Position Vector: 0.000000 I + 0.000000 J + 0.000000 K 
*	 Velocity Vector: 0.000000 I + 0.000000 J + 0.000000 K 
 *
 *	track -i start.json
 *    Azimuth: 30°
 *	Elevation: 45,5°
 *
 *	track -i grandours.json
 *  #1  Dubhe	Azimuth: 30°		Elevation: 45,5°
 *  #2  Merak	Azimuth: 30°		Elevation: 45,5°
 *  #3  Phecda	Azimuth: 30°		Elevation: 45,5°
 *  #4  Megrez	Azimuth: 30°		Elevation: 45,5°
 *  #5  Alioth	Azimuth: 30°		Elevation: 45,5°
 *  #6  Mizar	Azimuth: 30°		Elevation: 45,5°
 *  #7  Alkaid	Azimuth: 30°		Elevation: 45,5°
 * 
 *	track -i object.json (3 Position Vector or 3 HorizontalCoordinate)
 *    Azimuth: 30°
 *	Elevation: 45,5°
 *
 *	track -i Radar.json
 *    Position Vector: 0.000000 I + 0.000000 J + 0.000000 K 
 *	  Velocity Vector: 0.000000 I + 0.000000 J + 0.000000 K 
 *
 *  track -i Celestials.json (Line of Sight Unit Vector)
 *    Position Vector: 0.000000 I + 0.000000 J + 0.000000 K 
 *	  Velocity Vector: 0.000000 I + 0.000000 J + 0.000000 K 
 */
/**
* @brief 
* \param[in] aLocalSiderealTime TBD.
* \param[in] aCelestialCoordinates TBD.
* \param[in,out] aHorizontalCoordinate TBD.
*/
//void track_Start(double aLocalSiderealTime, GeographicCoordinate_ts* aGeographicCoordinate, CelestialCoordinates_ts* aCelestialCoordinates, HorizontalCoordinate_ts* aHorizontalCoordinate)
