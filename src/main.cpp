#include "CustomJLED.h"
#include "OneButton.h"
#include "pitches.h"
#include <Arduino.h>
#include <YA_FSM.h>

//#ifdef UNO
  // #include <stdinout.h>
//#endif
//#ifdef ESP32
 //#include <stdio.h>
//#endif


YA_FSM stateMachine;

const byte RED_BUTTON = 8;
const byte GREEN_BUTTON = 9;
const byte BLUE_BUTTON = 10;
const byte YELLOW_BUTTON = 11;
const byte RED_LED = 0;
const byte GREEN_LED = 1;
const byte YELLOW_LED = 2;
const byte BLUE_LED = 3;
const byte SPEAKER_PIN = 5;
OneButton redButton(RED_BUTTON, true);
OneButton greenButton(GREEN_BUTTON, true);
OneButton blueButton(BLUE_BUTTON, true);
OneButton yellowButton(YELLOW_BUTTON, true);

OneButton buttons[] = {redButton, greenButton, blueButton, yellowButton};

enum State { HOLDING, RED, GREEN, BLUE, YELLOW };

// Helper for print labels instead integer when state change
const char *const stateName[] PROGMEM = {"HOLDING", "RED", "GREEN", "BLUE","YELLOW"};

// Button Triggers
bool redActive = false;
bool greenActive = false;
bool blueActive = false;
bool yellowActive = false;
bool holdingActive = true;

int melody[] = {

    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

    4, 8, 8, 4, 4, 4, 4, 4};

//FSM_State *previousState;

JLed defaultPattern[] = {
    JLed(RED_LED).Breathe(250, 1000, 250).DelayAfter(1000).Repeat(5),
    JLed(GREEN_LED).Breathe(250, 1000, 250).DelayAfter(1000).Repeat(5),
    JLed(YELLOW_LED).Breathe(250, 1000, 250).DelayAfter(1000).Repeat(5),
    JLed(BLUE_LED).Breathe(250, 1000, 250).DelayAfter(1000).Repeat(5),
};

int chasePosition[] = {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED};

JLed chaseTo[] = {
    JLed(chasePosition[0]).Breathe(100, 500, 100).DelayBefore(100),
    JLed(chasePosition[1]).Breathe(100, 500, 100).DelayBefore(400),
    JLed(chasePosition[2]).Breathe(100, 500, 100).DelayBefore(700),
    JLed(chasePosition[3]).Breathe(100, 500, 100).DelayBefore(1000),
};

JLed redPattern[] = {
    JLed(RED_LED).Candle().Forever(),
    JLed(GREEN_LED).Blink(300, 300).Repeat(2),
    JLed(YELLOW_LED).Blink(300, 300).Repeat(2),
    JLed(BLUE_LED).Blink(300, 300).Repeat(2),
};

JLed greenPattern[] = {
    JLed(RED_LED).Blink(1000, 500).Forever(),
    JLed(GREEN_LED).Blink(1000, 500).Forever(),
    JLed(YELLOW_LED).Blink(1000, 500).Forever(),
    JLed(BLUE_LED).Blink(1000, 500).Forever(),
};

JLed bluePattern[] = {
    JLed(RED_LED).Blink(1000, 500).Forever(),
    JLed(GREEN_LED).Blink(1000, 500).Forever(),
    JLed(YELLOW_LED).Blink(1000, 500).Forever(),
    JLed(BLUE_LED).Blink(1000, 500).Forever(),
};

JLed yellowPattern[] = {
    JLed(RED_LED).Blink(1000, 500).Forever(),
    JLed(GREEN_LED).Blink(1000, 500).Forever(),
    JLed(YELLOW_LED).Blink(1000, 500).Forever(),
    JLed(BLUE_LED).Blink(1000, 500).Forever(),
};

auto sequence =
    JLedSequence(JLedSequence::eMode::PARALLEL, defaultPattern).Repeat(3);

void playMelody(int melody[], int noteDurations[], byte pinNumber) {
  int tuneLength = 8;//sizeof(melody) / sizeof(melody[0]);
  for (int thisNote = 0; thisNote < tuneLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pinNumber, melody[thisNote], noteDurations[thisNote]);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pinNumber);
  }
}

void rotate(int firstLed) {

  int *startPointer = chasePosition;
  //printf("orignal %d \n", *chasePosition);
  //printf("Starter Pointer: %d \n", *startPointer);
  while (*startPointer != firstLed) {
    startPointer++;
    //printf("Starter Pointer now: %d %d \n", *startPointer, startPointer);
  }
  // int len = sizeOf(chasePosition)/sizeOf(chasePosition[0]);//

  int n = 4;
  int i = &chasePosition[4] - startPointer;

  while (n--) {
    int chaser = i++ % 4;
    //printf("position %d \n ", chasePosition[chaser]);
    chaseTo[n] = JLed(chasePosition[chaser])
                     .Breathe(100, 500, 100)
                     .DelayBefore(n * 200 + 100);
    //printf("index: %d chaser: %d chasePosition %d\n", n, chaser, chasePosition[chaser]);
  }
  Serial.print("\n");
}

void onStateHolding() {

//   const char *stateName = stateMachine.ActiveStateName();
  // Serial.println(stateName);
}

void onStateBlink() {
    if ( stateMachine.CurrentState()->timeout ) {
        //printf("Timeout!");
        holdingActive = true;
   }

}

