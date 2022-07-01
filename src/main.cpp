#include <Arduino.h>
#include <YA_FSM.h>

YA_FSM stateMachine;

const byte RED_BUTTON = 2;
const byte GREEN_BUTTON = 3;
const byte BLUE_BUTTON = 4;
const byte YELLOW_BUTTON = 5;

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

// Button Triggers
bool redActive = false;
bool greenActive = false;
bool blueActive = false;
bool yellowActive = false;

void setupStateMachine()
{
	stateMachine.AddState(stateName[DefaultPattern], 0, onEntering, onStateDefault, onLeaving);
	stateMachine.AddState(stateName[BlinkRed], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkGreen], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkBlue], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BlinkYellow], 0, onEntering, onStateBlink, onLeaving);

	// Add transitions with related "trigger" callback functions
	stateMachine.AddTransition(DefaultPattern, BlinkRed, redActive);
	stateMachine.AddTransition(DefaultPattern, BlinkGreen, greenActive);
	stateMachine.AddTransition(DefaultPattern, BlinkBlue, blueActive);
	stateMachine.AddTransition(DefaultPattern, BlinkYellow, yellowActive);
	// stateMachine.AddTransition(BlinkGreen, BlinkBlue, checkButton);
	// // For this transition let'use a bool variable (must be global, because we need fix address)
	// stateMachine.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
	// stateMachine.AddTransition(BlinkBlue, BlinkRed, dummyBoolVar);
}

void setup()
{
	pinMode(RED_BUTTON, INPUT_PULLUP);
	pinMode(GREEN_BUTTON, INPUT_PULLUP);
	pinMode(BLUE_BUTTON, INPUT_PULLUP);
	pinMode(YELLOW_BUTTON, INPUT_PULLUP);
	Serial.begin(115200);

	setupStateMachine();
	// put your setup code here, to run once:
}

void loop()
{
	redActive = (digitalRead(RED_BUTTON) == LOW);
	greenActive = (digitalRead(GREEN_BUTTON) == LOW);
	blueActive = (digitalRead(BLUE_BUTTON) == LOW);
	yellowActive = (digitalRead(YELLOW_BUTTON) == LOW);
	if (stateMachine.Update())
	{
		Serial.print(F("Active state: "));
		Serial.println(stateMachine.ActiveStateName());
	}
}

void onStateDefault()
{
	const char *stateName = stateMachine.ActiveStateName();
	Serial.println(stateName);
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

bool transitionState()
{
	const char *stateName = stateMachine.ActiveStateName();
	Serial.println(stateName);
	return true;
}
