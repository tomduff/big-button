#include "Button.h"

#define HOLD_TIME 100

Button::Button(byte io, bool holdable)
  : pin(io), canHold(holdable) {
}

void Button::initialise() {
  pinMode(pin, INPUT);
  holdStart = 0;
  oldState = ButtonState::Released;
  event.changed = false;
  event.state = ButtonState::Released;
}

void Button::read() {
  byte newState =  digitalRead(pin);
  ButtonState eventState = ButtonState::Released;

  if (canHold) {
    if (!newState) {
      if (oldState) {
        eventState = ButtonState::Clicked;
        holdStart = 0;
      }
    } else {
      if (!oldState) holdStart = millis();
      else if (holdStart != 0 && millis() - holdStart > HOLD_TIME) eventState = ButtonState::Held;
    }
  } else {
    eventState = newState ? ButtonState::Clicked : ButtonState::Released;
  }

  event.changed = event.state != eventState;
  event.state = eventState;
  oldState = newState;
}

bool Button::isChanged() {
  return event.changed;
}

bool Button::isReleased() {
  return event.state == ButtonState::Released;
}

bool Button::isClicked() {
  return event.state == ButtonState::Clicked;
}

bool Button::isHeld() {
  return canHold && event.state == ButtonState::Held;
}

