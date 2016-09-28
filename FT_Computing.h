/** \file FT_Comuting.h --Library for using the Arduino Motor Shield to act
    like the fischertechnik Computing interface.

    Copyright Andre Grüning, 2015, 2016
    Released under the GNU v3 licence or later
*/

/**
 * @file   FT_Computing.h
 * @author gruening <gruening@eee>
 * @date   Sun Jan 17 17:19:23 2016
 * 
 * @brief  
 * 
 * 
 */

#ifndef FT_Computing_h
#define FT_Computing_h 1

#include "Arduino.h"

/** A general pwm driven motor. Can also be used for other actuators such as magnets etc. The
    direction signal is optional.

    Whether right/up/forwards really means right/up/forwards in your
    actual motor depends on the polarity of the wiring of the
    motor. In any case left/down/backwards will switch on the motor in
    the opposite direction of right/up/forwards.
*/
class Motor {
private:
  const int _pwmPin; /// pin for pwm signal
  const static int _maxDutyCycle = 96; /// \todo adaptable maximal duty cycle --- currently hard-coded
  const int _directionPin; /// pin for direction signal, or -1 if none.

public:  
  /** Creates a new motor. If you are not driving a motor but a devise
      for witch change of direction does not make sense, ie a filement
      lamp or electromagnet, you can leave out the dirPin argument. 

      The code in this constructor sets up the pins of the port and
      puts them into a defined state.


      @param pwmPin pin to use for pwm regulation of output
      @param directionPin pin to use for switching direction of
      ouput, default is -1, ie no pin for direction switches.  
  */
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

