/* Code to operate the Sorting Machine from the FT Computing kit.
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(35);

const Input& start = ft.e0; 
const Input& leftEnd = ft.e3; 
const Input& middlePos = ft.e4; 
const Input& rightEnd = ft.e5; 
const Input& measurement = ft.e6; 
const Input& longBlock = ft.e7; 


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
  
  // test whether also the long button is still pressed => long block
  if(longBlock.isDepressed()) {
    slide.right();
    while (not rightEnd.isDepressed());
    slide.left();
  } 
  else { // short block

    while (not leftEnd.isDepressed());
    slide.right();
  }

  while(not middlePos.isDepressed());
  slide.off();
}
