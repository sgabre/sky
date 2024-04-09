#ifndef _ORBIT_H_
#define _ORBIT_H_

//#include "common.h"

typedef struct OrbitalElement_ts
{
	//TRANSITION (INITIAL, FINAL, ENCOUNTER, ESCAPE, MANEUVER)
	//Seconds from now until apoapsis.
	//Seconds from now until periapsis.
	//Seconds from now until the next maneuver node.
	//Seconds from now until the next orbit patch starts.
	//GeocentricVector_ts H; 	// Angular Momentum Vector
	//GeocentricVector_ts N; 	// Node vector,
	//GeocentricVector_ts E; 	// Eccentricity Vectory
	double a;					// Semi-Major Axis
	double e;					// Eccentricity
	double i; 					// inclination
	double LAN;					// longitude of the ascending node 
	double AoP;					// argument of periapsis 
	double v;					// True Anomaly
	//semi-latus rectum
}OrbitalElement_ts;

void orbit(void);

//void orbit(body, a, e, i, lan, argPe, mEp, t);
//void orbit(body, Ra, Rp, i, lan, argPe, mEp, t);
//void orbit(body, pos, vel, ut);

#endif
