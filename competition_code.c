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

//*******************************************PreAuton************************************************************
void pre_auton() {
  bStopTasksBetweenModes = true;
}
//*******************************************Main****************************************************************

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
	wait1Msec(700);
	lock();
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

void basicMove(int speed, int time) {
	moveBase(speed, speed);
	wait1Msec(time);
	moveBase(0,0);
}

void autoLaunch() {
	while(nMotorEncoder[rightTop] < WRAPS * 360 && vexRT[Btn8D] == 0) {
					setTwistMotors(127); //start twisting : Once it hits WRAPS rotations around, launch the arm
			}
	launch();
}

//dist in foam blocks
void autoMoveStraight(float dist) {
	int neg = 1;
	if(dist < 0) {
		neg = -1;
	}
	dist = abs(dist);
	dist *= 60.96;
	wait1Msec(200);
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;  // (534 arbitary number for ticks per rotation) / (circumference of 32 cm) = (16.7 ticks per cm)
	float re = abs(SensorValue[rightEncoder] / 2.0);
	float le = SensorValue[leftEncoder];
	float base = 30;
	float totalComp = 0;
	wait1Msec(50);
	int totalloops = 0;
	float learning = 0;
	while(re / 8.15 < dist - (base / 18.0) || le / 8.15 < dist - (base / 18.0)) {
		if(totalloops <= 200) {
			base = totalloops / 4 + 20;
		}
		if(totalloops <= 200) {
			learning = totalloops / 400;
		}
		float delta = (re - le) * learning;
		totalComp += delta;
		motor[rightBase] = neg * (base - delta - totalComp / 500.0);
		motor[leftBase] = neg * (base + delta + totalComp / 500.0);
		re = abs(SensorValue[rightEncoder] / 2.0);
		le = SensorValue[leftEncoder];
		totalloops += 1;
		wait1Msec(5);
	}
	motor[rightBase] = 0;
	motor[leftBase] = 0;
}

void autoTurn(int degrees) {
	wait1Msec(200);
	int neg = 1;
	if(degrees < 0) {
		neg = -1;
	}
	degrees = abs(degrees);
	float dist = degrees * 0.3456; // 31.1 / 90
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;  // (534 arbitary number for ticks per rotation) / (circumference of 32 cm) = (16.7 ticks per cm)
	float re = abs(SensorValue[rightEncoder] / 2.0);
	float le = SensorValue[leftEncoder];
	float base = 70;
	float totalComp = 0;
	while(re / 8.15 < dist - (base / 30.0) || le / 8.15 < dist - (base / 30.0)) {
		float delta = (re - le) * 0.6;
		totalComp += delta;
		motor[rightBase] = -neg * (base - delta - totalComp / 300.0);
		motor[leftBase] = neg * (base + delta + totalComp / 300.0);
		re = abs(SensorValue[rightEncoder] / 2.0);
		le = SensorValue[leftEncoder];
		wait1Msec(5);
	}
	motor[rightBase] = 0;
	motor[leftBase] = 0;
}

//*****************autonomous-code***********************

task autonomous() {
	autoMoveStraight(-1);		// back from star to drop arm
	autoMoveStraight(0.48);	// forward to star
	autoLaunch(); 					// launch preload on robot
	if(false) { // false: [for competition 15 sec autonomous]		 true: [for programming skills autonomous]
		wait1Msec(500);
		autoLaunch(); 					// launch preload 1
		wait1Msec(500);
		autoLaunch(); 					// launch preload 2
		wait1Msec(500);
		autoLaunch(); 					// launch preload 3
		autoTurn(180); 					// turn around toward fence
		basicMove(-127, 1200); 	// straighten self after launch by backing against wall
		wait1Msec(500);
		autoMoveStraight(1.2);	// move forward to midline
	} else {
		autoMoveStraight(-0.8);
		return;
	}
	autoTurn(90); 						// turn toward cube
	autoMoveStraight(1.5);		// move to move up cube
	autoMoveStraight(-0.5);
	autoTurn(90); 						// turn away from cube toward outside
	//autoMoveStraight(-0.2); // no back up a bit toward fence
	//autoLaunch(); 					// no launch cube
	autoTurn(90); 						// turn right toward star line
	//autoMoveStraight(1.5);	// no move toward corner
	//autoTurn(135); 					// no rotate rest of way to face toward fence
	//basicMove(-127, 1200);	// no straighten self after launch by backing against wall
	//wait1Msec(500);
	//autoMoveStraight(1.5);	// no get in position to turn toward line of stars
	//autoTurn(90);						// no turn toward line of stars
	autoMoveStraight(1.5);
	autoLaunch();							// launch it
	autoMoveStraight(-0.5);
	autoTurn(-90);						// turn to line up for next one
	for(int i = 0; i < 3; i++) {
		autoMoveStraight(0.5);	// move toward next star
		autoTurn(90);						// turn toward it
		autoMoveStraight(0.5);	// move a it to get under it
		autoLaunch();						// launch it
		autoMoveStraight(-0.5); // back up to turn
		autoTurn(-90);					// turn to line up for next one
	}
}

//**********************************************driver*******************************************************************

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
