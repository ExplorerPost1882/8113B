#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl2,  leftEncoder,    sensorRotation)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightTop,      tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port2,           rightBot,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           leftTop,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftBot,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           leftBase,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightBase,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           armLock,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          sidewaysBase,  tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton() {
  bStopTasksBetweenModes = true;
}

const float SENSITIVITY = 0.65; //%
float WRAPS = 3.0;

void setTwistMotors(int amount) { // positive = launch
	motor[rightTop] = amount;
	motor[leftTop] = amount;
	motor[rightBot] = amount;
	motor[leftBot] = amount;
}

void moveBase(int leftMotor, int rightMotor) {
	motor[rightBase] = rightMotor;
	motor[leftBase] = leftMotor;
}

void unlock() {
	motor[armLock] = 127;
	wait1Msec(500);
	motor[armLock] = 50;
}

void lock() {
	motor[armLock] = -127;
	wait1Msec(100);
	motor[armLock] = 0;
}

void launch() {
	unlock();
	wait1Msec(300);
	setTwistMotors(0);
	wait1Msec(100);
	while(nMotorEncoder[rightTop] > 280) {
		setTwistMotors(-127);
	}
	setTwistMotors(0);
	wait1Msec(100);
	lock();
}

void move(int speed, int time) {
	moveBase(speed, speed * 0.97);
	wait1Msec(time);
	moveBase(0,0);
}

task overrideLock() {
	while(true) {
		if(vexRT[Btn8UXmtr2]) {
			unlock();
			while(vexRT[Btn8UXmtr2]) {
				wait1Msec(100);
			}
			lock();
		}
	}
}
//********************************************************auto**********************************************************
//dist in cm
void autoMoveStraight(int dist) {
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;  // (534 arbitary number for ticks per rotation) / (circumference of 32 cm) = (16.7 ticks per cm)
	//motor[rightBase] = 20;
	//motor[leftBase] = 20;
	float re = abs(SensorValue[rightEncoder] / 2);
	float le = SensorValue[leftEncoder];
	int rm = 70;
	int lm = 70;
	while(re / 8.35 < dist - 1 || le / 8.35 < dist - 1) {
		//float delta = abs((re - le) / 20.0);
		//motor[rightBase] = 70 - delta;
		//motor[leftBase] = 70 + delta;
		if(le < re){
			lm += (re - le) / 15;
		} else if(re < lm) {
			rm += (le - re) / 15;
		}
		motor[rightBase] = rm;
		motor[leftBase] = lm;
		re = abs(SensorValue[rightEncoder] / 2);
		le = SensorValue[leftEncoder];
	}
	motor[rightBase] = 0;
	motor[leftBase] = 0;
}

/**
multiplier base is 90 degrees (1 == turn 90)
+ multiplier is clockwise (right)
- multiplier is counter-clockwise (left)
**/
void autoTurn(int multiplier) {
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;
	if(multiplier == 0) return;
	if(multiplier > 0) {  // right
		while(SensorValue[rightEncoder] > -108 * multiplier && SensorValue[leftEncoder] < 108 * multiplier) {
			if(abs(SensorValue[rightEncoder] + 5) < SensorValue[leftEncoder]) { //tolerance of 5 ticks
				motor[rightBase] = 80;
				motor[leftBase] = 70;
			} else if(abs(SensorValue[rightEncoder]) < SensorValue[leftEncoder] + 5) {
				motor[rightBase] = 70;
				motor[leftBase] = 80;
			} else {
				motor[rightBase] = 80;
				motor[leftBase] = 80;
			}
		}
	} else {
		while(SensorValue[rightEncoder] > 108 * multiplier && SensorValue[leftEncoder] < -108 * multiplier) {
			if(SensorValue[rightEncoder] + 5 < abs(SensorValue[leftEncoder])) { //tolerance of 5 ticks
				motor[rightBase] = 80;
				motor[leftBase] = 70;
			} else if(SensorValue[rightEncoder] < abs(SensorValue[leftEncoder] + 5)) {
				motor[rightBase] = 70;
				motor[leftBase] = 80;
			} else {
				motor[rightBase] = 80;
				motor[leftBase] = 80;
			}
		}
	}
}

task autonomous() {
	autoMoveStraight(200);
	//autoTurn(1);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task arm() {
	nMotorEncoder[rightTop] = 0;
	while(true) {
		if(vexRT[Btn6UXmtr2] == 1) {
			setTwistMotors(40);
			nMotorEncoder[rightTop] = 0;
		} else if(vexRT[Btn5UXmtr2] == 1) {
			setTwistMotors(-40);
			nMotorEncoder[rightTop] = 0;
		} else {
			setTwistMotors(0);
		}
		if(vexRT[Btn8DXmtr2] == 1) { //prepare launch---------------------------------------right bottom button
			while(vexRT[Btn8DXmtr2] == 1) {//actual launch------------------------------------release right bottom button
				while(nMotorEncoder[rightTop] < 2.5 * 360) {
					setTwistMotors(127); //pre-twist : Once it hits 1 rotations around, keep it there
				}
				setTwistMotors(0);
			}
			while(nMotorEncoder[rightTop] < WRAPS * 360 && vexRT[Btn8D] == 0) {
					setTwistMotors(127); //start twisting : Once it hits WRAPS rotations around, launch the arm
			}
			launch();
		}
	}
	//int armMotion = vexRT[Ch2Xmtr2];
	//if(armMotion < 30) { //deadzone check for arm lift
	//	armMotion = 0;
	//} else if(armMotion > -30) {
	//	armMotion = 0;
	//}
	//setTwistMotors(armMotion);
}

bool pressed = false;

task usercontrol() {
	writeDebugStreamLine("started w bat @, %3d", nAvgBatteryLevel);
	startTask(arm);
	startTask(overrideLock);
	while(true) {
		if((vexRT[Btn6D] == 1 && vexRT[Btn5U] == 1) || (vexRT[Btn6DXmtr2] == 1 && vexRT[Btn5DXmtr2] == 1)) {
			stopTask(arm);
			wait1Msec(200);
			startTask(arm);
		}
		if(vexRT[Btn7UXmtr2] == 1 && !pressed) {
			WRAPS += 0.1;
			pressed = true;
			writeDebugStreamLine("changed to %d", WRAPS);
		} else if(vexRT[Btn7DXmtr2] == 1 && !pressed) {
			WRAPS -= 0.1;
			pressed = true;
			writeDebugStreamLine("changed to %d", WRAPS);
		} else if(vexRT[Btn7DXmtr2] == 0 && vexRT[Btn7UXmtr2] == 0) {
			pressed = false;
		}
		int forwardVelocity = vexRT[Ch3] * SENSITIVITY; //-----------------------------------left joystick up + down
		int turnVelocity = vexRT[Ch1] * SENSITIVITY; //--------------------------------------right joystick left + right

		if(abs(forwardVelocity) < 20) { //deadzone check
			forwardVelocity = 0;
		}
		if(abs(turnVelocity) < 10) {
			turnVelocity = 0;
		}

		moveBase(forwardVelocity + turnVelocity, forwardVelocity - turnVelocity); //move(left, right)

		if(vexRT[Btn5U] == 1) { //slide left-------------------------------------------------left top trigger
			motor[sidewaysBase] = -127;
		} else if(vexRT[Btn6U] == 1) {//slide right------------------------------------------right top trigger
			motor[sidewaysBase] = 127;
		} else {
			motor[sidewaysBase] = 0;
		}
	}
}
