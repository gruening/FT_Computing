/* Fischertechnik Computing Code 
   Copyright 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

const Input& inPosition = ft.e3;
const Input& lowerEnd = ft.e4;
const Input& upperEnd = ft.e5;

const Motor& turnTable = ft.motor1;
const Motor& drillerSlide = ft.motor2;
const Motor& driller = ft.motor3;


void setup() {
  Serial.begin(9600); // for debugging
  // set timer 2 divisor to 32 for PWM frequency of 980.39 Hz -- seems
  // to work better than with the FT mini-motors 490 Hz (or both
  // divided by 2? as the symmetric one is used?)
  TCCR2B = TCCR2B & B11111000 | B00000101;    
}

void loop() {

  Serial.println("Moving DrillerSlide up.");

  drillerSlide.up();
  while(not upperEnd.isDepressed()) {}
  drillerSlide.off();

  Serial.println("Turn table on.\n");

  turnTable.left();
  Serial.println("Wait until current item is removed.");
  while(inPosition.isDepressed()) {}
  delay(100); // debounce
  Serial.println("wait for new item in position.");
  while(not inPosition.isDepressed()) {}
  turnTable.off();
  

  delay(1000);


  Serial.println("Driller down");
  drillerSlide.down();
  while(not lowerEnd.isDepressed()) {}
  drillerSlide.off();

  driller.on();
  delay(2000);
  driller.off();


}
