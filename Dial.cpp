#include "Dial.h"

#define TOLERANCE 10
#define MIN_VALUE 0
#define MAX_VALUE 1023

Dial::Dial(byte io)
  : pin(io) {
}

void Dial::initialise() {
  pinMode(pin, INPUT);
  read();
}

void Dial::read() {
  int newValue =  analogRead(pin);
  event.changed = abs(event.value - newValue) > TOLERANCE;
  event.value = newValue;
  oldValue = newValue;
}

bool Dial::isChanged() {
  return event.changed;
}

int Dial::value() {
  return event.value;
}

byte Dial::value(byte min, byte max) {
  return map(event.value, MIN_VALUE , MAX_VALUE, min, max);
}

