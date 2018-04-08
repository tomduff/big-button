#ifndef ClockGenerator_h_
#define ClockGenerator_h_

#include <Arduino.h>
#include "Io.h"

class ClockGenerator {
public:
  ClockGenerator();
  Signal tick();
  void setSpeed(byte offset);
  void setWidth(byte offset);
  void setMulitplier(byte offset);
  int getSpeed();
  int getWidth();
  int getMulitplier();
  void start();
  void stop();
  void reset();
  bool isRunning();
private:
  unsigned long last;
  long speed;
  int width;
  int multiplier;
  int running;
};

#endif
