#ifndef Dial_h_
#define Dial_h_

#include <Arduino.h>

struct DialEvent {
  bool changed;
  int value;
};

class Dial {
public:
  Dial(byte io);
  void initialise();
  bool read(byte min, byte max);
  bool isChanged();
  byte value();
private:
  byte pin;
  DialEvent event;
};

#endif