void onEntering() {
  int state = stateMachine.GetState();
  Serial.println("Entering State");

  switch (state) {
  case BLUE:
  
        sequence = JLedSequence(JLedSequence::eMode::PARALLEL, bluePattern).Repeat(5);
      sequence.Reset();
    break;
  case RED:
    Serial.println("RED SEQUENCE");
    sequence =
        JLedSequence(JLedSequence::eMode::PARALLEL, redPattern).Repeat(5);
    sequence.Reset();
    break;
  case YELLOW:
    sequence =
        JLedSequence(JLedSequence::eMode::PARALLEL, yellowPattern).Repeat(5);
    sequence.Reset();
    break;
  case GREEN:
        sequence = JLedSequence(JLedSequence::eMode::PARALLEL, greenPattern).Repeat(5);
      sequence.Reset();

    break;
  default:
  
       // sequence = JLedSequence(JLedSequence::eMode::PARALLEL, defaultPattern).Repeat(3);
      //sequence.Reset();
    break;
  }
}

void onLeaving() {
  Serial.println("Leaving State");
  int nextState = stateMachine.CurrentState()->nextState->index;
  // rotate(nextState);
}

void handleClick(enum State buttonId) {
  // Serial.println(stateName[buttonId]);
  //printf("handle click %d", buttonId);
  holdingActive = false;
  switch (buttonId) {
  case BLUE:
    blueActive = true;
    break;
  case RED:
    redActive = true;
    break;
  case YELLOW:
    yellowActive = true;
    break;
  case GREEN:
    greenActive = true;
    break;
  default:
  holdingActive = true;
    break;
  }
}

void setupButtons() {
  // void * red = &RED;
  // void * green = &GREEN;
  // void * blue = &BLUE;
  // void * yellow = &YELLOW;

  redButton.attachClick([]() {
    //printf("clicked");
    handleClick(RED);
  });
  greenButton.attachClick([]() { handleClick(GREEN); });
  blueButton.attachClick([]() { handleClick(BLUE); });
  yellowButton.attachClick([]() { handleClick(YELLOW); });
}

void setupStateMachine() {
  stateMachine.AddState(stateName[HOLDING], 0, onEntering, onStateHolding,
                        onLeaving);
  stateMachine.AddState(stateName[RED], 3000, onEntering, onStateBlink, onLeaving);
  stateMachine.AddState(stateName[GREEN], 3000, onEntering, onStateBlink,
                        onLeaving);
  stateMachine.AddState(stateName[BLUE], 3000, onEntering, onStateBlink,
                        onLeaving);
  stateMachine.AddState(stateName[YELLOW], 3000, onEntering, onStateBlink,
                        onLeaving);

  // Add transitions with related "trigger" callback functions
  stateMachine.AddTransition(HOLDING, RED, redActive);
  stateMachine.AddTransition(HOLDING, GREEN, greenActive);
  stateMachine.AddTransition(HOLDING, BLUE, blueActive);
  stateMachine.AddTransition(HOLDING, YELLOW, yellowActive);
  stateMachine.AddTransition(RED, HOLDING, holdingActive);
  stateMachine.AddTransition(YELLOW, HOLDING, holdingActive);
  stateMachine.AddTransition(GREEN, HOLDING, holdingActive);
  stateMachine.AddTransition(BLUE, HOLDING, holdingActive);
  // Reset the state trigger variable so we don't return
  // to this state without another button press
//  stateMachine.AddAction(RED, YA_FSM::R, redActive);
  stateMachine.AddAction(GREEN, YA_FSM::R, greenActive);
  stateMachine.AddAction(BLUE, YA_FSM::R, blueActive);
  stateMachine.AddAction(YELLOW, YA_FSM::R, yellowActive);
}

void setup() {

  Serial.begin(115200);
  setupButtons();
  setupStateMachine();
  //pinMode(12, INPUT_PULLUP);
  //printf("Setup Started");
  playMelody(melody, noteDurations, SPEAKER_PIN);
//  rotate(RED_LED);
  //rotate(BLUE_LED);
  Serial.println(stateMachine.ActiveStateName());
}
/*
void buttonTick() {
  for (size_t i = 0; i < (4); i++) {
    //printf("button %d", i);
    buttons[i].tick();
  }
}
*/
//int now, delayT;

void loop() {
  // previousState = stateMachine.CurrentState();
  // Serial.println(digitalRead(12));
  // redActive = (digitalRead(RED_BUTTON) == LOW);
  // greenActive = (digitalRead(GREEN_BUTTON) == LOW);
  // blueActive = (digitalRead(BLUE_BUTTON) == LOW);
  // yellowActive = (digitalRead(YELLOW_BUTTON) == LOW);
  // buttonTick();
  /*
  if (now == NULL) {
    //printf("now null");
    now = millis();
    delayT = millis();
  } else if (now - delayT > 5000) {
    redButton.tick(true);
    //        	//printf("now %d\n", now);
    now = millis();
    if (now - delayT > 6000) {
      delayT = millis();
    }
  } else {
    now = millis();
  }*/
  redButton.tick();
  greenButton.tick();
  blueButton.tick();
  yellowButton.tick();
  sequence.Update();

  if (stateMachine.Update()) {
    Serial.print(F("Active state: "));
    Serial.println(stateMachine.ActiveStateName());
  }
}
