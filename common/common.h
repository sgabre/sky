#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct GeographicCoordinate_ts
{
	double Latitude;
	double Longitude;
	double Altitude;	
}GeographicCoordinate_ts;

typedef struct TopocentricVector_ts
{
	double S;
	double E;
	double Z;   
}TopocentricVector_ts;

typedef struct TopocentricCoordinates_ts
{
	TopocentricVector_ts Position;
	TopocentricVector_ts Velocity;
}TopocentricCoordinates_ts;

typedef struct GeocentricVector_ts
{
	double I;
	double J;
	double K;    
}GeocentricVector_ts;

typedef struct GeocentricCoordinates_ts
{
	GeocentricVector_ts Position;
	GeocentricVector_ts Velocity;
}GeocentricCoordinates_ts;

#endif
