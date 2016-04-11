/* Code to operate the Sorting Machine from the FT Computing kit.
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(21);

const Input& start = ft.e6; // ok
const Input& leftEnd = ft.e3; // E3 and E5 go on
const Input& rightEnd = ft.e5; // no reaction
const Input& middlePos = ft.e4; // ok
const Input& measurement = ft.e7; // ok

const Motor& slide = ft.motor1;

void setup() {
  Serial.begin(9600);

  // move slide into middle Position:
  slide.right();
  while(middlePos.isReleased() && rightEnd.isReleased());
  slide.left();
  while(middlePos.isReleased());
  slide.off();
}

void loop() {

  // wait for start button to be depressed:
  while(not start.isDepressed());

  // move slide left
  slide.left();
  while(not measurement.isDepressed());
  
  // test whether also the start button is still pressed => long block
  if(start.isDepressed()) {
    slide.right();
    while (not rightEnd.isDepressed());
    slide.left();
  } 
  else { // short block

    while (not leftEnd.isDepressed());
    slide.right();
  }

  while(not middlePos.isDepressed());
    
}
