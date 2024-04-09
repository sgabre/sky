#include <math.h>

#include "Angle.h"

double DMS(double aDegree, double aMinute, double aSeconds, char aPole)
{
	double Return = ( (double) aDegree) +( (double)aMinute/60) + ( (double)aSeconds/3600);
	
	if( (aPole == 'S') || (aPole == 'W') )
		{
			Return = -1.0 * Return;
		}
	return Return;
}

double Radians(double aAngle)
{
	return aAngle * M_PI/180;
}

double Degres(double aAngle)
{
	return aAngle * 180/M_PI;
}