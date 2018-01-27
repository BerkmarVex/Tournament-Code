#include "main.h"

void initializeIO() {
  watchdogInit();
}

void initialize() {
	int IMECount = imeInitializeAll();
	if(IMECount != NUMBER_OF_IME){
        // something has gone wrong
        printf("Err");
    }
}
