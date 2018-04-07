#include "Input.h"

#define HYSTERIA 100
#define MAX_READING 1024



Input::Input(byte io)
  : pin(io), previous(Signal::Low) {
}

void Input::initialise() {
		pinMode(pin, INPUT);
}

Signal Input::signal() {
	Signal current = Signal::Low;
	byte reading = digitalRead(pin);
	switch(previous) {
		case Low:
	  case Falling:
			if(reading) current = Signal::Rising;
			break;
		case High:
	  case Rising:
			if(reading) current = Signal::High;
		  else if(reading) current = Signal::Falling;
		  break;

	}
	previous = current;
	return current;
}
