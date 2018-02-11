#include "main.h"

void autonomous() {
 motorSet(2 , -100);
  motorSet(3 , 100);
 delay(4500);
motorStopAll();
}
