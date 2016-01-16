/* Code to operate the LBG RigiDuo Seilbahn with the Arduino Motor shield. 
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

// const Input& topCall = ft.e1; 
// const Input& middleCall = ft.e2; 
const Input& forwards = ft.e3; 
const Input& backwards = ft.e4; 
// const Input& middlePosition = ft.e5; 
// const Input& bottomPosition = ft.e6; 

const Motor& motor = ft.motor1;

enum {NONE, FORWARDS, BACKWARDS};

int currentDirection = NONE;



void setup() {}

int direction() {
  if(forwards.isDepressed()) return FORWARDS;
  else if (backwards.isDepressed()) return BACKWARDS; 
  else return NONE;
}

void loop() {

  int newDirection;

  do {
     newDirection = direction(); 
  } while(newDirection == currentDirection);

  switch(newDirection) {
  case NONE: 
    motor.off(); 
    break;
  case FORWARDS:
    motor.forwards();
    break;
  case BACKWARDS:
    motor.backwards();
    break;
  }

  currentDirection = newDirection;
}

  
  
  
  
