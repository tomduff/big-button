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
#define BANK_INTERUPT 1

#define TRIGGER 25

#define TOLERANCE 0
#define CHANNELS 6
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
int ResetButton = 6;
int ResetButtonState = 0;
int LastResetButtonState = 0;

int ChannelSelect = 0;

//FILL BUTTON
int FillButton = 5;
int FillButtonState = 0;
byte Fill[6] = {0, 0, 0, 0, 0, 0};

//CLEAR

int ClearState = 0;

//Bank Button Latching
long time = 0;
int BankButtonState = LOW;
int PreviousBankButtonState = LOW;
int BankState[6] = {LOW, LOW, LOW, LOW, LOW, LOW};

int BankClear = 0;
int ButtonBankSelectButton = 0;

//SHIFT KNOB

int ShiftAmount = 0;
int BankPush[6] = {0, 0, 0, 0, 0, 0};
int BankArrayShift[6] = {0, 0, 0, 0, 0, 0};
int Shift[6] = {0, 0, 0, 0, 0, 0};
int ShiftSteps = 0;
int OldShiftAmount = 0;


int looper = 0;
int Channel = 0;
int ClockState = 0;            //clock state stuff
int StepLength = 0;           //What the pot uses for step length
int steps = 0;              //beginning number of the steps in the sequence adjusted by StepLength
const int clkIn = 2;
int BigButton = 19;

int ButtonClear = 4;         //reset button for the moment
int ButtonDelete = 7;
int ButtonBankSelect = 3;

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

  pinMode(clkIn, INPUT);
  pinMode(BigButton, INPUT);
  pinMode(ButtonDelete, INPUT);
  pinMode(ButtonClear, INPUT);
  pinMode(ButtonBankSelect, INPUT);
  pinMode(ResetButton, INPUT);
  pinMode(FillButton, INPUT);

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

  RecordButtonState = digitalRead(BigButton);
  DeleteButtonState = digitalRead(ButtonDelete);
  ClearButtonState = digitalRead(ButtonClear);
  ResetButtonState = digitalRead(ResetButton);
  FillButtonState = digitalRead(FillButton);


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
    if(millis() - time > TRIGGER) {
      for (int i = 0; i < CHANNELS; ++i) digitalWrite(outs[i], LOW);
    }
    looper = looper;
    ClockKeep = ClockKeep;
  }

  //RECORD BUTTON
  if (RecordButtonState != LastRecordButtonState && RecordButtonState == HIGH) {
    Sequence[Channel + BankArrayShift[Channel]][BankPush[Channel] + 1 + Shift[Channel]] = 1;
  }

  //This bit is the clock in and step advance stuff
  if ((ClockKeep == 1) || (ClockKeep == 5) || (ClockKeep == 9)  || (ClockKeep == 13) ||  (ClockKeep == 17)  || (ClockKeep == 21) || (ClockKeep == 25) || (ClockKeep == 29)) digitalWrite(BankLED, BankArrayShift[Channel] == 0);
  else digitalWrite(BankLED, BankState[Channel]);


  // Determine Channel
  activeChannel();

  // Determine shift
  shiftAmount();

  //lots of the shit with that shift knob
  if (abs(ShiftAmount - OldShiftAmount) > TOLERANCE) {
    Shift[Channel] = ShiftAmount;
    OldShiftAmount = ShiftAmount;
  }

  // Switch for selecting between both channels and banks
  BankArrayShift[Channel] = BankState[Channel] == LOW ? 0 : CHANNELS;

  //This is the clear button
  if (ClearButtonState == HIGH) {
    for (int i = 1; i < 42; i++) Sequence[Channel + BankArrayShift[Channel]][i] = 0;
  }

  //This is the FILL button
  for (int i = 0; i < CHANNELS; ++i) {
    if (i == Channel) Fill[i] = FillButtonState;
    else Fill[i] = LOW;
  }

  // This is the delete button
  if (DeleteButtonState == HIGH) Sequence[Channel + BankArrayShift[Channel]][looper + 1] = 0;

  if (ResetButtonState != LastResetButtonState && ResetButtonState == HIGH) {
    looper = 0;
    ClockKeep = 0;
    for (int i = 0; i < CHANNELS; ++i) BankPush[i] = 0;
  }

  // this bit chooses how long the sequence is
  numberOfSteps();

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
  ChannelSelect = analogRead(0);
  if (ChannelSelect < 150) Channel = 0;
  else if (ChannelSelect < 320) Channel = 1;
  else if (ChannelSelect < 490) Channel = 2;
  else if (ChannelSelect < 660) Channel = 3;
  else if (ChannelSelect < 830) Channel = 4;
  else Channel = 5;
}

void numberOfSteps() {
  StepLength = analogRead(1);
  if (0 < StepLength) steps = 2;
  if (200 < StepLength) steps = 4;
  if (500 < StepLength) steps = 8;
  if (800 < StepLength) steps = 16;
  if (1000 < StepLength) steps = 32;
}

void shiftAmount() {
  ShiftSteps = analogRead(2);
  if (0 < ShiftSteps) ShiftAmount = 0;
  if (127 < ShiftSteps) ShiftAmount = 1;
  if (254 < ShiftSteps) ShiftAmount = 2;
  if (383 < ShiftSteps) ShiftAmount = 3;
  if (511 < ShiftSteps) ShiftAmount = 4;
  if (638 < ShiftSteps) ShiftAmount = 5;
  if (767 < ShiftSteps) ShiftAmount = 6;
  if (895 < ShiftSteps) ShiftAmount = 7;
  if (1000 < ShiftSteps) ShiftAmount = 8;
}

void debug() {
  for(int i=0; i < CHANNELS; ++i) {
    digitalWrite(outs[i], HIGH);
    delay(50);
    digitalWrite(outs[i], LOW);
  }
}

void clockInterrupt() {
  clock = HIGH;
}

void bankInterrupt() {  
  BankButtonState = digitalRead(ButtonBankSelect);
  if (BankButtonState == HIGH && PreviousBankButtonState == LOW) {
    if (BankState[Channel] == HIGH) BankState[Channel] = LOW;
    else BankState[Channel] = HIGH;
  }
}

