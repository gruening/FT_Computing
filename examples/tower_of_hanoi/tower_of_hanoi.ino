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

/// angle of tower
const Sensor& angle = ft.ex;
/// motor to rotate tower
const Motor& rotate = ft.motor1;

/// motor to lift arm
const Motor& lift = ft.motor2;
/// indicate bottom position of arm
const Input& bottom = ft.e3;
/// indicate top position of arm
const Input& top = ft.e4;

/// pwm driver to the electromagnet
const Motor& magnet = ft.motor3;

//!   We have three poles to stack discs on
enum {COPPAR, SILVER, GOLD, NUM_POLES};

//! We have this many discs in total. Discs are ordered in size and labled by their order in size.
#define NUM_DISCS 5

//! tolarance for pole position
#define TOLERANCE 3

/** A Pole with a stack of discs on it. */
struct Pole {
  /** actual stack to store the number and id of discs **/
  int stack[NUM_DISCS];
  /** indicates the topmost used position of the pole, ie the number of discs on it **/
  int top;
  /// tower angle for this pole
  const int angle;
  Pole(const int _angle): top(0), angle(_angle) {};

  /**
      put a new disc on the pole. Check that this new disc is smaller
      than the current top-most disc. 

      @param disc new disc
      @return whether move was legal and carried out.
   */
  bool put(const int disc) {
    if (top != 0 && stack[top-1] < disc) {
      fprintf(stderr, "Illegal move: A bigger disc can never be put on a smaller disc.\n");
      return false;
    }
    else if (top == NUM_DISCS) {
      fprintf(stderr, "Illegal move: The pole cannot take any more discs.\n");
      return false;
    }
    else {
      stack[top++] = disc;
      return true;
    }
  }

  /**
     Take the topmost disc from the pole if any.
   */
  int take() {
    if (top == 0) {
      fprintf(stderr, "Illigal move: No disc on pole.\n");
      return -1;
    }
    return stack[--top];
  }
};

/** Tower of Hanoi has NUM_POLES poles at the give angles:
    Pole angles should be checked by trail and error with the help of test.ino
 */
Pole poles[NUM_POLES] = { Pole(300), Pole(500), Pole(680)};

/**
   rotate tower to position of pole.
   @param pole pole to rotate to.
 */
void rotate_to(const Pole& pole) {

  rotate.left();
  while(angle.getReading() > pole.angle + TOLERANCE);

  rotate.right();
  while(angle.getReading() < pole.angle - TOLERANCE);
  
  rotate.off();
}

/**
   lower arm to bottom position.
 */
void lower_arm() {

  lift.down();
  while (bottom.off());
  lift.off();
}

/**
   raise arm to top position
 */
void raise_arm() {

  lift.up();
  while (top.off());
  lift.off();
}

/**
   Move robot arm to pick up a disc at from and put it down at
   to. In the course of this operation the magnet is switched on with
   a strength appropriate for the disc (ie lighter discs get less
   magnetic field strength.

   @param disc id for disc -- to adjust magnetic strength.
   @param from pole to take disc from
   @param to pole to put disc down on.

   Pre-condition: arm is in the top position.
   Post-condition: arm is in the top position.

   This is the only function that couples the controler with the model
   (in this case the ft robot hardware).

 */
void robot_move_disc(const int disc, const int from, const int to) {

  int strength;
  switch(disc) {
  case 1:
    strength = (Motor::_maxDutyCycle * 2) / 5;
    break;
  case 2:
  case 3:     
    strength = Motor::_maxDutyCycle / 2;
    break;
  default:
    strength = Motor::_maxDutyCycle;
  }

  // pick up disc
  rotate_to(poles[from]);
  lower_arm();
  magnet.right(strength);
  raise_arm();

  // drop disc
  rotate_to(poles[to]);
  lower_arm();
  // demagnetise by reversing polarity for a short time.
  magnet.left(strength);
  delay(10);
  magnet.right(strength);
  delay(2);
  magnet.left(strength);
  delay(1);
  magnet.off();
  raise_arm();
}

/** 
    move a single disc from one pole to another.
    @param from pole to take top-most disc from.
    @param to pole to put the disc on
*/
void move_single_disc(const int from, const int to) {

  const int disc = poles[from].take();
  poles[to].put(disc);
  fprintf(stderr, "Moving Disk %i from %i to %i.\n", disc, from, to);
  robot_move_disc(disc, from, to);
}

/**
   recusrively move a number of discs from one pole to another, keeping the
   invariants of the Tower of Hanoi problem.

   @param from pole to take the discs from.
   @param free pole to use to temporarily store discs.
   @param to pole to put the discs on
   @param number of discs to move from from to to.
 */
void move_discs(const int from, const int free, const int to, const int number) {

  // done -- no discs to move
  if(number == 0) return;

  // recursively move the topmost number-1 discs to the free pole
  move_discs(from, to, free, number-1);

  // move the remaining single disc direct:
  move_single_disc(from, to);

  // recursively move the discs temporarily stored on the free pole to
  // the to pol.
  move_discs(free, from, to, number-1);
}
	
void setup() {

  // Serial.begin(9600);
  Serial.begin(115200);
  // Set up stdout
  fdev_setup_stream(&serial, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial;
  stderr = &serial;
  
  // Fill COPPAR pole with NUM_DISCS discs:
  for(int i = NUM_DISCS; i > 0; i--) {
    poles[COPPAR].put(i);
  }

#ifdef TEST

  lower_arm();
  raise_arm();
  rotate_to(pole[COPPAR]);
  rotate_to(pole[GOLD]);
  exit(0);
#endif

  raise_arm();
  
}

void loop() {

  /* move all discs from the COPPAR to the GOLD pole, using the SILVER
     pole to temporarily store discs. 
  */
  move_discs(COPPAR, SILVER, GOLD, NUM_DISCS);

  exit(0);
}
