#include <Arduino.h>
#include <YA_FSM.h>
#include "OneButton.h"

YA_FSM stateMachine;

const byte RED_BUTTON = 8;
const byte GREEN_BUTTON = 3;
const byte BLUE_BUTTON = 4;
const byte YELLOW_BUTTON = 5;

OneButton redButton(RED_BUTTON, true);
OneButton greenButton(GREEN_BUTTON, true);
OneButton blueButton(BLUE_BUTTON, true);
OneButton yellowButton(YELLOW_BUTTON, true);

OneButton buttons[] = {redButton, greenButton, blueButton, yellowButton};

enum State
{
	HOLDING,
	RED,
	GREEN,
	BLUE,
	YELLOW
};

// Helper for print labels instead integer when state change
const char *const stateName[] PROGMEM = {"HOLDING", "RED", "GREEN", "BLUE", "YELLOW"};

// Button Triggers
bool redActive = false;
bool greenActive = false;
bool blueActive = false;
bool yellowActive = false;


FSM_State *previousState;

void onStateHolding()
{

	// const char *stateName = stateMachine.ActiveStateName();
	// Serial.println(stateName);
}

void onStateBlink()
{
	// const char *stateName = stateMachine.ActiveStateName();
	// Serial.println(stateName);
}

void onEntering()
{
	Serial.println("Entering State");
}

void onLeaving()
{
	Serial.println("Leaving State");
}

void handleClick(enum State buttonId)
{
	// Serial.println(stateName[buttonId]);
	switch (buttonId)
	{
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
		break;
	}
}

void setupButtons()
{
	int red = RED;
	int green = GREEN;
	int blue = BLUE;
	int yellow = YELLOW;
	redButton.attachClick(handleClick, red);
	greenButton.attachClick(handleClick, green);
	blueButton.attachClick(handleClick, blue);
	yellowButton.attachClick(handleClick, yellow);
}

void setupStateMachine()
{
	stateMachine.AddState(stateName[HOLDING], 0, onEntering, onStateHolding, onLeaving);
	stateMachine.AddState(stateName[RED], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[GREEN], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[BLUE], 0, onEntering, onStateBlink, onLeaving);
	stateMachine.AddState(stateName[YELLOW], 0, onEntering, onStateBlink, onLeaving);

	// Add transitions with related "trigger" callback functions
	stateMachine.AddTransition(HOLDING, RED, redActive);
	stateMachine.AddTransition(HOLDING, GREEN, greenActive);
	stateMachine.AddTransition(HOLDING, BLUE, blueActive);
	stateMachine.AddTransition(HOLDING, YELLOW, yellowActive);
	// stateMachine.AddTransition(RED, HOLDING, nullptr);
}

void setup()
{
	// pinMode(RED_BUTTON, INPUT_PULLUP);
	// pinMode(GREEN_BUTTON, INPUT_PULLUP);
	// pinMode(BLUE_BUTTON, INPUT_PULLUP);
	// pinMode(YELLOW_BUTTON, INPUT_PULLUP);
	Serial.begin(115200);
	setupButtons();
	setupStateMachine();
	Serial.println("Setup");
	Serial.println(stateMachine.ActiveStateName());

	// put your setup code here, to run once:
}

void buttonTick()
{
	for (size_t i = 0; i < (sizeof(buttons) / sizeof(OneButton)); i++)
	{
		/* code */

		buttons[i].tick();
	}
}

void loop()
{
	previousState = stateMachine.CurrentState();

	// redActive = (digitalRead(RED_BUTTON) == LOW);
	// greenActive = (digitalRead(GREEN_BUTTON) == LOW);
	// blueActive = (digitalRead(BLUE_BUTTON) == LOW);
	// yellowActive = (digitalRead(YELLOW_BUTTON) == LOW);
	// buttonTick();
	redButton.tick();
	greenButton.tick();
	blueButton.tick();
	yellowButton.tick();

	// Serial.print("redActive");
	// Serial.print(redActive);
	if (stateMachine.Update())
	{
		Serial.print(F("Active state: "));
		Serial.println(stateMachine.ActiveStateName());
	}
}