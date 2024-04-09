Intoduction
---------------

Table of Contents
---------------

[TOC]

Prerequisites
---------------

The Sky project described in this article is hosted on my [github](https://git-scm.com/) page. 

Tools used in this project can be found under the following URLs:

* [git] (https://git-scm.com/)
* [CMake] (https://cmake.org/)
* [Unity] (https://www.throwtheswitch.org/unity/)
* [CMock] (https://www.throwtheswitch.org/cmock)
* Compilators

The architecture in this project is base ont the Appendix D of the Fundamentals of Astrodynamics - Bate, Mueller, and White

The structure of the sky project is as follows:
.
|- main
|  \- (entry point of the main application)
|- LSTime
|  \- (module_a sources)
|- Site
|  \- (module_a sources)
|- Track
|  \- (module_a sources)
|- Predict
|  \- (module_a sources)
|- Kepler
|  \- (module_a sources)
|- Gauss
|  \- (module_a sources)
|- Intercept
|  \- (module_a sources)
\- test
\- (Unit-Tests sources)

The Site, Track, Predict, Kepler, Gauss, Intercept module contains a library of shared code. 

In this project it is both referenced by the main application as well as the unit-tests.

The external directory contains the sources from external projects or other git submodules. 

The main directory contains the production code and the test directory contains the source of the unit-tests.

LSTime Module
-----------------

The LSTime Module compute the local @ Greenwich sidereal time 

The Longitude and the local Date & Time is given.

Site/Track Module
-----------------

The Site/Track Module compute the geocentric-equatorial components of position and velocity of the radar site (RS and VS), and of the satellite (R and V).

The geographic location of a radar tracking site on the surface of the earth is known. 

Its geodetic latitude, longitude, and altitude above mean sea level are specified. 

Observations of a satellite are made by a radar at this site at a specified date and universal time. 

The radar determines p, p, EI, I, Az, A from its tracking and doppler capability.

Predict Module
-----------------

The Predict Module find orbital parameters from given vector position and velocity.

For a number of unidentified space objects the three components of vector position and velocity are generated from radar observations 

The Predict Module find:

a. The type of trajectory (circular, rectilinear, elliptical, parabolic, hyperbolic).
b. Position and velocity vectors at impact or closest approach in the geocentric-equatorial coordinate system.
c. Time for object to go from its observed position to point of impact or closest approach.
d. The total change in true anomaly from the observed position to impact or point of closest approach.

Kepler Module
-----------------
The Kepler Module determines the position and velocity vectors after an interval of time 

The position and velocity vectors of a satellite at a particular instant of time is given

Gauss Module
-----------------

The Gauss Module determines the departure & arrival velocity base on the "short way" or "long way".

The departure position and arrival position at expected time for a conic orbit is given.

Intercept Module
-----------------

The Intercept Module computes the impulsive velocity change required for both intercept and intercept­ plus-rendezvous for various combinations of launch time and interceptor time-of-flight. 

Neglect the atmosphere and assume impulsive velocity change from the launch site and at the target. 

The radar tracking data on a target satellite, location of the tracking site, time of radar observation, and the location of an interceptor launch site is given. 
