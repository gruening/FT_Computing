/** 
    Code to operate the Teach-In Robot from the FT Computing kit.
    Copyright 2018 Andre Gruening
    GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

/**
   Interface serial port with the stdlib FILE io ioperations
   From http://forum.arduino.cc/index.php/topic,120440.0.html
   @param ch char to write
   @param f file structure to use for writing.
*/
int serial_putchar(char ch, FILE* f) {
  // expand nl to cr nl
  if (ch == '\n') serial_putchar('\r',f);
  // write char to 
  return Serial.write(ch) == 1? 0 : 1;
}

/// hold file data for serial output
FILE serial;

/// motor to rotate tower
const Motor& rotate = ft.motor1;
/// angle of tower
const Sensor& angle = ft.ex;

/// motor to lift arm
const Motor& arm = ft.motor2;
/// indicate position of arm
const Sensor& height= ft.ey;

/// pwm driver for the electromagnet
const Motor& magnet = ft.motor3;

/// input buttons
const Input& left = ft.e3; /// turn left
const Input& right = ft.e4; /// turn right
const Input& up = ft.e5; /// arm up
const Input& down = ft.e6; /// arm down
const Input& toggle_magnet = ft.e7; /// toggle magnet
const Input& teach_in = ft.e8; /// teach in a position
const Input& run = ft.e2; /// exit teaching and replay the tought in sequency.
/// @todo This is not accessible -- need to change for final example.

//! tolarance for position of ex and ey readings
#define TOLERANCE 3

/**
   rotate arm to target position
   @param target position reading to rotate to.
 */
void rotate_to(const int target) {

  rotate.left();
  while(angle.getReading() > target + TOLERANCE);
  rotate.right();
  while(angle.getReading() < target - TOLERANCE);
  rotate.off();
}

/**
   move arm to given position
   @param pos position reading to move arm to.
 */
void move_arm(const int pos) {

  arm.down();
  while(height.getReading() > pos + TOLERANCE);
  arm.up();
  while(height.getReading() < pos - TOLERANCE);
  arm.off();
}

/** 
 switch off magnet such that held objects is demagnetised by reversing
 current through magent for a few times.  

 @pre assume magnet is always switch on with magnet.on() (ie
 magnet.right()) 
 @param magnet pwm output the magnet is connected to.
*/
void demagnetise_off(const Motor& magnet) {

  magnet.left();
  delay(10);
  magnet.right();
  delay(2);
  magnet.left();
  delay(1);
  magnet.off();
}


void setup() {

  // Set up stdout and stderr
  Serial.begin(115200);
  fdev_setup_stream(&serial, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial;
  stderr = &serial;
}

/**
   data structure to describe position of robot:
   angle of tower, hight of arm, and whether magnet is on or off.
 */
struct Position {
  int angle;
  int height;
  bool magnet;
};

/// maximal number of robot positions we want to store.
#define MAX_LEN 255

/// list of taught positions
Position pos[MAX_LEN];
/// pointer beyond last used element of list pos
int num_pos = 0;

/**
   teach the robot a number of positions.
   use buttons left, right, up, down and toggle_magnet to manoeuver
   the robot into desired positions and magnet state, then press
   teach_in button to store this postion (and magnet state).

   Press run button to end the teaching.
 */
void teach() {

  printf("Teaching...");

  num_pos = 0;

  while(not run.isDepressed()) {

    static bool on = 0; 

    if(teach_in.isDepressed()) {
      if(num_pos < MAX_LEN) 
	pos[num_pos++] = {angle.getReading(), height.getReading(), on};
      else {
	printf("ERROR: used up all position storage.\n");
        return;
      }
    }
    while(left.isDepressed()) rotate.left();
    while(right.isDepressed()) rotate.right();
    while(up.isDepressed()) arm.up();
    while(down.isDepressed()) arm.down();
    if(toggle_magnet.isDepressed()) {
      on ^= true;
      if(on) magnet.on();
      else demagnetise_off(magnet);
      while(toggle_magnet.isDepressed()) { /* wait for button release */};
    }
  }
}
  

/**
   replay leant sequence of positions
*/
void replay() {

  printf("Replaying...");

  for(int current = 0; current < num_pos; current++) {

    // Avoid obstacles by first moving up:
    if(pos[current].height > height.getReading()) {
      move_arm(pos[current].angle);
    }

    rotate_to(pos[current].angle);

    // Avoid obstacles by last moving down:
    if(pos[current].height < height.getReading()) {
      move_arm(pos[current].angle);
    }
    pos[current].magnet ? magnet.on() : demagnetise_off(magnet);
  }
}

/**
   run teaching is the teachin button is depressed, otherwise replay
   the learn sequence if any.
*/
void loop() {

  while(1) {

    if(teach_in.isDepressed()) {
      while(teach_in.isDepressed());
      teach();
    }
    replay();
  }
}
