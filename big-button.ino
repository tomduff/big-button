//KOSMO BIG BUTTON 2016 SAM BATTLE
//email address is computer@lookmumnocomputer.com
//look mum no computer
//lookmumnocomputer@gmail.com
//www.facebook.com/lookmumnocomputer
//ITS AN ABSOLUTE MESS BUT IT WORKS.... SORT OF....
//YOU NEED QUITE A CLEAN CLOCK TRIGGER SIGNAL. QUITE A SHORT PULSE!
//To make it work better add this circuit to the clock input :-
//https://www.cgs.synth.net/modules/cgs24_gatetotrigger.html

//the premise of this is a simple performance sequencer.
//it is used in synth bike mk2 to sequencer the drums.
//i figured whats the point in not sharing it!!!
//dont be a pleb and steal it for a product or some shit. Build it and
//enjoy it as a musical instrument :)

// Clock in is pin 2
// Clear Button is pin 4 .... this clears the whole loop sequence
// Button Delete is pin 7 This deletes the step your on
// Bank select Pin 3 .... each channel has 2 banks of patterns, so you can record 2 alternative patterns and go between with this button
// BIG BUTTON is pin 19 (A5), this is the performance button!
// Reset in is pin 6 you can add a button or a jack!
// FILL BUTTON pin 5, push this and it will continuously play the channel your on, doesnt record, push it and twist the select knob to make a fill!
// STEP LENGTH analog pin 1 (A1)
// Channel select Analog pin (A0)
// SHIFT KNOB Analog pin (A2)
// LED (big button LED) pin 20 (A6)

// OUT 1 ... pin 8
// OUT 2 ... pin 9
// OUT 3 ... pin 10
// OUT 4 ... pin 11
// OUT 5 ... pin 12
// OUT 6 ... pin 13

#include "Input.h"
#include "Output.h"
#include "Button.h"
#include "Shuffle.h"
#include "Track.h"
#include "ClockGenerator.h"

#define CLOCK_INTERUPT 0
#define BANK_BUTTON_INTERUPT 1

#define TRIGGER 25
#define TOLERANCE 0
#define TRACKS 6
#define BUTTONS 6

#define CLOCK_WAIT 5000

#define CLOCK 2
#define BANK_BUTTON 3
#define CLEAR_BUTTON 4
#define FILL_BUTTON 5
#define RESET_BUTTON 6
#define DELETE_BUTTON 7
#define BIG_BUTTON 19

#define BIG_LED 18
#define TRACK1 8
#define TRACK2 9
#define TRACK3 10
#define TRACK4 11
#define TRACK5 12
#define TRACK6 13

byte channel = 0;
byte shift = 0;
byte steps = 0;
bool clocked = false;
unsigned long now = 0;
unsigned long lastClock = 0;

ClockGenerator clockGenerator = ClockGenerator();
Shuffle shuffle = Shuffle();
Input clockInput = Input(CLOCK);
Button bankButton = Button(BANK_BUTTON, false);
Button clearButton = Button(CLEAR_BUTTON, true);
Button fillButton = Button(FILL_BUTTON, false);
Button resetButton = Button(RESET_BUTTON, false);
Button deleteButton = Button(DELETE_BUTTON, false);
Button recordButton = Button(BIG_BUTTON, false);

Button button[BUTTONS] = {bankButton, clearButton, fillButton, resetButton, deleteButton, recordButton};
Output out[TRACKS] = {Output(TRACK1), Output(TRACK2), Output(TRACK3), Output(TRACK4), Output(TRACK5), Output(TRACK6)};
Track track[TRACKS] = {Track(), Track(), Track(), Track(), Track(), Track()};

void setup() {

  clockInput.initialise();
  for (int index = 0; index < BUTTONS; ++index) button[index].initialise();
  for (int index = 0; index < TRACKS; ++index) {
    out[index].initialise();
    track[index].initialise();
  }

  pinMode(BIG_LED, OUTPUT);
  startUpDisplay();
}

