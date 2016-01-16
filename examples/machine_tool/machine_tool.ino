/* The Machine Tool -- Code to operate the machine tool from the
   fischertechnik comoputing kits.
   Copyright 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

/// We have a power supply with 12V
FT_Computing ft(12);

/// button to mark item is inposition
const Input& inPosition = ft.e3;
/// lower end of driller slide 
const Input& lowerEnd = ft.e4;
/// upper end of driller slide
const Input& upperEnd = ft.e5;

/// motor to drive turntable
const Motor& turnTable = ft.motor1;
/// motor to drive driller slide
const Motor& slide = ft.motor2;
/// motor/lamp to simulate drilling
const Motor& driller = ft.motor3;


void setup() {
  Serial.begin(9600); // for debugging
}

void loop() {

  Serial.println("Moving driller slide up.");

  slide.up();
  // wait for driller slide to reach upper end
  while(not upperEnd.isDepressed()) {
    // wait for button to be depressed
  }
  slide.off();

  Serial.println("Turn table on:");

  turnTable.right();
  Serial.println("- wait until current item is removed.");
  while(inPosition.isDepressed()) {
    // wait current item to be moved on for button to be released 
  }
  delay(100); // debounce inPosition button
  Serial.println("- wait for new item in position.");
  while(not inPosition.isDepressed()) {
    // wait for new item to be in position
  }
  turnTable.off();
  
  delay(1000);

  Serial.println("Driller down.");
  slide.down();
  while(not lowerEnd.isDepressed()) {
    // wait for driller slide to reach lower end
  }
  slide.off();

  // simulate the actual drilling of the item.
  driller.on();
  delay(2000);
  driller.off();


}