  /** 
   * Switches on a motor rotating "right". If argument pwm is left
   * out, maxDutyCycle is used.
   * 
   * @param pwm optional, the dutycycle of this motor output 0-255.
   */
  void right(int pwm = _maxDutyCycle) const {
    if(_directionPin != -1) digitalWrite(_directionPin, LOW);
    if(pwm > _maxDutyCycle) pwm = _maxDutyCycle;
    analogWrite(_pwmPin, pwm);
  };
  /** 
   * Switches on a motor rotating "left", ie opposite to what a call
   * to right() would do. If argument pwm is left
   * out, maxDutyCycle is used.
   * 
   * @param pwm optional, the dutycycle of this motor output 0-255.
   */
  void left(int pwm = _maxDutyCycle) const {
    if(_directionPin != -1) digitalWrite(_directionPin, HIGH);
    analogWrite(_pwmPin, pwm);
  };
  /** 
      Convenience function if your motor moves sth upwards instead of
      right. Calls right().
      @param pwm option, the duty cycle
  */
  void up(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  /** 
   * Convenience function. Calls left().
   * 
   * @param pwm optional, the duty cycle.
   */
  void down(int pwm = _maxDutyCycle) const {
    this->left(pwm);
  };
  /** 
   * Convenience function if your motor moves sth forwards rather then
   * right. Calls right().
   * 
   * @param pwm optional, the duty cycle
   */
  void forwards(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  /** 
   * Convenience function. Calls left().
   * 
   * @param pwm optional the duty cycle
   */
  void backwards(int pwm = _maxDutyCycle) const {
    this->left(pwm);
  };
  /** 
   * Convenience function. Calls right(). Use in case polarity does
   * not matter for the actuactor.
   * 
   * @param pwm optional the duty cycle
   */
  void on(int pwm = _maxDutyCycle) const {
    this->right(pwm);
  };
  /** 
   * switches actuator off. Also brings the direction pin into a defined state.
   * 
   */
  void off() const {
    this->right(0);
  };
};

/** 
 * represents a generic digital input in an object-oriented style.
 */
class Input {

private:
  const int _pin; /// the pin number of the output
  const int _active; /// whether this Input is active HIGH or active  LOW (reversed logic)

public:
  /** 
   * Creates and initialised a new input. Hides logical details from
   * the user.
   * 
   * @param pin the Arduino pin number to use for this Input
   * @param active optional, whether the input is logically considered
   * active if it receives a HIGH voltage or a LOW voltage (default).
   */
  Input(int pin, int active = LOW) : _pin(pin), _active(active) {
    if(pin != -1)
      pinMode(_pin, INPUT_PULLUP);
  };

  /** 
   * returns the logic activity stage of this pin taking into account
   * the logic of the pin.
   * @return true if this Input is logically active, false otherwise.
   */
  bool isActive() const {
    if(_pin == -1) return false;
    return digitalRead(_pin) == _active;
  }
  /** 
   * Convenience function. Calls isActive() and inverts the result.
   * @return true if this input is  logically inactive, false otherwise.
   */
  bool isInactive() const {
    return ! this->isActive();
  }
  /** 
   * Convenience function. Wraps a call to isActive(). Useful where
   * the active state of this Input is though of a button being depressed.
   *
   * @return true if the connected sensor is depressed, false otherwise.
   */
  bool isDepressed() const {
    return this ->isActive();
  }
  /** 
   * Convenience function. Wraps a call to isInactive().
   * @return true if the sensor connected to this Input is not depressed. 
   */
  bool isReleased() const {
    return this ->isInactive();
  }
  /** 
   * @return the Ardiuno pin this Input is using.
   */
  int toPin() const {
    return _pin;
  }
};

class Sensor {

private:
  const int _pin;

public:
  Sensor(const int pin) : _pin(pin) {};
  int getReading() const {
    return analogRead(_pin);
  }
};

/** 
 * Arduino Interface to work with the fischertechnik Computing kits
 * from the 80ties.
 *
 * This base Comptuting kit allow to contruct model of simple
 * industrial robots and other devices controlled by computers. See
 * the examples that come with this class for how to use it for
 * diverse suggested models of the Computing kit.
 *
 * The Computing kit contains as sensors:
 * - 8 microswitch push buttons as digital inputs,
 * - 2 potentiometers as analog inputs 
 * and as actuator:
 * - 3 fischertechnik lamps
 * - 2 fishertechnik mini motors
 * - 1 electro magnet
 *
 * In the eighties fishertechnik provided hardware interfaces to
 * connect these components with then popular microcomputers, such as
 * diverse home computers from Commodore, Atari, Schneider etc, and
 * even the NDR-Klein-Computer (the German equivalent of the BBC
 * computer in Britain.)
 * 
 * Those interface could control up to 8 digital input lines (E1 to
 * E8), and 2  * analogue ones (EX and EY), and it could drive up to 4
 * digital outputs (M1 to M4) for the motors, magnets etc. 
 *
 * The computers for which interfaces existed are all long gone (are
 * they? I still have them in my basement), but today the Arduino is
 * alive. Hence we use it to drive the fischertechnik Computing kit.
 *
 * We connect the Arduino with the Arduino motor shield <URL> which
 * provides 2 motor outputs with enough power to drive the old
 * fischertechnic minimotors. 2 motor outputs are enough for the base
 * fischertechnik Computing kits. (More advanced kits would use up to
 * 4 motors). 
 *   
 * If you want to use also the electro magnet from the
 * kits, you will need to have a circuit that provides sufficient
 * current to the magnet as the Arduino pins themselves can only provide
 * up to 20mA (see <URL>). Such a circuit can be easily build with a
 * single power transisotor and a diode, and the schematics are
 * provided with this package at <URL>.
 *
 * All these power outputs for motors and magenets etc are capable of
 * PWM. PWM can be used in two ways for us in the ft context:
 * - ft kits were at those designed for a nominal voltage of 6V (if I
 *   am correct -- I could not find any documentation for that any
 *   longer). So if you use a power supply that provides more than 6V
 *   (the one I use provides eg 12 V), you need to make sure that the
 *   motors do not get the full 12 V, see <PWM> -- and this can be
 *   achieved by running them with a duty cycle less than
 * 255. < @todo attached my calculatrion here>
 * - in principle you can also reduce the speed with pwm, but the ft
 *   minimotor were not designed with pwm in mind -- so 1. they do get
 *   quite (insanely?) hot when driven by pwm, and 2. they do not run
 *   well with low duty cycles -- hence your choices of speed via pwm
 *   are pretty limited.
 * 
 * I suggest to connect the motor not per the old ribbon cabel
 * As opposed to the orignal use of the push buttons in the
 * fischertechnik Instruction (and actually also the standard Arduino
 * style (see <URL>), we use them here a active low
 * style. While this is wrapped and hidden from the user of this
 * interface (ie a pressed button eg e8 will lead to e8.inActive()
 * return true) you have to remember this when wiring the
 * fischertechnik buttons:
 * They need to be connect with one pole to the corresponding Arduino
 * input pin, but they need to be connect to GND -- and *NOT* to 5V as
 * in the ft instructions.
 * This change was done to be able to connect with as few external
 * components as possible.
 *
 * A second change concerns the analog inputs, ie the potentiometers:
 * I recommend connecting them directly to the analog inputs pins
 * provide by the Motor shield (ie their two outer are connected to
 * GND and 5V, while the middle pin is connect to an Arduino analog
 * input pin, see attached schematics at <URL>.
 * Also this change is due to avoiding aditional components beyond
 * what the Arduino and its motor shield provide anyway.
 */
class FT_Computing {

public:
  
  /**
   * maximum admissable ontime for the PWM pins in order not to
   * overpower the minimotors for given supply voltage
   */
  const int maxPwm;  

  /** 
   * creates an instance of the Ardunio ft interface. Among other
   * things, it essentially specifies to which Arduino pins the ft
   * actuators and sensors are connected, and sets up these pins
   * correctly. All these pins have defaults, as speficied in the
   * header declaration below, and the corresponding arguments can
   * hence be left out if you connect to the same pins as I did.
   * 
   * @param vMot  the supply voltage to the motor
   * @param pwmM1 pin for pwm signal for Motor 1
   * @param dirM1 pin for direction signal for Motor 1
   * @param pwmM2 pin for pwm signal for Motor 2
   * @param dirM2 pin for direction signal for Motor 2
   * @param pwmM3 pin for pwm signal for Motor 3
   * @param dirM3 pin for direction signal for Motor 3
   * @pinE0 pin digital input line E0 is connected to (this has an
   * alias of E8 to stick with the ft convention).
   * @pinE1 pin digital input line E1 is connected to.
   * @pinE2 pin digital input line E2 is connected to.
   * @pinE3 pin digital input line E3 is connected to.
   * @pinE4 pin digital input line E4 is connected to.
   * @pinE5 pin digital input line E5 is connected to.
   * @pinE6 pin digital input line E6 is connected to.
   * @pinE7 pin digital input line E7 is connected to.
   * 
   */
  FT_Computing(int vMot = 12, 
	       int pwmM1 = 3,
	       int dirM1 = 12, 
	       int pwmM2 = 11, 
	       int dirM2 = 13, 
	       int pwmM3 = 10, 
	       int dirM3 = -1,
	       int pinE0 = A5,
	       int pinE1 = -1, // assign
	       int pinE2 = -1, // assign 
	       int pinE3 = A4, 
	       int pinE4 = 5, 
	       int pinE5 = 7, 
	       int pinE6 = 4, 
	       int pinE7 = 2,

	       // two analog inputs:
	       int pinEX = A2, 
	       int pinEY = A3 
	       );

  const static int vMax = 6; /// maximum average output voltage [V] for the actuators (ft is 6V based)
  const Motor motor1; /// first motor output
  const Motor motor2; /// second motor output
  const Motor motor3; /// third motor output

  const Input e0; /// digital input line E0 (alias E8).
  const Input e1; /// digital input line E1.
  const Input e2; /// digital input line E2.
  const Input e3; /// digital input line E3.
  const Input e4; /// digital input line E4.
  const Input e5; /// digital input line E5.
  const Input e6; /// digital input line E6.
  const Input e7; /// digital input line E7.
  const Input& e8; /// digital input line E8 as an alias for E0 -- to
		   /// stick with ft numbering convention.


  const Input& e[]; /// all digitial inputs in an array


  const Sensor ex;  /// EX analog input
  const Sensor ey;  /// EY analog input
};

#endif
