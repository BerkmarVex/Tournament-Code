/************
*  imports  *
*************/

#include "main.h"
#include <pid.h>

/*************************
*  local initialization  *
**************************/

//JINX
char jinxSend[20];

//Lift variables

bool leftShoulderButtonUp;
bool leftShoulderButtonDown;
bool rightShoulderButtonUp;
bool rightShoulderButtonDown;

//Sensor variables
int count;
int counts;
int line;
bool hasCone = false;
//Top,Mid,Bottom
const int GLPIDPOS[] = {630,1530,2420};
//Cortex Ports

const int LEFTMOTOR = 3;
const int RIGHTMOTOR = 2;
const int GLMOTOR = 9;
const int GLIGHT = 2;
const int POT = 1;

//Controller variables

int leftJoyV;
int leftJoyH;
int rightJoy;
int rightJoyH;
bool rightR = false;
bool rightRcheck;
bool stillPressed = false;
bool prevRightRcheck = false;
bool leftL = false;
bool leftLcheck;
bool prevLeftLcheck = false;
bool stillPressedL = false;
int reverse;

//Function definition

void dualzone();
void checkPressed();
void buttonStatus();
void update();
void tank();
void arcade();
void update();
void drive();
void glpid();
void shoulderbutton(bool check);
void linereader();
void checkPressedL();
void buttonStatusL();



void update() {
 /***********************************************************************
	*  this function just updates the varables that get used every cylce  *
	***********************************************************************/

	//Button Group
	leftShoulderButtonUp = joystickGetDigital(1, 6, JOY_UP);
	leftShoulderButtonDown = joystickGetDigital(1, 6, JOY_DOWN);
	rightShoulderButtonUp = joystickGetDigital(1, 5, JOY_UP);
	rightShoulderButtonDown = joystickGetDigital(1, 5, JOY_DOWN);

	//Joystick
	leftJoyV = joystickGetAnalog(1, 3);
	leftJoyH = joystickGetAnalog(1, 4);
	rightJoy = joystickGetAnalog(1, 2);
	rightJoyH = joystickGetAnalog(1, 1);
	rightRcheck = joystickGetDigital(1, 8, JOY_RIGHT);
	leftLcheck = joystickGetDigital(1, 7, JOY_LEFT);

	//Functions
	dualzone();
	checkPressed();
	checkPressedL();
	glLift.sensor = analogRead(POT);

	//Linereader
	line = analogRead(GLIGHT);
};

void shoulderbutton(bool check) {
	update();
	if(leftShoulderButtonUp || leftShoulderButtonDown) {
		if(leftShoulderButtonUp && leftShoulderButtonDown) {
			glLift.target = GLPIDPOS[1];
			motorSet(GLMOTOR, glLift.output);
			motorSet(GLMOTOR, glLift.output);
			return;
		}

		else if(leftShoulderButtonUp) {
			glLift.target = GLPIDPOS[0];
			motorSet(GLMOTOR ,glLift.output);
			return;
		}

		else if(leftShoulderButtonDown) {
			glLift.target = GLPIDPOS[2];
			motorSet(GLMOTOR, glLift.output);
			return;
		}
	}
	if(rightShoulderButtonUp || rightShoulderButtonDown) {
		if(rightShoulderButtonUp) {
			motorSet(GLMOTOR, 100);
			return;
		}
		else {
			motorSet(GLMOTOR, -100);
			return;
		}
	}
	if(!check) {
		motorSet(GLMOTOR, 0);
		return;
	}
};

void buttonStatus() {
 /*************************************************************
	*  This function is used to ensure that the varable rightR  *
	*  does not get changed if the user holds down the switch   *
	*  button for more than 20 milliseconds                     *
 	*************************************************************/
	if(rightRcheck == true) {
		if(rightRcheck == !prevRightRcheck) {
			stillPressed = false;
		}
		else{
			stillPressed = true;
		}
	}
	else if(rightRcheck == false) {
		stillPressed = false;
	}
}

