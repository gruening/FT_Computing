/**
 * @file   FT_Computing.cpp
 * @author Copyright Andre Grüning 2015, 2016.
           Released under the GNU v3 licence or later, see <URL>
 * @date   Sun Jan 17 18:20:36 2016
 * 
 * @brief  Library for using the Arduino Motor Shield to act like the fischertechnik Computing interface
 */

#include "Arduino.h"
#include "FT_Computing.h"

FT_Computing::FT_Computing(int vss,
			   int pwmM1, int dirM1,
			   int pwmM2, int dirM2, 
			   int pwmM3, int dirM3,
			   int pinE0, int pinE1,
			   int pinE2, int pinE3,
			   int pinE4, int pinE5,
			   int pinE6, int pinE7,
			   int pinEX, int pinEY) : 
  //maxPwm(2*(255*vMax) / (6*vMot)),
  //maxPwm( (255*6) / vss );
  motor1(pwmM1, dirM1),
  motor2(pwmM2, dirM2),
  motor3(pwmM3, dirM3),
  e0(pinE0), e1(pinE1), 
  e2(pinE2), e3(pinE3),
  e4(pinE4), e5(pinE5),
  e6(pinE6), e7(pinE7),
  e8(e0),
  ex(pinEX), ey(pinEY)
{

  // set timer 2 divisor to 32 for PWM frequency of 980.39 Hz -- seems
  // to work better than with the FT mini-motors 490 Hz (or both
  // divided by 2? as the symmetric one is used?)
  // TCCR2B = (TCCR2B & B11111000) | B00000101;    
  TCCR2B = (TCCR2B & B11111000) | B00000101;    
}
