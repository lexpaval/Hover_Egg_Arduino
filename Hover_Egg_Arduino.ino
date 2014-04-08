#include "pid.h"
#include "distance_GP2Y0A41SK.h"

const int pin_output = 11;	// this pin is linked to the red colored wire of the schematic
const int pin_input = A0;	// this pin is linked to the black colored wire
int setPoint = 150;			// the desired point for the egg (30mm - 400mm)
int error = 0;				// we store the error in this variable
int sensorValue = 0;		// we store the value returned by the sensor
int distanceValue = 0;		// we store the value of the distance in miliseconds as returned by adc_distance
int outputValue = 0;		// the value we output for the process
PID control;				// PID related variable
int serialByte = 0;			// we store here the value recieved from the Serial in ASCII
char serialData[3];			// we store the value recieved from serial converted from ASCII

void serialSetpoint()
{
	//set the last char as \0 to use for checking purposes
	serialData[2] = '\0';

	// send data only when you receive data:
	if (Serial.available() > 0) {

		// read the incoming byte
		serialByte = Serial.read();

		// convert from ASCII to actual numbers
		switch (serialByte)
		{
		case 48:
			strcat(serialData, "0");
			break;
		case 49:
			strcat(serialData, "1");
			break;
		case 50:
			strcat(serialData, "2");
			break;
		case 51:
			strcat(serialData, "3");
			break;
		case 52:
			strcat(serialData, "4");
			break;
		case 53:
			strcat(serialData, "5");
			break;
		case 54:
			strcat(serialData, "6");
			break;
		case 55:
			strcat(serialData, "7");
			break;
		case 56:
			strcat(serialData, "8");
			break;
		case 57:
			strcat(serialData, "9");
			break;

		default:
			// do nothing
			break;
		}

		// print on serial what you got
		Serial.print("I received: ");
		Serial.println(serialData);

		Serial.print("Checker: ");
		Serial.println(serialData[2]);
	}

	// set our point only if the last char is number
	if (serialData[2] != '\0'){
		setPoint = atoi(serialData);

		// zeroize the error as it's no longer good
		pid_zeroize(&control);

		// reset variables used for serial
		strcpy(serialData, "");
		serialData[2] = '\0';

		// print on serial what you got
		Serial.print("Point set at: ");
		Serial.println(setPoint);
	}

	// flush serial
	Serial.flush();
}

void setup()
{
	// initialise serial port
	Serial.begin(4800);

	/***************************************************/
	// manual tuning for PID v0.4 on milimeters
	control.derivative_gain = 0.6;
	control.integral_gain = 4.7;
	control.proportional_gain = 3.5;
	control.windup_guard = 0;
	// almost the same as above
	// but closer to the setpoint
}

void loop()
{
	// read the value from the sensor
	sensorValue = analogRead(pin_input);

	// print the value from the sensor
	/*Serial.print("sensorValue = ");
	Serial.println(sensorValue);*/

	// convert the value from adc to milimeters
	distanceValue = getDistanceCentimeter(sensorValue)*10;

	//print the distance value in milimeters
	Serial.print("distanceValue = ");
	Serial.println(distanceValue);

	// calculate the error based on distance
	error = distanceValue - setPoint;

	// send the error and calculate the PID
	pid_update(&control, error, 0.1);

	//print control
	/*Serial.print("control value = ");
	Serial.println(control.control);*/

	// enjoy
	analogWrite(pin_output, control.control); // from 0 to 255 - theoretically

	// check for serial setpoint
	serialSetpoint();
}