void loop() {
  now = millis();

  activeChannel();
  shiftAmount();
  numberOfSteps();

  for (int index = 0; index < BUTTONS; ++index) button[index].read();

  if (resetButton.isChanged() && resetButton.isClicked()) {
    shuffle.reset();
    for (int index = 0; index < TRACKS; ++index) track[index].reset();
  }

  if (clearButton.isChanged()) {
    if (clearButton.isClicked()) track[channel].clearPattern(); // Just dump the recorded pattern
    if (clearButton.isHeld()) track[channel].clear(); // Dump all track set up
  }

  if (recordButton.isChanged() && recordButton.isClicked()) track[channel].setPattern(HIGH);
  if (deleteButton.isChanged() && deleteButton.isClicked()) track[channel].setPattern(LOW);
  if (recordButton.isChanged()) track[channel].setFill(fillButton.isClicked());

  // TODO : Bank stuff?
  // TODO : Shift stuff
  // TODO : Step stuff

  // Clock on people
  if (!clockGenerator.isRunning()) handleClock(clockInput.signal());
  else handleClock(clockGenerator.tick());

}

void handleClock(Signal signal) {
  shuffle.clock(signal);
  if (signal == Signal::Low && (now - lastClock) > CLOCK_WAIT) {
    clocked = false;
    lastClock = 0;
  } else if (signal == Signal::Rising || signal == Signal::High) {
    clocked = true;
    lastClock = now;
  }

  for (int index = 0; index < TRACKS; ++index) {
    if (signal == Signal::Rising) track[index].stepOn();
    handleStep(index);
  }

  if (track[channel].getPosition() % 16 == 0) digitalWrite(BIG_LED, HIGH);
  else digitalWrite(BIG_LED, LOW);
}

void handleStep(int index) {
  int step = track[index].getStep();
  Signal signal = shuffle.tick(track, track[index].getShuffle());
  if (!track[index].getStepped() && Signal::Rising) signal = Signal::Low;
  int output = out[index].signal(signal, track[index].getOutMode(), step);
}

void activeChannel() {
  channel = map(analogRead(0), 0, 1023, 0 , 5);
}

void numberOfSteps() {
  steps = 1 << map(analogRead(1), 0, 1023, 2, 6);
}

void shiftAmount() {
  shift = map(analogRead(2), 0, 1023, 0, steps);
}

//void debug() {
//  for (int i = 0; i < TRACKS; ++i) {
//    digitalWrite(out[i], HIGH);
//    delay(50);
//    digitalWrite(out[i], LOW);
//  }
//}
//
//void debugOut(int out) {
//  for (int i = 0; i < 5; ++i) {
//    digitalWrite(out[out], HIGH);
//    delay(50);
//    digitalWrite(out[out], LOW);
//    delay(50);
//  }
//}
//
//void debugNumber(int number, int out) {
//  for (int i = 0; i < number; ++i) {
//    digitalWrite(out[out], HIGH);
//    delay(200);
//    digitalWrite(out[out], LOW);
//    delay(200);
//  }
//  delay(2000);
//}

void startUpDisplay() {
  digitalWrite(BIG_LED, HIGH);
  delay(100);
  digitalWrite(BIG_LED, LOW);
  delay(20);
  digitalWrite(BIG_LED, HIGH);
  delay(80);
  digitalWrite(BIG_LED, LOW);
  delay(20);
  digitalWrite(BIG_LED, HIGH);
  delay(60);
  digitalWrite(BIG_LED, LOW);
  delay(20);
  digitalWrite(BIG_LED, HIGH);
  delay(40);
  digitalWrite(BIG_LED, LOW);
  delay(20);
  digitalWrite(BIG_LED, HIGH);
  delay(20);
  digitalWrite(BIG_LED, LOW);
  delay(10);
  digitalWrite(BIG_LED, HIGH);
  delay(20);
  digitalWrite(BIG_LED, LOW);
}

