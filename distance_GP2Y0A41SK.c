#include "distance_GP2Y0A41SK.h"

// Ported to C from C++ from
// Arduino library for distance sensors
// Copyright 2011-2013 Jeroen Doggen (jeroendoggen@gmail.com)
// by Alexandru Paval 2014 (lexmazter@gmail.com)

/// getDistanceCentimeter(): Returns the distance in centimeters: between 4-36cm (3 & 37 are boundary values)
int getDistanceCentimeter(int adcValue)
{
	if (adcValue > 600)                             // lower boundary: 4 cm (3 cm means under the boundary)
	{
		return (3);
	}

	if (adcValue < 80)                             //upper boundary: 36 cm (returning 37 means over the boundary)
	{
		return (37);
	}

	else
	{
		return (1 / (0.000413153 * adcValue - 0.0055266887));
	}
}