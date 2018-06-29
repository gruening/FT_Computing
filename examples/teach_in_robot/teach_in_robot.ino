/** 
    Code to operate the Tower of Hanoi Robot from the FT Computing kit.
    Copyright 2018 Andre Gruening
    GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(12);

/// Compare http://forum.arduino.cc/index.php/topic,120440.0.html
int serial_putchar(char ch, FILE* f) {
  // expand nl to cr nl
  if (ch == '\n') serial_putchar('\r',f);
  // write char to 
  return Serial.write(ch) == 1? 0 : 1;
}

FILE serial;

/// motor to rotate tower
const Motor& rotate = ft.motor1;
/// angle of tower
const Sensor& angle = ft.ex;

/// motor to lift arm
const Motor& arm = ft.motor2;
/// indicate position of arm
const Sensor& height= ft.ey;
/// pwm driver to the electromagnet
const Motor& magnet = ft.motor3;

const Input& left = ft.e3;
const Input& right = ft.e4;
const Input& up = ft.e5;
const Input& down = ft.e6;
const Input& toggle_magnet = ft.e7;
const Input& teach_in = ft.e8;
const Input& run = ft.e2; // this is not accessible

//! tolarance for position
#define TOLERANCE 3

void rotate_to(const int target) {

  rotate.left();
  while(angle.getReading() > target + TOLERANCE);

  rotate.right();
  while(angle.getReading() < target - TOLERANCE);
  
  rotate.off();
}

/**
   lower arm to bottom position.
 */
void move_arm(const int pos) {

  arm.down();
  while(height.getReading() > pos + TOLERANCE);

  arm.up();
  while(height.getReading() < pos - TOLERANCE);
  
  arm.off();
}

/** assume Magnet is always switch on with magnet.right */
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

struct Position {
  int angle;
  int height;
  bool magnet;
};

#define MAX_LEN 255
  
Position pos[MAX_LEN];
int num_pos = 0;

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
      on ^= 1;
      if(on) magnet.on();
      else demagnetise_off(magnet);
      while(down.isDepressed()) { /* wait for button release */};
    }
  }
}
  


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

void loop() {

  while(1) {

    if(teach_in.isDepressed()) {
      while(teach_in.isDepressed());
      teach();
    }
    replay();
  }
}
