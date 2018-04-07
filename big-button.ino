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

#define CLOCK_INTERUPT 0
#define BANK_BUTTON_INTERUPT 1

#define TRIGGER 25

#define TOLERANCE 0

#define CHANNELS 6

#define CLOCK 2
#define BANK_BUTTON 3
#define CLEAR_BUTTON 4
#define FILL_BUTTON 5
#define RESET_BUTTON 5
#define DELETE_BUTTON 7
#define BIG_BUTTON 19
#define CHANNEL1 8
#define CHANNEL2 9
#define CHANNEL3 10
#define CHANNEL4 11
#define CHANNEL5 12
#define CHANNEL6 13

int buttonPushCounter = 0;   // counter for the number of button presses
volatile int clock = LOW;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int RecordButtonState = 0;
int LastRecordButtonState = 0;
int DeleteButtonState = 0;
int LastDeleteButtonState = 0;
int ClearButtonState = 0;

//Clock Reset Keepers
int ClockKeep = 0;
int ResetSteps = 33;

//RESET BUTTON
int ResetButtonState = LOW;
int LastResetButtonState = LOW;

//FILL BUTTON
int FillButtonState = 0;
byte Fill[6] = {0, 0, 0, 0, 0, 0};

//CLEAR_BUTTON

int ClearState = 0;

//Bank Button Latching
long time = 0;
int BankButtonState = LOW;
int PreviousBankButtonState = LOW;
int BankState[6] = {LOW, LOW, LOW, LOW, LOW, LOW};

int BankClear = 0;

//SHIFT KNOB

int ShiftAmount = 0;
int BankPush[6] = {0, 0, 0, 0, 0, 0};
int BankArrayShift[6] = {0, 0, 0, 0, 0, 0};
int Shift[6] = {0, 0, 0, 0, 0, 0};
int OldShiftAmount = 0;


int looper = 0;
int channel = 0;
int ClockState = 0;            //clock state stuff
int StepLength = 0;           //What the pot uses for step length
int steps = 0;              //beginning number of the steps in the sequence adjusted by StepLength

int BankLED = 18;
int outs[6] = {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4, CHANNEL5, CHANNEL6};



int Sequence[14][43] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},


  //sequence array 1

};//sequence array 1




void setup()
{

  pinMode(CHANNEL1, OUTPUT);
  pinMode(CHANNEL2, OUTPUT);
  pinMode(CHANNEL3, OUTPUT);
  pinMode(CHANNEL4, OUTPUT);
  pinMode(CHANNEL5, OUTPUT);
  pinMode(CHANNEL6, OUTPUT);
  pinMode(BankLED, OUTPUT);

  pinMode(CLOCK, INPUT);
  pinMode(BIG_BUTTON, INPUT);
  pinMode(DELETE_BUTTON, INPUT);
  pinMode(CLEAR_BUTTON, INPUT);
  pinMode(BANK_BUTTON, INPUT);
  pinMode(RESET_BUTTON, INPUT);
  pinMode(FILL_BUTTON, INPUT);

  activeChannel();

  digitalWrite(BankLED, HIGH);
  delay(200);
  digitalWrite(BankLED, LOW);
  delay(180);
  digitalWrite(BankLED, HIGH);
  delay(160);
  digitalWrite(BankLED, LOW);
  delay(140);
  digitalWrite(BankLED, HIGH);
  delay(120);
  digitalWrite(BankLED, LOW);
  delay(100);
  digitalWrite(BankLED, HIGH);
  delay(80);
  digitalWrite(BankLED, LOW);
  delay(60);
  digitalWrite(BankLED, HIGH);
  delay(40);
  digitalWrite(BankLED, LOW);
  delay(20);
  digitalWrite(BankLED, HIGH);
  delay(60);
  digitalWrite(BankLED, LOW);

  attachInterrupt(0, clockInterrupt, RISING);
  attachInterrupt(1, bankInterrupt, CHANGE);
}



