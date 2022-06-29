#include <Arduino.h>
#include <YA_FSM.h>

YA_FSM stateMachine;

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
	setupStateMachine();
	// put your setup code here, to run once:
}

void loop()
{
	// put your main code here, to run repeatedly:
}

void onStateBlink()
{
	const char *stateName = stateMachine.ActiveStateName();
	Serial.println(stateName);
}

void onEntering()
{
	Serial.println("Entering State");
}

void onLeaving()
{
	Serial.println("Leaving State");
}

void setupStateMachine()
{

	// Follow the order of defined enumeration for the state definition (will be used as index)
	// Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb
	stateMachine.AddState(stateName[BlinkRed], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkGreen], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkBlue], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkYellow], 0, onEntering, onStateBlink, onLeaving);

	// Add transitions with related "trigger" callback functions
	// stateMachine.AddTransition(BlinkRed, BlinkGreen, checkButton);
	// stateMachine.AddTransition(BlinkGreen, BlinkBlue, checkButton);
	// // For this transition let'use a bool variable (must be global, because we need fix address)
	// stateMachine.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
	// stateMachine.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
}
