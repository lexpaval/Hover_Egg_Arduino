#include "pid.h"

const int pin_output = 11;	// this pin is linked to the red colored wire of the schematic
const int pin_input = A0;	// this pin is linked to the black colored wire (min = 77 | max = ~500)
int setPoint = 250;			// the desired point for the ball | max 360
int error = 0;				// we store the error in this variable
int sensorValue = 0;		// we store the value returned by the sensor
int outputValue = 0;		// the value we output for the process
PID control;				// PID related variable
int serialByte = 0;			// we store here the value recieved from the Serial in ASCII
char serialData[3];			// we store the value recieved from serial converted from ASCII
static volatile unsigned char serialCounter = 0;	// the counter to read the serial 3 times then put the Setpoint	

void serialSetpoint()
{
	// send data only when you receive data:
	if (Serial.available() > 0) {

		// increment the counter
		serialCounter++;

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

		Serial.print("Counter at: ");
		Serial.println(serialCounter);
	}

	// set our point only after we've read 3 numbers
	if (serialCounter == 3){
		setPoint = atoi(serialData);

		// zeroize the error as it's no longer good
		pid_zeroize(&control);

		// reset variables used for serial
		strcpy(serialData, "");
		serialCounter = 0;

		// print on serial what you got
		Serial.print("Point set at: ");
		Serial.println(setPoint);
	}
}

void setup()
{
	// initialise serial port
	Serial.begin(9600);

	/*****************************************************/
	// manual tuning for PID v0.1
	
	control.derivative_gain = 0.6;
	control.integral_gain = 0.2;
	control.proportional_gain = 0.6;
	control.windup_guard = 10;
	
	// works from ~100 -> 360

	/****************************************************/
	// manual tuning for PID v0.2
	/*
	control.derivative_gain = 0.1;
	control.integral_gain = 0.2;
	control.proportional_gain = 0.1;
	control.windup_guard = 1;
	*/
	// works overall but with around 50 points behind
	// and massive overshoot when going down

	/***************************************************/
	// manual tuning for PID v0.3
	//control.derivative_gain = 0.3;
	//control.integral_gain = 7.5;
	//control.proportional_gain = 0.4;
	//control.windup_guard = 1;
	// almost the same as above
	// but closer to the setpoint
}

void loop()
{
	// read the value from the sensor
	sensorValue = analogRead(pin_input);

	// calculate the error
	error = sensorValue - setPoint;

	// send the error and calculate the PID
	pid_update(&control, error, 0.1);

	// enjoy
	analogWrite(pin_output, control.control); // from 0 to 255 - theoretically

	// check for serial setpoint
	serialSetpoint();

	// print the value from the sensor
	Serial.print("sensorValue = ");
	Serial.println(sensorValue);
}