#ifndef MAIN_H_
#define MAIN_H_

#include <API.h>
#include "JINX.h"

#ifdef __cplusplus
extern "C" {
	#endif

	void autonomous();

	void initializeIO();

	void initialize();
	
	#define IME_LEFT_MOTOR 0
	#define IME_RIGHT_MOTOR 1
	#define NUMBER_OF_IME 2
	
	void operatorControl();

#ifdef __cplusplus
}
#endif

#endif
