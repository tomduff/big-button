#include "Button.h"

#define HOLD_TIME 100

Button::Button(byte io, bool holdable)
  : pin(io), canHold(holdable), holdStart(0), oldState(ButtonState::Released) {
   pinMode(pin, INPUT);
}

ButtonState Button::event() {
  return canHold ? holdState() : readState();
}

ButtonState Button::holdState() {
  ButtonState event = ButtonState::Released;
  ButtonState state = readState();
  if (state == ButtonState::Released) {
    if (oldState == ButtonState::Clicked) {
      event = ButtonState::Clicked;
      holdStart = 0;
    }
  } else {
    if (oldState == ButtonState::Released) holdStart = millis();
    else if (isHeld()) event = ButtonState::Held;
  }

  return event;
}

ButtonState Button::readState() {
  return digitalRead(pin) ? ButtonState::Clicked : ButtonState::Released;
}

bool Button::isHeld() {
  return (holdStart != 0 && millis() - holdStart > HOLD_TIME);
}
