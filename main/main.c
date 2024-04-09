#include <stdio.h>

#include "common.h"

#include <gauss.h>
#include <intercept.h>
#include <kepler.h>
#include <LSTime.h>
#include <orbit.h>
#include <shell.h>
#include <site.h>
#include <track.h>
#include <angle.h>





int main(int argc, char *argv[]) 
{
	printf("main::START\n");
	gauss();
	intercept();
	kepler();
	LocalSiderealTime();
	orbit();
	shell();
	site();
	track();
	printf("main::STOP\n");
		
}