#ifndef Button_h_
#define Button_h_

#include <Arduino.h>

enum ButtonState {
  Released = 0,
  Clicked = 1,
  Held = 2
};

class Button {
public:
  Button(byte io, bool hold);
  ButtonState event();
  bool isHeld();
private:
  byte pin;
  bool canHold;
  unsigned long holdStart;
  ButtonState oldState;
  ButtonState readState();
  ButtonState holdState();
};

#endif
