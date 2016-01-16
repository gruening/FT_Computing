/* Fischertechnik Computing Code 
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

const Input& topCall = ft.e6; 
const Input& middleCall = ft.e7; 
const Input& bottomCall = ft.e8; 
const Input& topFloor = ft.e3; 
const Input& middleFloor = ft.e4; 
const Input& bottomFloor = ft.e5; 

const Motor& lift = ft.motor1;
const Motor& lampDown = ft.motor2;
const Motor& lampUp = ft.motor3;

enum {NONE, BOTTOM, MIDDLE, TOP};

int currentFloor = NONE;

void setup() {

     lift.down();
     while(not bottomFloor.isDepressed());
     lift.off();
     currentFloor = BOTTOM;
}

int call();
int floor();

void loop() {

  int newFloor = NONE;

  // wait until a floor is called that is different from the current floor:
  do {
     newFloor = call(); 
  } while(newFloor == NONE or newFloor == currentFloor);

  if(newFloor > currentFloor) {
    lampUp.on();
    lift.up();
  }
  else {
    lampDown.on();
    lift.down();
  }

  while(floor() != newFloor);
  currentFloor = newFloor;
  lift.off();
  lampDown.off();
  lampUp.off();

}

  
int call() {
  if(bottomCall.isDepressed()) return BOTTOM;
  else if (middleCall.isDepressed()) return MIDDLE; 
  else if (topCall.isDepressed()) return TOP;
  else return NONE;
}
  
int floor() {
  if(bottomFloor.isDepressed()) return BOTTOM;
  else if (middleFloor.isDepressed()) return MIDDLE; 
  else if (topFloor.isDepressed()) return TOP;
  else return NONE;
}
