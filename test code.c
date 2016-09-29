#pragma config(Sensor, dgtl1,  armStopper,     sensorTouch)
#pragma config(Motor,  port1,           frontLeft,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           backLeft,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           backRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           armRightTop,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           armRightBottom, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           armLeftTop,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           armLeftBottom, tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const float SENSITIVITY = 0.8; //%

void setArmMotors(int speed) {
	motor[armLeftTop] = speed;
	motor[armRightTop] = speed;
	motor[armLeftBottom] = speed;
	motor[armRightBottom] = speed;
}

void moveBase(int leftMotors, int rightMotors) {
	motor[frontRight] = rightMotors;
	motor[backRight] = rightMotors;
	motor[frontLeft] = leftMotors;
	motor[backLeft] = leftMotors;
}

task checkControls() {
	while(true) {
		///////////////////////////////////MOVEMENT
		int forwardVelocity = vexRT[Ch3];
		int sidewaysVelocity = vexRT[Ch4] * SENSITIVITY;
		if(abs(forwardVelocity) < 5) { //deadzone check
			forwardVelocity = 0;
		}
		if(abs(sidewaysVelocity) < 5) {
			sidewaysVelocity = 0;
		}
		moveBase(forwardVelocity + sidewaysVelocity, forwardVelocity - sidewaysVelocity); //move(left, right)
		//////////////////////////////////ARM
		if(vexRT[Btn8U] == 1) {//if up button is pressed raise the arm
			setArmMotors(127);
		} else if(vexRT[Btn8D] == 1) { //else if down button is pressed lower the arm
			setArmMotors(-70);
		} else { //if no buttons are pressed stop the arm
			setArmMotors(-15);
		}
	}
}

task main() {
	startTask(checkControls);
	while(true) {
		if(SensorValue(armStopper) == 1) {

		}
	}
}
