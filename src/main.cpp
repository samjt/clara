#include <Arduino.h>
#include <YA_FSM.h>
YA_FSM myState;


enum State {BLINK1, BLINK2, BLINK3};

// Helper for print labels instead integer when state change
const char * const stateName[] PROGMEM = { "Blink1", "Blink2", "Blink3"};


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

void setupStateMachine(){
  
  // Follow the order of defined enumeration for the state definition (will be used as index)
	//Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb	
	myState.AddState(stateName[BLINK1], 0, onEntering, onStateBlink1, onLeaving);
	myState.AddState(stateName[BLINK2], 0, onEntering, onStateBlink2, onLeaving);
	myState.AddState(stateName[BLINK3], 0, onEntering, onStateBlink3, onLeaving);

	// Add transitions with related "trigger" callback functions	
	myState.AddTransition(BLINK1, BLINK2, checkButton );
	myState.AddTransition(BLINK2, BLINK3, checkButton );
	// For this transition let'use a bool variable (must be global, because we need fix address)
	myState.AddTransition(BLINK3, BLINK1, dummyBoolVar );
}
