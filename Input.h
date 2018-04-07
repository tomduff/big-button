#ifndef Input_h_
#define Input_h_

#include <Arduino.h>
#include "Io.h"

class Input {
public:
  Input(byte io);
  virtual void initialise();
  Signal signal();
private:
  byte pin;
  Signal previous;
};

#endif
