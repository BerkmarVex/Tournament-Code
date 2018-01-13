#include "main.h"
#include <math.h>

int analogRead ( unsigned char  channel );
void motorSet(unsigned char channel, int speed);
bool joystickGetDigital (unsigned char joystick, unsigned char buttonGroup, unsigned char button);
int joystickGetAnalog ( unsigned char joystick, unsigned char axis);

bool leftShoulderButtonUp;
bool leftShoulderButtonDown;
bool rightShoulderButtonUp;
bool rightShoulderButtonDown;
int rightlever;
int leftlever;
int count;
int counts;
int asd;
int dsa;
int line;
bool hasCone = false;
void shoulderbutton(bool check);

struct pid {
	int aValue;
	int dValue;
	int pidError;
	int priError;
	double pidKp;
	double pidKi;
	double pidKd;
	int	bias;
	int	iTime;
	int	integral;
	int	derivative;
	double pid;
	int output;
};

struct pid glift;

void glpid() {
	glift.pidKp = 0.2;
	glift.pidKi = 0.0;
	glift.pidKd = 0.4;
	glift.iTime = 500;
	glift.bias = 0;
	glift.aValue = analogRead(1);

	glift.pidError = glift.dValue - glift.aValue;
	glift.integral = glift.integral + (glift.pidError);
	glift.derivative = (glift.pidError - glift.priError);
	glift.pid = (glift.pidKp) * glift.pidError + (glift.pidKi) * glift.integral + (glift.pidKd) * glift.derivative + glift.bias;
	glift.output = (int) glift.pid;
	glift.priError = glift.pidError;
};

void update() {
	leftShoulderButtonUp = joystickGetDigital(1, 6, JOY_UP);
	leftShoulderButtonDown = joystickGetDigital(1, 6, JOY_DOWN);
	rightShoulderButtonUp = joystickGetDigital(1, 5, JOY_UP);
	rightShoulderButtonDown = joystickGetDigital(1, 5, JOY_DOWN);
	rightlever = joystickGetAnalog(1,3);
	leftlever = joystickGetAnalog(1,2);
	line = analogRead(2);
};

void shoulderbutton(bool check) {
	update();
	if(leftShoulderButtonUp || leftShoulderButtonDown){

		if(leftShoulderButtonUp && leftShoulderButtonDown){
			glift.dValue = 2150;
			motorSet(9, glift.output);
			return;
		}

		else if(leftShoulderButtonUp){
			glift.dValue = 3100;
			motorSet(9, glift.output);
			return;
		}

		else if(leftShoulderButtonDown){
			glift.dValue = 1333;
			motorSet(9, glift.output);
			return;
		}
	}
	if(rightShoulderButtonUp || rightShoulderButtonDown){
		if(rightShoulderButtonUp){
			motorSet(9, 100);
			return;
		}
		motorSet(9, -100);
		return;
	}
	if(!check){
	motorSet(9, 0);
	return;
	}
};


void linereader(){
	update();

	if(line <= 2850){
		hasCone = true;
		delay(40);
		glift.dValue = 3100;
		motorSet(9, glift.output);
		shoulderbutton(hasCone);
		return;
	}

	hasCone = false;
	shoulderbutton(hasCone);
	return;
};



void drive(){
	update();
	if ((rightlever >=90 && rightlever <= 120) || (rightlever>=-90 && rightlever<=-120)) {
      // divide number between 90 and 120, -90 and -120, to make the controls more precise
    if (rightlever%2 == 1 || rightlever%2 == -1) { //make power variable even so it can be divided by 2
      rightlever++;
    }
    rightlever = rightlever/2;
  }
  if ((leftlever >=-90 && leftlever <= -120) || (leftlever >=90 && leftlever <= 120)) {
    	// divide number between 90 and 120, -90 and -120, to make the controls more precise
    if (leftlever%2 == 1 || leftlever%2 == -1){ // make power2 variable even so it can be divided by 2
        leftlever++;
    }
  	leftlever = leftlever/2;
  }
	motorSet(2, rightlever);
  motorSet(3, leftlever);
};

void operatorControl() {
	analogCalibrate(2);
	imeReset(IME_RIGHT_MOTOR);
	imeReset(IME_LEFT_MOTOR);
	while (1) {
		imeGet(0, &count);
		imeGet(1, &counts);
		linereader();
		glpid();
		drive();
		//printf("%d\n", hasCone);
		//printf("Left(%d), Right(%d)\n", count, counts);
		//printf("out(%d),aVal(%d),err(%d),want(%d),Kd(%d)\n", glift.output, glift.aValue, glift.pidError,glift.dValue, glift.derivative);
		//printf("%d\n", analogRead(1));
	  //printf("%d\n", line);
		delay(20);
	}
};
