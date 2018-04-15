#ifndef Shuffle_h_
#define Shuffle_h_

#include <Arduino.h>
#include "Shuffle.h"
#include "Io.h"

struct ShuffleState {
  unsigned long lastClock;
  Signal clockSignal;
  Signal shuffleSignal[6];
  bool newCycle[6];
};

class Shuffle {
public:
  Shuffle();
  void clock(Signal signal);
  Signal tick(int track, int shuffle);
  void reset();
private:
  unsigned long gate;
  int beat;
  ShuffleState state;
  Signal shuffleClock(int track, int shuffle);
  void newCycle();
};

#endif