void buttonStatusL() {
 /*************************************************************
	*  This function is used to ensure that the varable rightR  *
	*  does not get changed if the user holds down the switch   *
	*  button for more than 20 milliseconds                     *
 	*************************************************************/
	if(leftLcheck == true) {
		if(leftLcheck == !prevLeftLcheck) {
			stillPressedL = false;
		}
		else{
			stillPressedL = true;
		}
	}
	else if(leftLcheck == false) {
		stillPressedL = false;
	}
}
void checkPressed() {
 /*************************************************************
	*  this function switches tank and arcade controls          *
	*  this if statments take in 3 booleans                     *
	*  if the button was pressed for an extended amount of time *
	*	 if the button was pressed                                *
	*  and the current drive style                              *
	*************************************************************/
	buttonStatus();
	if (!stillPressed && rightRcheck && rightR) {
		rightR = false;
	}
	else if (!stillPressed && rightRcheck && !rightR) {
		rightR = true;
	}
}
void checkPressedL() {
 /*************************************************************
	*  this function switches tank and arcade controls          *
	*  this if statments take in 3 booleans                     *
	*  if the button was pressed for an extended amount of time *
	*	 if the button was pressed                                *
	*  and the current drive style                              *
	*************************************************************/
	buttonStatusL();
	if (!stillPressedL && rightRcheck && rightR) {
		leftL = false;
	}
	else if (!stillPressedL && rightRcheck && !rightR) {
		leftL = true;
	}
}

void ReverseChange(){
	if (leftL){
		reverse = -1;
	}
		else if (!leftL){
			reverse = 1;
		}
}

void dualzone() {
 /*********************************************************
  *  This function zones the controller to have a slower  *
	*  inner ring to allow for more precise controll and    *
	*	 a faster outer ring for faster movment               *
  *********************************************************/
	if((rightJoy >=90 && rightJoy <= 120) || (rightJoy>=-90 && rightJoy<=-120)) {
    rightJoy = rightJoy/2;
  }

  else if((leftJoyV >=-90 && leftJoyV <= -120) || (leftJoyV >=90 && leftJoyV <= 120)) {
		leftJoyV = leftJoyV/2;
  }

	else if((leftJoyH >=-90 && leftJoyH <= -120) || (leftJoyH >=90 && leftJoyH <= 120)) {
	  	leftJoyH = leftJoyH/2;
	}

	else if((rightJoyH >=-90 && rightJoyH <= -120) || (rightJoyH >=90 && rightJoyH <= 120)) {
			rightJoyH = rightJoyH/2;
  }
}

void tank() {
 /***********************************************************
	  This function splits the controls for the               *
		left and right motors to the left and right joy sticks  *
	***********************************************************/
	update();
	motorSet(RIGHTMOTOR, reverse * -rightJoy);
	motorSet(LEFTMOTOR, reverse * leftJoyV);

}

void arcade() {
 /******************************************
	*  This functions combines left and      *
	*  right motor controls to the vertical  *
  *  left and horizontal right joysticks   *
	******************************************/
	update();
	motorSet(RIGHTMOTOR, reverse * (-leftJoyV + rightJoyH));
	motorSet(LEFTMOTOR, reverse * (leftJoyV + rightJoyH));
}

void lift(){
	motorSet(4, 50);
	motorSet(5, 50);
}


void linereader() {
	update();
	if(line <= 2850) {
		hasCone = true;
		delay(40);
		glLift.target = GLPIDPOS[0];
		motorSet(GLMOTOR, glLift.output);
		shoulderbutton(hasCone);
		return;
	}

	hasCone = false;
	shoulderbutton(hasCone);
	return;
};

void drive() {
	if (!rightR) {
		tank();
	}
	else if(rightR) {
		arcade();
	}
}

void jinxMSGSend(){
	sprintf(jinxSend, "%f", glLift.integral);
	writeJINXData("integral", jinxSend);

	sprintf(jinxSend, "%f", glLift.derivative);
	writeJINXData("derivative", jinxSend);

	sprintf(jinxSend, "%d", glLift.error);
	writeJINXData("Error", jinxSend);

	sprintf(jinxSend, "%d", glLift.output);
	writeJINXData("output", jinxSend);

	sprintf(jinxSend, "%d", glLift.sensor);
	writeJINXData("sensor", jinxSend);
	writeJINXSerial(jinxSend);


}

void operatorControl() {
	/******************
	*  main function  *
	*******************/

	analogCalibrate(2);
	imeReset(IME_RIGHT_MOTOR);
	imeReset(IME_LEFT_MOTOR);

	while (1) {
		imeGet(0, &count);
		imeGet(1, &counts);
		linereader();
		pidDo(&glLift);
		drive();
		//jinxMSGSend();
		//printf("out(%d), Sen(%d), Kd(%d), err(%d)\n", glLift.output, glLift.sensor, (int)glLift.derivative, glLift.error);
		delay(20);
	}
};
