/* Code to operate the Sorting Machine from the FT Computing kit.
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

const Sensor& desiredPos = ft.ex;
const Sensor& actualPos = ft.ey
const Motor& arial = ft.motor1;

void setup() {
  Serial.begin(9600);
}




void loop() {
  
  const int desired = desiredPos.getReading();
  const int actual = actualPos.getReading();

  const int squareDiff = (actual-desired)(actual-desired);
  const enum (LEFT, RIGHT) direction = (actual-desired) < 0 ? LEFT : RIGHT;
  
  int speed;
							  
  if(squareDiff == 0) speed = 0;
  else if (squareDiff < 25) speed = 8;
  else if (squareDiff < 100) speed = 16;
  else if (squareDiff < 400) speed = 32;
  else if (squareDiff < 900) speed = 48;
  else speed = maxDuty;

  switch(direction) {
  case LEFT: { arial.left(speed);  break; }
  case RIGHT: {arial.right(speed); break; }
  }
}
