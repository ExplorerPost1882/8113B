/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

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

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous() {


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

const float SENSITIVITY = 0.8; //%
const float ONE_ROTATION = 627.2; //encoder

void setArmMotors(int speed) {
	motor[armLeftTop] = speed;
	motor[armRightTop] = speed;
	motor[armLeftBottom] = speed;
	motor[armRightBottom] = speed;
}

void setTwistMotors(int speed) {
	motor[leftTwist] = speed;
	motor[rightTwist] = speed;
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
		if(vexRT[Btn8D] == 1) { //if up button is pressed raise the arm until sensor is pressed
			setTwistMotors(127); //start twisting : Uses encoder to rotate 3 revolutions
			waitUntil(nMotorEncoder[rightTwist] >= 1 * ONE_ROTATION);
			setTwistMotors(0);
			setArmMotors(127); //start arm
			waitUntil(SensorValue[armStopper] == 1);
			setArmMotors(0); //stop once sensor is triggered
			wait1Msec(500);
			setTwistMotors(-127);
			waitUntil(nMotorEncoder[rightTwist] <= 0);
			setTwistMotors(0);
			setArmMotors(-20);
			wait1Msec(1000);
			setArmMotors(0); //stop twist and arm
		} else if(vexRT[Btn8R] == 1) { //else if down button is pressed lower the arm
			setArmMotors(-10);
		} else { //if no buttons are pressed stop the arm
			setArmMotors(0);
		}
	}
}

task usercontrol()
{
  // User control code here, inside the loop

  while (true) {
  	if(vexRT[Btn6U]) {
			stopAllTasks();
  }
}
