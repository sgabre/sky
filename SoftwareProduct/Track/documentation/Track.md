
# Track

From a Celestial Coordinate object (e.g Start), the Track module compute the Horizontal coordinate (Azimuth & Elevation).

The Track Module compute the geocentric-equatorial components of position and velocity of the satellite (R and V).

The geographic location of a radar tracking site on the surface of the earth is known.

Its geodetic latitude, longitude, and altitude above mean sea level are specified.

Observations of a satellite are made by a radar at this site at a specified date and universal time.

The radar determines $\rho$, $\dot{\rho}$, $A_z$ , $\dot{A_z}$, $E$ , $\dot{E}$ from its tracking and doppler capability.

Note:

The Track module compute also the Horizontal components (Azimuth, Elevation) when the Right Ascension and the declination of a start is given.

# Usage

Start Tracker 

Track --asc --dec 

Object 

## Local hour angle 

The local hour angle (LHA) of an object in the observer's sky is

$$ H = \theta - \alpha$$

where,

&Theta; , is the Local Sidereal Time, in radians

&alpha; , is is the object's Right ascension, in radians


## Calcul de la Hauteur :

$$ h = \arcsin{\left(\sin{(\delta)} \cdot \sin{(\phi)} - \cos{(\delta)}\cdot \cos{(\phi)}\cdot \cos{(H)}\right )} $$

where,


* $\phi$, is the geographic latitude of the observation site, in radians
* $\delta$, is the celestial Déclinaison of the celesital object site, in radians
* $H$, is the local hours angle, in radians

  
## Calcul de l’Azimuth:

$$ \alpha = \arccos{\left ( \frac{\sin{(\delta)}-\sin{(\phi)}\cdot\sin{(h)}}{\cos{(\phi)} \cdot\cos{(h)}}\right )} $$


$$ \sin{(\alpha)} =  \frac{\cos{(\delta)}\cdot\sin{(H)}}{\cos{(h)}} $$


## Draft

Notations utilisées :

Latitude = lat
Longitude = longi
Déclinaison = dec
Ascension droite = asc
Azimut =az
Hauteur = hau
Angle Horaire de l'étoile = H = angle - asc + longi
angleH = angle lié a l'heure sidérale.
angleT = angle lié a l'heure.
angle = angleH + angleT
Calcul de la Hauteur :

sinushauteur = sin(dec) * sin(lat) - cos(dec) * cos(lat) * cos(H)
La hauteur est un angle compris entre -90° et +90°, la hauteur s'obtient donc simplement par :
hau = arcsin (sinushauteur)
Calcul de l'Azimut :

cosazimuth = ( sin(dec) - sin(lat) * sin(hau) ) / ( cos(lat) * cos(hau) )
L'azimut est un angle compris entre 0 et 360°, nous avons donc besoin d'un calcul intermédiaire :
sinazimuth = ( cos(dec) * sin (H) ) / cos(hau)
Si sinazimuth > 0 alors :
az = + arccos(cosazimuth)
Sinon :
az = - arccos(cosazimuth)












