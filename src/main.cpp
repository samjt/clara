#include <Arduino.h>
#include <YA_FSM.h>

YA_FSM myState;

enum State
{
	DefaultPattern,
	BlinkRed,
	BlinkGreen,
	BlinkBlue,
	BlinkYellow
};

// Helper for print labels instead integer when state change
const char *const stateName[] PROGMEM = {"DefaultPattern", "BlinkRed", "BlinkGreen", "BlinkBlue", "BlinkYellow"};

void setup()
{
	// put your setup code here, to run once:
}

void loop()
{
	// put your main code here, to run repeatedly:
}

void onStateBlink()
{
	myState.ActiveStateName();
}

void setupStateMachine()
{

	// Follow the order of defined enumeration for the state definition (will be used as index)
	// Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb
	myState.AddState(stateName[BlinkRed], 0, onEntering, onStateBlink, onLeaving);
	myState.AddState(stateName[BlinkGreen], 0, onEntering, onStateBlink, onLeaving);
	myState.AddState(stateName[BlinkBlue], 0, onEntering, onStateBlink, onLeaving);
	myState.AddState(stateName[BlinkYellow], 0, onEntering, onStateBlink, onLeaving);

	// Add transitions with related "trigger" callback functions
	myState.AddTransition(BlinkRed, BlinkGreen, checkButton);
	myState.AddTransition(BlinkGreen, BlinkBlue, checkButton);
	// For this transition let'use a bool variable (must be global, because we need fix address)
	myState.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
	myState.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
}
