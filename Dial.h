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
  void read();
  bool isChanged();
  int value();
  byte value(byte min, byte max);
  int value(int min, int max);
private:
  byte pin;
  int oldValue;
  DialEvent event;
};

#endif
