#ifndef Button_h_
#define Button_h_

#include <Arduino.h>

enum ButtonState {
  Released = 0,
  Clicked = 1,
  Held = 2
};

struct ButtonEvent {
  bool changed;
  ButtonState state;
};

class Button {
public:
  Button(byte io, bool hold);
  void initialise();
  void read();
  bool isChanged();
  bool isReleased();
  bool isClicked();
  bool isHeld();
private:
  byte pin;
  bool canHold;
  unsigned long holdStart;
  byte oldState;
  ButtonEvent event;
};

#endif
