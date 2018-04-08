#ifndef Output_h_
#define Output_h_

#include <Arduino.h>
#include "Io.h"

class Output {
public:
  Output(byte io);
  virtual void initialise();
  byte write(Signal signal, OutMode mode, byte value);
private:
  byte pin;
  unsigned long triggerStart;
  byte handleTrigger(Signal signal);
};

#endif
