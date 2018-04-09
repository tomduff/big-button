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
// LED (big button LED) pin 18 (A4)

// OUT 1 ... pin 8
// OUT 2 ... pin 9
// OUT 3 ... pin 10
// OUT 4 ... pin 11
// OUT 5 ... pin 12
// OUT 6 ... pin 13

#include "Input.h"
#include "Output.h"
#include "Button.h"
#include "Dial.h"
#include "Shuffle.h"
#include "Track.h"
#include "ClockGenerator.h"

#define CLOCK_INTERUPT 0
#define BANK_BUTTON_INTERUPT 1

#define TRIGGER 25
#define TOLERANCE 0
#define TRACKS 6
#define BUTTONS 6
#define DIALS 3

#define CLOCK_WAIT 5000

#define CLOCK 2
#define BANK_BUTTON 3
#define CLEAR_BUTTON 4
#define FILL_BUTTON 5
#define RESET_BUTTON 6
#define DELETE_BUTTON 7
#define BIG_BUTTON 19

#define CHANNEL_POT 0
#define STEP_POT 1
#define SHIFT_POT 2

#define BIG_LED 18
#define TRACK1 8
#define TRACK2 9
#define TRACK3 10
#define TRACK4 11
#define TRACK5 12
#define TRACK6 13

byte index = 0;
byte active = 0;
byte shift = 0;
byte steps = 0;
bool clocked = false;
unsigned long now = 0;
unsigned long duration = 0;
unsigned long lastClock = 0;

//ClockGenerator clockGenerator = ClockGenerator();

Shuffle shuffle = Shuffle();

Input clockInput = Input(CLOCK);

Button bankButton = Button(BANK_BUTTON, false);
Button clearButton = Button(CLEAR_BUTTON, true);
Button fillButton = Button(FILL_BUTTON, false);
Button resetButton = Button(RESET_BUTTON, false);
Button deleteButton = Button(DELETE_BUTTON, false);
Button bigButton = Button(BIG_BUTTON, false);

Dial channelDial = Dial(CHANNEL_POT);
Dial stepDial = Dial(STEP_POT);
Dial shiftDial = Dial(SHIFT_POT);

Button *button[BUTTONS] = {&bankButton, &clearButton, &fillButton, &resetButton, &deleteButton, &bigButton};
Dial *dial[DIALS] = {&channelDial, &stepDial, &shiftDial};
Output out[TRACKS] = {Output(TRACK1), Output(TRACK2), Output(TRACK3), Output(TRACK4), Output(TRACK5), Output(TRACK6)};
Track track[TRACKS] = {Track(), Track(), Track(), Track(), Track(), Track()};

void setup() {
  clockInput.initialise();
  for (index = 0; index < BUTTONS; ++index) button[index]->initialise();
  for (index = 0; index < TRACKS; ++index) {
    out[index].initialise();
    track[index].initialise();
  }
  pinMode(BIG_LED, OUTPUT);
  startUpDisplay();

  //Serial.begin(9600);
}

void loop() {
  now = millis();

  // Clock on, Step on
  handleClock(clockInput.signal());

  // Clock on people
  //  if (!clockGenerator.isRunning()) handleClock(clockInput.signal());
  //  else handleClock(clockGenerator.tick());

  for (index = 0; index < BUTTONS; ++index) button[index]->read();
  for (index = 0; index < DIALS; ++index) dial[index]->read();

  if (channelDial.isChanged()) active = channelDial.value(0, TRACKS - 1);
  if (stepDial.isChanged()) track[active].setLength(stepDial.value(0, MAX_STEP_INDEX));
  if (shiftDial.isChanged()) track[active].rotatePattern(shiftDial.value(-track[active].getLength(), track[active].getLength()));

  if (resetButton.isChanged() && resetButton.isClicked()) {
    for (index = 0; index < TRACKS; ++index) track[index].reset();
    shuffle.reset();
  }

  if (clearButton.isChanged()) {
    if (clearButton.isClicked()) track[active].clearPattern(); // Just dump the recorded pattern
    if (clearButton.isHeld()) track[active].clear(); // Dump all track set up
  }

  if (bigButton.isChanged() && bigButton.isClicked()) {
    // Quantize to nearest beat
    if (now - lastClock < duration / 2) track[active].setPattern(HIGH);
    else track[active].setNextPattern(HIGH);
  }
  if (deleteButton.isChanged() && deleteButton.isClicked()) track[active].setPattern(LOW);

  if (fillButton.isChanged()) track[active].setFill(fillButton.isClicked());
}

void handleClock(Signal signal) {

  // Let shuffle know the new clock signal
  shuffle.clock(signal);

  for (index = 0; index < TRACKS; ++index) {

    // Step on
    if (signal == Signal::Rising) {
      track[index].stepOn();
      clockOn();
    }

    // Shuffle step
    Signal signal = shuffle.tick(track, track[index].getShuffle());

    // Force a trigger when the button is pressed regardless as to whether we are on a beat
    if (index == active && bigButton.isChanged() && bigButton.isClicked()) signal = Signal::Rising;

    // Otherwise only trigger when we step to a trigger
    else if (!track[index].getStepped() && Signal::Rising) signal = Signal::Low;

    // Send that beat
    out[index].write(signal, track[index].getOutMode(), track[index].getStep());
  }

  if (track[active].getPosition() % 16 == 0) digitalWrite(BIG_LED, HIGH);
  else digitalWrite(BIG_LED, LOW);
}

void clockOn() {
  if (lastClock != 0) duration = now - lastClock;
  lastClock = now;
}

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

