/**
   FT_Comuting.h -- Library for using the Arduino Motor Shield to act
   like the fischertechnik Computing interface  

   Copyright Andre Grüning, 2015, 2016
   Released under the GNU v3 licence or later
*/

#ifndef FT_Computing_h
#define FT_Computing_h 1

#include "Arduino.h"

/** A general pwm driven motor.
    Can also be used for other actuators such as magnets etc. The
    direction signal is optional.
 */
class Motor {
 private:
  const int _pwmPin; /// pin for pwm signal
  const static int _maxDutyCycle = 96; /// \todo maximal duty cycle --- currently hard-coded
  const int _directionPin; /// pin for direction signal, or -1 if none.

 public:  
  Motor(int pwmPin, int directionPin = -1) : 
    _pwmPin(pwmPin), 
    //_maxDutyCycle(maxDutyCycle),
    _directionPin(directionPin) 
  {
    if(_directionPin != -1) {
      pinMode(_directionPin, OUTPUT);
    }
    this->off();
  };

  void right(int pwm = _maxDutyCycle) const {
    if(_directionPin != -1) digitalWrite(_directionPin, LOW);
    analogWrite(_pwmPin, pwm);
  };
  void left(int pwm = _maxDutyCycle) const {
    if(_directionPin != -1) digitalWrite(_directionPin, HIGH);
    analogWrite(_pwmPin, pwm);
  };
  void up(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  void down(int pwm = _maxDutyCycle) const {
    this->left(pwm);
  };
  void forwards(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  void backwards(int pwm = _maxDutyCycle) const {
    this->left(pwm);
  };
  void on(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  void off() const {
    this->right(0);
  };
};


class Input {

 private:
  const int _pin;
  const int _active;

 public:
 Input(int pin, int active = LOW) : _pin(pin), _active(active) {
    pinMode(_pin, INPUT_PULLUP);
  };

  bool isActive() const {
    return digitalRead(_pin) == _active;
  }
  bool isInactive() const {
    return ! this->isActive();
  }
  bool isDepressed() const {
    return this ->isActive();
  }
  bool isReleased() const {
    return this ->isInactive();
  }
  int toPin() const {
    return _pin;
  }
};


class FT_Computing {

 public:
  const int maxPwm;  //* maximum admissable ontime for the PWM pins:
  FT_Computing(int vMot = 12, 
	       int pwmM1 = 3, int dirM1 = 12, 
	       int pwmM2 = 11, int dirM2 = 13, 
	       int pwmM3 = 10, int dirM3 = -1,
	       int pinE0 = A5 /* 2 */, int pinE1 = -1, // assign
	       int pinE2 = -1, // assign 
	       int pinE3 = 6, 
	       int pinE4 = 4, int pinE5 = 5,
	       int pinE6 = 7, int pinE7 = A4
	       );

  const static int vMax = 6; //* maximum average output voltage [V] for the actuators (ft is 6V based)
  const Motor motor1;
  const Motor motor2;
  const Motor motor3;

  /** aliases to match FT labeling for the on/off actuators such as buttons. Read with digitalRead
      Note as opposed to the FT interface, a closed contact is LOW, and open contact
      is HIGH. **/
  const Input e0;
  const Input e1;
  const Input e2;
  const Input e3;
  const Input e4;
  const Input e5;
  const Input e6;
  const Input e7;
  const Input& e8;

  /** aliases for the potentiometres: */
  const int EX; 
  const int EY; 
};

#endif
