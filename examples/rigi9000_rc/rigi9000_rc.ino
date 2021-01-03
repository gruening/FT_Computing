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

//FT_Computing ft(12);

const Sensor leftDistance(A0); //! distance sensor for left gondola
const Sensor rightDistance(A1); //! distance sensor for right gondola
const Motor motor(11,13);; //! moter for cable car
const Input rc_event(5, HIGH); //! any button on remote depressed
const Input rc_automatic(9, HIGH); //! D0 depressed: toggle automatic operation
const Input rc_stop(8, HIGH); //! D1 depressed: stop gondolas ("D")
const Input rc_left_down(7, HIGH); //! D2 left gondola down ("A")
const Input rc_right_down(6, HIGH); //! D3 right gondola down ("C")
  

/** 
 * Returns the reading of a Sharp Distance sensor such as [model] in
 * approximate cm. 
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

const int closeToBottom = 20;


void manual(void);

void loop() {
  manual();
  //automatic_operation();
}

enum STATE {STOP, UNCHANGED, LEFT_DOWN, RIGHT_DOWN};

/**
@returns the reading from the remote control
 */
enum STATE get_rc(void) {
  
  if (rc_stop.isActive()) {
    Serial.write("STOP\r\n");
    return STOP;
  }
  else if (rc_left_down.isActive()) {
    Serial.write("LEFT DOWN\r\n");
    return LEFT_DOWN;
  }
  else if (rc_right_down.isActive()) {
    Serial.write("RIGHT DOWN\r\n");
    return RIGHT_DOWN;
  }
  //  else if  (rc_auto.isActive()) {
  //  return AUTO;
  // }
  else return UNCHANGED;
}

/**
Takes a state and depending on the readings from the distance sensor
mutates the state to stop if we are already too close to the distance sensor
@param a state
@returns the state or state stop if we are already to close too the sensor.
 */
enum STATE check_distance(const enum STATE state) {

  //return state; // for debugging

  const int left_distance = distance(leftDistance);
  const int right_distance = distance(rightDistance);

  Serial.write("Left Distance:\t");
  Serial.print(left_distance);
  Serial.write("\r\nRight Distance:\t");
  Serial.print(right_distance);
  Serial.write("\r\n");
    
  if ((state == LEFT_DOWN) && left_distance < closeToBottom) {
    Serial.write("CLOSE LEFT.\r\n");
    return STOP;
  }
  else if ((state == RIGHT_DOWN) && right_distance < closeToBottom) {
    Serial.write("CLOSE RIGHT.\r\n");
    return STOP;
  }
  else return state;
}

/**
Takes a state and switches on the motor accordingly. 
*/

void act(const enum STATE state) {

  switch(state) {
  case LEFT_DOWN:
    Serial.write("MOTOR LEFT DOWN\r\n");
    motor.left(255/5);
    break;
  case RIGHT_DOWN:
    Serial.write("MOTOR RIGHT DOWN\r\n");
    motor.right(255/5);
    break;
  default:
  case STOP:
    Serial.write("MOTOR STOP\r\n");
    motor.off();
  }
}

enum STATE current = STOP;

void manual() {

  while(true) {
    const enum STATE rc_state = get_rc();

    // if a button on the RC was pressed:
    if ( (rc_state != UNCHANGED) && (rc_state != current)) {
      Serial.write("DETECTED BUTTON PRESS\r\n");
      current = check_distance(rc_state);
      act(current);
    }
    // if no button was pressed (or the a button that does not change
    // the current state), just check the distances
    else if ((current != STOP) && (check_distance(current) == STOP)) {
      current = STOP;
      act(current);
    }

    //if (rc_state == AUTO)  re turn;
  }
}

void automatic() {

  while(true) {

    //  switch(current)
    
  }
    
}

#if 0

void old() {  

  // accelerate right gondola downwards
  for(int dutyCycle = minDutyCycle; dutyCycle < maxDutyCycle; dutyCycle++) {
    motor.right(dutyCycle);
    delay(100);
  }  

  // wait for right gondola to get close to bottom station
  do {
    dist = distance(rightSensor);
    Serial.println(dist); 
  } while (dist > closeToBottom);

  // decelerate
  for(int dutyCycle = maxDutyCycle; dutyCycle > minDutyCycle; dutyCycle--) {
    motor.right(dutyCycle);
    delay(100);
  }  

  // stop
  motor.off();


     // wait for passenger to (dis)embark
  //delay(10000);
  // or do it manualy:
  while(startButton.isReleased());
b
n n
  for(int dutyCycle = minDutyCycle; dutyCycle < maxDutyCycle; dutyCycle++) {
    motor.left(dutyCycle);
    delay(100);
  }  
cms     // wait for left gondomdndmnmnmdla to   get close to bottom station
  do {
    dist = distance(leftSensor); 
    Serial.println(dist);
  } while (dist  > closeToBottom);

  // decelarate
  for(int dutyCycle = maxDutyCycle; dutyCycle > minDutyCycle; dutyCycle--) {
    motor.left(dutyCycle);
    delay(100);nfcbnd
  }  

  // stop
  motor.off();

  //delay(10000);
  while(startButton.isReleased());

}

#endif
