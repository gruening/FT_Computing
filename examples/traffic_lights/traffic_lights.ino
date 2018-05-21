/* Fischertechnik Computing Code 
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

const Input& button = ft.e8; // button to request red for traffic
const Motor& green = ft.motor1;
const Motor& yellow = ft.motor2;
const Motor& red = ft.motor3;

volatile bool pedestrianRequest = false;

void raisePedestrianRequest() {
  pedestrianRequest = true;
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(button.toPin()), raisePedestrianRequest, FALLING);
  //attachInterrupt(0, raisePedestrianRequest, FALLING);
}

void loop() {
  // green for at least 3s, and waiting for button to be pressed
  green.on();
  // wait here until pedestrian button has been pressed
  while(pedestrianRequest == false) {
    delay(1);
  }
 delay(3000); // minimal green time
 green.off();
 pedestrianRequest = false; // acknowledge we have serves the pedestrian request.
  

  // yellow for 4s
  yellow.on();
  delay(4000);
  yellow.off();

  // red for 10s
  red.on();
  delay(10000);

  // red-yellow for 2s
  yellow.on();
  delay(2000);
  red.off();
  yellow.off();

  // and start over from green

}
