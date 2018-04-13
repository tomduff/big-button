#include "Dial.h"

#define MIN_VALUE 0
#define MAX_VALUE 1023

Dial::Dial(byte io)
  : pin(io) {
}

void Dial::initialise() {
  pinMode(pin, INPUT);
}

bool Dial::read(byte min, byte max) {
  int newValue =  map(analogRead(pin), MIN_VALUE , MAX_VALUE, min, max);
  event.changed = event.value != newValue;
  event.value = newValue;
  return event.changed;
}

bool Dial::isChanged() {
  return event.changed;
}

byte Dial::value() {
  return event.value;
}

