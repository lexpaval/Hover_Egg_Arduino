#include "pid.h"
#include "distance_GP2Y0A41SK.h"
#include <SoftwareSerial.h>

const int pin_output = 11;	// this pin is linked to the red colored wire of the schematic
const int pin_input = A0;	// this pin is linked to the black colored wire
const int bt_tx = 4;		// this pin is linked to the transmission pin on BT module
const int bt_rx = 2;		// this pin is linked to the recieving pin on BT module
double setPoint = 150;		// the desired point for the egg (30mm - 400mm)
double auxSetPoint = 150;	// the intermediate setpoint to get the damping effect on going down
double pid_offset = 60;		// the offset of the pid setpoint
double error = 0;			// we store the error in this variable
double sensorValue = 0;		// we store the value returned by the sensor
double distanceValue = 0;	// we store the value of the distance in miliseconds as returned by adc_distance
double outputValue = 0;		// the value we output for the process
PID control;				// PID related variable
int serialByte = 0;			// we store here the value recieved from the Serial in ASCII
char serialData[3];			// we store the value recieved from serial converted from ASCII
SoftwareSerial bluetooth(bt_tx, bt_rx); // this creates a Serial connection with the BT module
int buffer[20];

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

	}

	// set our point only if the last char is number
	if (serialData[2] != '\0'){
		setPoint = atoi(serialData);

		// zeroize the error as it's no longer good
		pid_zeroize(&control);

		// reset variables used for serial
		strcpy(serialData, "");
		serialData[2] = '\0';
	}

	// flush serial
	Serial.flush();
}

void bluetoothSetpoint()
{
		//set the last char as \0 to use for checking purposes
		serialData[2] = '\0';

		// send data only when you receive data:
		if (bluetooth.available() > 0) {

			// read the incoming byte
			serialByte = bluetooth.read();

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
		}

		// set our point only if the last char is number
		if (serialData[2] != '\0'){
			setPoint = atoi(serialData);

			// zeroize the error as it's no longer good
			pid_zeroize(&control);

			// reset variables used for serial
			strcpy(serialData, "");
			serialData[2] = '\0';
		}
}

void plot(int data1, int data2, int data3, int data4)
{
	int pktSize;

	buffer[0] = 0xCDAB;             //SimPlot packet header. Indicates start of data packet
	buffer[1] = 4 * sizeof(int);      //Size of data in bytes. Does not include the header and size fields
	buffer[2] = data1;
	buffer[3] = data2;
	buffer[4] = data3;
	buffer[5] = data4;

	pktSize = 2 + 2 + (4 * sizeof(int)); //Header bytes + size field bytes + data

	//IMPORTANT: Change to serial port that is connected to PC
	Serial.write((uint8_t *)buffer, pktSize);
}

void setup()
{
	// initialise serial port
	Serial.begin(9600);

	// initialise BT port
	bluetooth.begin(115200);
	bluetooth.print("$$$");
	delay(100);
	bluetooth.println("U,9600,N");
	bluetooth.begin(9600);

	/***************************************************/
	// manual tuning for PID on milimeters
	control.derivative_gain = 2.5;
	control.integral_gain = 0.4;
	control.proportional_gain = 1.4;
	control.windup_guard = 1;
}

void loop()
{
	// read the value from the sensor
	sensorValue = analogRead(pin_input);

	// convert the value from adc to milimeters
	distanceValue = getDistanceMilimeter(sensorValue);

	// damping effect on going down
	if (setPoint > auxSetPoint){
		auxSetPoint++;
	}
	else {
		// set the setpoint straight on going up
		auxSetPoint = setPoint;
	}

	// calculate the error based on distance
	error = (distanceValue - auxSetPoint) + pid_offset;

	// plot function for simplot
	//plot(distanceValue, setPoint, 0, 0);

	// send the error and calculate the PID
	pid_update(&control, error, 0.1);

	// enjoy
	analogWrite(pin_output, control.control); // from 0 to 255

	// check for serial setpoint
	serialSetpoint();
	bluetoothSetpoint();
}