void loop() {

  RecordButtonState = digitalRead(BIG_BUTTON);
  DeleteButtonState = digitalRead(DELETE_BUTTON);
  ClearButtonState = digitalRead(CLEAR_BUTTON);
  ResetButtonState = digitalRead(RESET_BUTTON);
  FillButtonState = digitalRead(FILL_BUTTON);


  if (clock == HIGH) {
    time = millis();
    ++looper;
    ++ClockKeep;
    for (int i = 0; i < CHANNELS; ++i) {
      BankPush[i] = BankPush[i] + 1;
      digitalWrite(outs[i], Sequence[i + BankArrayShift[i]][BankPush[i] + Shift[i]] || (Fill[i]));
    }
    clock = LOW;
  } else {
    if (millis() - time > TRIGGER) {
      for (int i = 0; i < CHANNELS; ++i) digitalWrite(outs[i], LOW);
    }
    looper = looper;
    ClockKeep = ClockKeep;
  }

  //RECORD BUTTON
  if (RecordButtonState != LastRecordButtonState && RecordButtonState == HIGH) {
    Sequence[channel + BankArrayShift[channel]][BankPush[channel] + 1 + Shift[channel]] = 1;
  }

  //This bit is the clock in and step advance stuff
  if ((ClockKeep == 1) || (ClockKeep == 5) || (ClockKeep == 9)  || (ClockKeep == 13) ||  (ClockKeep == 17)  || (ClockKeep == 21) || (ClockKeep == 25) || (ClockKeep == 29)) digitalWrite(BankLED, BankArrayShift[channel] == 0);
  else digitalWrite(BankLED, BankState[channel]);


  // Determine Channel
  activeChannel();

  // Determine shift
  shiftAmount();

  // this bit chooses how long the sequence is
  numberOfSteps();

  //lots of the shit with that shift knob
  if (abs(ShiftAmount - OldShiftAmount) > TOLERANCE) {
    Shift[channel] = ShiftAmount;
    OldShiftAmount = ShiftAmount;
  }

  // Switch for selecting between both channels and banks
  BankArrayShift[channel] = BankState[channel] == LOW ? 0 : CHANNELS;

  //This is the clear button
  if (ClearButtonState == HIGH) {
    for (int i = 1; i < 42; i++) Sequence[channel + BankArrayShift[channel]][i] = 0;
  }

  // This is the FILL button
  for (int i = 0; i < CHANNELS; ++i) {
    if (i == channel) Fill[i] = FillButtonState;
    else Fill[i] = LOW;
  }

  // This is the delete button
  if (DeleteButtonState == HIGH) Sequence[channel + BankArrayShift[channel]][looper + 1] = 0;

  if (ResetButtonState != LastResetButtonState && ResetButtonState == HIGH) {
    looper = 0;
    ClockKeep = 0;
    for (int i = 0; i < CHANNELS; ++i) BankPush[i] = 0;
  }

  if (looper >= steps) looper = 0; //this bit starts the sequence over again
  if (ClockKeep >= 32) {
    looper = 0;
    ClockKeep = 0;
  }

  for (int i = 0; i < CHANNELS; ++i) {
    if ((BankPush[i]  + Shift[i]) >= steps) BankPush[i] = 0;
  }

  lastButtonState = clock;
  LastRecordButtonState = RecordButtonState;
  LastResetButtonState = ResetButtonState;//section is for the state change detections
  PreviousBankButtonState = BankButtonState;
}

void activeChannel() {
  channel = map(analogRead(0), 0, 1023, 0 , 5);
}

void numberOfSteps() {
  steps = 1 << map(analogRead(1), 0, 1023, 1, 5);

}

void shiftAmount() {
  ShiftAmount = map(analogRead(2), 0, 1023, 0, steps);
}

void debug() {
  for (int i = 0; i < CHANNELS; ++i) {
    digitalWrite(outs[i], HIGH);
    delay(50);
    digitalWrite(outs[i], LOW);
  }
}

void debugOut(int out) {
  for (int i = 0; i < 5; ++i) {
    digitalWrite(outs[out], HIGH);
    delay(50);
    digitalWrite(outs[out], LOW);
    delay(50);
  }
}

void debugNumber(int number, int out) {
  for (int i = 0; i < number; ++i) {
    digitalWrite(outs[out], HIGH);
    delay(200);
    digitalWrite(outs[out], LOW);
    delay(200);
  }
  delay(2000);
}

void clockInterrupt() {
  clock = HIGH;
}

void bankInterrupt() {
  BankButtonState = digitalRead(BANK_BUTTON);
  if (BankButtonState == HIGH && PreviousBankButtonState == LOW) {
    if (BankState[channel] == HIGH) BankState[channel] = LOW;
    else BankState[channel] = HIGH;
  }
}

