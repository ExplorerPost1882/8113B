#pragma config(Motor,  port1,           frontLeft,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           backLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           backRight,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           armLeftTop,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           armRightTop,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           armLeftBottom, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           armRightBottom, tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wezerds1               !!*//

const float SENSITIVITY = 0.75; //%

void checkControls() {
	///////////////////////////////////MOVEMENT
	int forwardVelocity = vexRT[AccelY]; //WARNING!! test
	int sidewaysVelocity = vexRT[AccelX] * SENSITIVITY;
	int rightSpeed = forwardVelocity + sidewaysVelocity;
	int leftSpeed = forwardVelocity - sidewaysVelocity;
	motor[frontRight] = rightSpeed;
	motor[backRight] = rightSpeed;
	motor[frontLeft] = leftSpeed;
	motor[backLeft] = leftSpeed;
	//////////////////////////////////ARM
	if(vexRT[Btn8U] == 1) {//if up button is pressed raise the arm
		motor[armLeftTop] = 127;
		motor[armRightTop] = 127;
		motor[armLeftBottom] = 127;
		motor[armRightBottom] = 127;
	} else if(vexRT[Btn8D] == 1) { //else if down button is pressed lower the arm
		motor[armLeftTop] = -127;
		motor[armRightTop] = -127;
		motor[armLeftBottom] = -127;
		motor[armRightBottom] = -127;
	} else { //if no buttons are pressed stop the arm
		motor[armLeftTop] = 0;
		motor[armRightTop] = 0;
		motor[armLeftBottom] = 0;
		motor[armRightBottom] = 0;
	}

}

task main() {
	while(true) {
		checkControls();
	}
}
