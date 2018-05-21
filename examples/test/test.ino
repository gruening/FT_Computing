/* Fischertechnik Computing Code 
   Copyright 2015, 2016, 2018 Andre Gruening
   GNU Licence v3 or later
*/

#include "FT_Computing.h"

FT_Computing ft(50);

/// Compare http://forum.arduino.cc/index.php/topic,120440.0.html
int serial_putchar(char ch, FILE* f) {
  // expand nl to cr nl
  if (ch == '\n') serial_putchar('\r',f);
  // write char to 
  return Serial.write(ch) == 1? 0 : 1;
}

FILE serial;

void setup(){
    Serial.begin(9600);

    // Set up stdout
    fdev_setup_stream(&serial, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial;
}


const Input* const e[] = {&ft.e0, &ft.e1, &ft.e2, &ft.e3, &ft.e4, &ft.e5, &ft.e6, &ft.e7, &ft.e8};
const int numberInputs = sizeof(e) / sizeof(e[0]);


void loop () {

  bool flag = false;
  for(int i = 0; i < numberInputs; i++) {
    if(e[i]->isActive()) {
      printf("E%1u\t", i);
      flag = true;
    }
  }
  if(flag) putc('\n', stdout);
  printf("EX: %i\t", ft.ex.getReading());
  printf("EY: %i\n", ft.ey.getReading());
  delay(1);
}
