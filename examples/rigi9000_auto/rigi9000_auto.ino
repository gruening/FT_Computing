/* Code to operate the LBG RigiDuo Seilbahn with the Arduino Motor shield. 
   Copyright 2015, 2016 Andre Gruening
   GNU Licence v3 or later
*/

/** Distance sensor:
    - analog reg values around 50-60 mean infinite
    - anaglo read vaues at 600 mean stop,
    - suggestion: between f = 600 and 100 make it slower with a factor of 1/f
*/


#include "FT_Computing.h"

FT_Computing ft(12);

const Input& forwards = ft.e3; 
const Input& backwards = ft.e4; 
const Sensor& leftSensor = ft.ex;
const Sensor& rightSensor = ft.ey;


const Motor& motor = ft.motor2;
const Input& startButton = ft.e0;

/** 
 * Returns the reading of a Sharp Distance sensor such as [model] in approximate cm.
 * 
 * @param sensor Sensor to read
 * 
 * @return sensor reading in cm
 */
double distance(Sensor sensor) {

  const double farRawLimit = 100.0;
  const double closeRawLimit = 600.0;

  const double closeProxy = 1.0 / closeRawLimit;
  const double farProxy = 1.0 / farRawLimit;

  const double maxDist = 70.0; // [cm]
  const double minDist = 10.0; // [cm]

  const double distProxy = 1.0 / sensor.getReading();

  if(distProxy < closeProxy) return minDist;
  if(distProxy > farProxy) return maxDist;

  return minDist + maxDist * (distProxy - closeProxy) / (farProxy - closeProxy);
}

void setup() {
  Serial.begin(9600);
}


const int maxDutyCycle = 56;
const int minDutyCycle = 48;

void loop() {

  double dist;
  // wait for right gondola to arrive


  for(int dutyCycle = minDutyCycle; dutyCycle < maxDutyCycle; dutyCycle++) {
    motor.right(dutyCycle);
    delay(100);
  }  

  do {
    dist = distance(rightSensor);
    Serial.println(dist); 
  } while (dist > 20.0);


  for(int dutyCycle = maxDutyCycle; dutyCycle > minDutyCycle; dutyCycle--) {
    motor.right(dutyCycle);
    delay(100);
  }  


  motor.off();


  // wait for passenger to (dis)embark
  //delay(10000);
  while(startButton.isReleased());

  // wait for left gondola to arrive


  for(int dutyCycle = minDutyCycle; dutyCycle < maxDutyCycle; dutyCycle++) {
    motor.left(dutyCycle);
    delay(100);
  }  

  do {
    dist = distance(leftSensor); 
    Serial.println(dist);
  } while (dist  > 20.0);


  for(int dutyCycle = maxDutyCycle; dutyCycle > minDutyCycle; dutyCycle--) {
    motor.left(dutyCycle);
    delay(100);
  }  

  motor.off();

  //delay(10000);
  while(startButton.isReleased());

}
