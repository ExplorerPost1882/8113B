#pragma config(Motor,  port1,           frontLeft,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           backLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           backRight,     tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void moveRobot() {
	motor[frontRight] = vexRT[Ch2];
	motor[frontLeft] = vexRT[Ch2];
	motor[backRight] = vexRT[Ch2];
	motor[backLeft] = vexRT[Ch2];
}

task main() {
	while(true) {
		moveRobot();
	}
}
