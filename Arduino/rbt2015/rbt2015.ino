#include "ArduinoCom.h"

#define LIDAR_DRIVE_IA 44 //44, 45, 46 pin pwm on mega
#define LIDAR_DRIVE_IB 45

#define LIDAR_POWER 46

/* Drive DC MOTOR
IA IB STATE
L L OFF
H L Forward
L H Backward
H H OFF

Avant PWM sur IA et IB OFF
Arrière PWM sur IB et IA OFF
*/
#define STEPPERA_DIR 12
#define STEPPERA_STEP 13
//#define STEPPERA_SLEEP
//#define STEPPERA_MS1
//#define STEPPERA_MS2

#define STEPPERB_DIR 14
#define STEPPERB_STEP 15
//#define STEPPERB_SLEEP
//#define STEPPERB_MS1
//#define STEPPERB_MS2

#define TELEMETER_US_NB 4
const unsigned char telemeters_pin[2][TELEMETER_US_NB] = {	{16, 17},
															{18,19},
															{20,21},
															{22,23}};

#define TELEMETER_IR_NB 3
const unsigned char telemeters_ir_pin[TELEMETER_IR_NB] = {4, 5, 6};

//sens 0=> Forward || 1=>Backward
void setDCDriver(bool sens, unsigned char value)
{
	if(sens)
	{
		digitalWrite(LIDAR_DRIVE_IB, LOW);
		if(value == 0) digitalWrite(LIDAR_DRIVE_IA, LOW);
		else if(value == 255) digitalWrite(LIDAR_DRIVE_IA, HIGH);
		else analogWrite(LIDAR_DRIVE_IA, value);
	}
	else
	{
		digitalWrite(LIDAR_DRIVE_IA, LOW);
		if(value == 0) digitalWrite(LIDAR_DRIVE_IB, LOW);
		else if(value == 255) digitalWrite(LIDAR_DRIVE_IB, HIGH);
		else analogWrite(LIDAR_DRIVE_IB, value);
	}
}

void setSteppersDir(bool A, bool B)
{
	digitalWrite(STEPPERA_DIR, A?LOW:HIGH);
	digitalWrite(STEPPERB_DIR, B?LOW:HIGH);
}
/*
long stepperACount;
long stepperBCount;
long startTime;
long targetStepA;
long targetStepB;
long targetSpeedA; //in us/step
long targetSpeedB; //in us/step
long currentSpeedA;
long currentSpeedB;
long incrementAAt;
long releaseAAt;
bool waitReleaseA;
long incrementBAt;
long releaseBAt;
bool waitReleaseB;

//speed in stepBySeconds
//accelerationA inStepBySecondsBySeconds
void setStepperCommand(int targetA, uint speedA, uint accelerationA,
					   int targetB, uint speedB, uint accelerationB)
{
	startTime = micros();
	speedA 
	
}
*/
void setup()
{
  //PWM vers driver moteur DC
  pinMode(LIDAR_DRIVE_IA, OUTPUT);
  pinMode(LIDAR_DRIVE_IB, OUTPUT);
  
  pinMode(LIDAR_POWER, OUTPUT);

  //Commande vers Stepper
  pinMode(STEPPERA_DIR, OUTPUT);
  pinMode(STEPPERA_STEP, OUTPUT);
  pinMode(STEPPERB_DIR, OUTPUT);
  pinMode(STEPPERB_STEP, OUTPUT);

  //Analog In vers Current Sensor
  //Analog In vers VBat
  //Analog in vers tele IR

  //Acquisition vers telemètre ultrason

  //Uart1 => debug/prog
  //Uart2 => comm Rpi

}

void loop()
{
	digitalWrite(STEPPERA_DIR, LOW);
	
	digitalWrite(STEPPERA_STEP, HIGH);
	delayMicroseconds(2); //minimum 1uS
	digitalWrite(STEPPERA_STEP, LOW);
	delay(5); //200 step by seconds 1RPM
	
	/*
	if( ()incrementAAt <= micros()) && !waitReleaseA)
	{
		digitalWrite(STEPPERA_STEP, HIGH);
		releaseAAt = micros() + 10;
		waitReleaseA = true;
		incrementAAt += currentSpeedA;
	}
	
	if( ()incrementBAt <= micros() && !waitReleaseB)
	{
		digitalWrite(STEPPERA_STEP, HIGH);
		releaseBAt = micros() + 10;
		waitReleaseB = true;
		incrementAAt += currentSpeedB;
	}
	
	*/
}

/*
2000 steps
200 step by seconds
400 stepBySecondSeconds
Each seconds add 400steps by seconds
Each milli add 0.4steps by seconds
Each milli add 400steps by milliseconds
Each micros add 0.4stemps by milliseconds
Each micros add 400steps by microsseconds

500mS to full Speed and fullStop

*/


