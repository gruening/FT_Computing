/**
   FT_Comuting.h - Library for using the Arduino Motor Shield to act like the fischertechnik Computing interface
   Copyright by Andre Grüning, 2015, 2016
   Released under the GNU v3 licence or later
*/

#include "Arduino.h"
#include "FT_Computing.h"

FT_Computing::FT_Computing(int vMot,
			   int pwmM1, int dirM1,
			   int pwmM2, int dirM2, 
			   int pwmM3, int dirM3,
			   int pinE0, 
			   int pinE3,
			   int pinE4, int pinE5) : 
  maxPwm(2*(255*vMax) / (3*vMot)), 
  motor1(pwmM1, dirM1),
  motor2(pwmM2, dirM2),
  motor3(pwmM3, dirM3),
  e0(pinE0), 
  e3(pinE3),
  e4(pinE4), e5(pinE5),
  e8(e0),
  EX(A2), EY(A3) {
  // switch on pullups for digital inputs:
}
