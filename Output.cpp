#include "Output.h"

#define TRIGGER_PULSE 20

Output::Output(byte io)
 : pin(io), triggerStart(0) {
}

void Output::initialise() {
		pinMode(pin, OUTPUT);
}

byte Output::signal(Signal signal, OutMode mode, int step) {
	byte out = LOW;
	switch(mode) {
		case Trigger:
			if (step) out = handleTrigger(signal);
		  break;
		case Gate:
			if (step) out = HIGH;
		  break;
		case Clock:
		  if (step && (signal == Signal::Rising || signal == Signal::High)) out = HIGH;
			break;
	}
	digitalWrite(pin, out);
	return out;
}

byte Output::handleTrigger(Signal signal) {
	byte out = LOW;
	unsigned long now = millis();
	if (signal == Signal::Rising) {
		out = HIGH;
		triggerStart = now;
	} else if (signal == Signal::High && (triggerStart != 0) && (now - triggerStart < TRIGGER_PULSE)) {
		out = HIGH;
	} else {
		triggerStart = 0;
	}

	return out;
}
