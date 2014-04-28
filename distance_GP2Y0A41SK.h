#ifdef __cplusplus
extern "C"{
#endif

// Ported to C from C++ from
// Arduino library for distance sensors
// Copyright 2011-2013 Jeroen Doggen (jeroendoggen@gmail.com)
// by Alexandru Paval 2014 (lexmazter@gmail.com)

/*
* This function will return the distance read from the sensor in centimeters
*/
int getDistanceCentimeter(int adcValue);
/*
* This function will return the distance read from the sensor in milimeters
*/
int getDistanceMilimeter(int adcValue);

#ifdef __cplusplus
} // extern "C"
#endif