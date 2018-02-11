#include "main.h"



void initializeIO() {
  watchdogInit();
}

void initialize() {

	initJINX(stdout);
	delay(100);
	taskCreate(JINXRun, TASK_DEFAULT_STACK_SIZE, NULL, (TASK_PRIORITY_DEFAULT));
	delay(100);

	int IMECount = imeInitializeAll();
	if(IMECount != NUMBER_OF_IME){
        // something has gone wrong
        printf("Err");
    }
}
