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
#define BIG_LED 18
#define BIG_BUTTON 19
#define CHANNEL1 8
#define CHANNEL2 9
#define CHANNEL3 10
#define CHANNEL4 11
#define CHANNEL5 12
#define CHANNEL6 13

byte buttonPushCounter = 0;   // counter for the number of button presses
volatile byte clock = LOW;         // current state of the button
byte lastButtonState = 0;     // previous state of the button
byte RecordButtonState = 0;
byte LastRecordButtonState = 0;
byte DeleteButtonState = 0;
byte LastDeleteButtonState = 0;
byte ClearButtonState = 0;


//RESET BUTTON
byte ResetButtonState = LOW;
byte LastResetButtonState = LOW;

//FILL BUTTON
byte FillButtonState = 0;

//CLEAR_BUTTON

byte ClearState = 0;

//Bank Button Latching
long time = 0;
byte BankButtonState = LOW;
byte PreviousBankButtonState = LOW;
byte BankState[6] = {LOW, LOW, LOW, LOW, LOW, LOW};

byte BankClear = 0;

//SHIFT KNOB

byte ShiftAmount = 0;
byte BankArrayShift[6] = {0, 0, 0, 0, 0, 0};
byte Shift[6] = {0, 0, 0, 0, 0, 0};
byte OldShiftAmount = 0;


byte current = 0;
byte next = 1;

byte channel = 0;
byte ClockState = 0;            //clock state stuff
byte StepLength = 0;           //What the pot uses for step length
byte steps = 0;              //beginning number of the steps in the sequence adjusted by StepLength

byte outs[6] = {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4, CHANNEL5, CHANNEL6};

byte sequence[12][64] = {};

void setup() {
  memset(sequence,0,sizeof(sequence));

  pinMode(CHANNEL1, OUTPUT);
  pinMode(CHANNEL2, OUTPUT);
  pinMode(CHANNEL3, OUTPUT);
  pinMode(CHANNEL4, OUTPUT);
  pinMode(CHANNEL5, OUTPUT);
  pinMode(CHANNEL6, OUTPUT);
  pinMode(BIG_LED, OUTPUT);

  pinMode(CLOCK, INPUT);
  pinMode(BIG_BUTTON, INPUT);
  pinMode(DELETE_BUTTON, INPUT);
  pinMode(CLEAR_BUTTON, INPUT);
  pinMode(BANK_BUTTON, INPUT);
  pinMode(RESET_BUTTON, INPUT);
  pinMode(FILL_BUTTON, INPUT);

  activeChannel();

  startUp();

  attachInterrupt(0, clockInterrupt, RISING);
  attachInterrupt(1, bankInterrupt, CHANGE);
}


void loop() {

  RecordButtonState = digitalRead(BIG_BUTTON);
  DeleteButtonState = digitalRead(DELETE_BUTTON);
  ClearButtonState = digitalRead(CLEAR_BUTTON);
  ResetButtonState = digitalRead(RESET_BUTTON);
  FillButtonState = digitalRead(FILL_BUTTON);
  
  activeChannel();
  shiftAmount();
  numberOfSteps();
  
  Shift[channel] = ShiftAmount;
  BankArrayShift[channel] = BankState[channel] == LOW ? 0 : CHANNELS;
  if (ResetButtonState != LastResetButtonState && ResetButtonState == HIGH) next = 0;
  if (DeleteButtonState == HIGH) sequence[channel + BankArrayShift[channel]][next] = 0;
  if (RecordButtonState != LastRecordButtonState && RecordButtonState == HIGH) sequence[channel + BankArrayShift[channel]][next + Shift[channel]] = 1;

  if (clock == HIGH) {
    
    time = millis();
    
    current = next;
    ++next;
    if (next >= steps) next = 0;
    
    if (current % 16 == 0) digitalWrite(BIG_LED, BankArrayShift[channel] == 0);
    else digitalWrite(BIG_LED, BankState[channel]);
    
    for (int i = 0; i < CHANNELS; ++i) {
      digitalWrite(outs[i], sequence[i + BankArrayShift[i]][current + Shift[i]] || (i == channel) && FillButtonState);
    }
    
    clock = LOW;
  } else {
    if (millis() - time > TRIGGER) {
      for (int i = 0; i < CHANNELS; ++i) digitalWrite(outs[i], LOW);
    }
  }



  //This is the clear button
  if (ClearButtonState == HIGH) {
    for (int i = 0; i < 64; i++) sequence[channel + BankArrayShift[channel]][i] = 0;
  }

  // This is the delete button


  lastButtonState = clock;
  LastRecordButtonState = RecordButtonState;
  LastResetButtonState = ResetButtonState;
  PreviousBankButtonState = BankButtonState;
}

void activeChannel() {
  channel = map(analogRead(0), 0, 1023, 0 , 5);
}

void numberOfSteps() {
  steps = 1 << map(analogRead(1), 0, 1023, 2, 6);
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


void startUp() {
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

