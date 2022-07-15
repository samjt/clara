#include <Arduino.h>
#include <YA_FSM.h>
#include "OneButton.h"
#include "CustomJLED.h"
YA_FSM stateMachine;

const byte RED_BUTTON = 8;
const byte GREEN_BUTTON = 3;
const byte BLUE_BUTTON = 4;
const byte YELLOW_BUTTON = 5;
const byte RED_LED = 0;
const byte GREEN_LED = 1;
const byte YELLOW_LED = 2;
const byte BLUE_LED = 3;

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

JLed defaultPattern[] = {
	JLed(RED_LED).Breathe(250, 1000, 250).DelayAfter(1000).Forever(),
	JLed(GREEN_LED).Breathe(250, 1000, 250).DelayAfter(1000).Forever(),
	JLed(YELLOW_LED).Breathe(250, 1000, 250).DelayAfter(1000).Forever(),
	JLed(BLUE_LED).Breathe(250, 1000, 250).DelayAfter(1000).Forever(),
};

byte chasePosition[] = {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED};

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

auto sequence = JLedSequence(JLedSequence::eMode::PARALLEL, defaultPattern).Repeat(5);

void onStateHolding()
{

	// const char *stateName = stateMachine.ActiveStateName();
	// Serial.println(stateName);
}

void onStateBlink()
{
}

void onEntering()
{
	int state = stateMachine.GetState();
	Serial.println("Entering State");
	switch (state)
	{
	case BLUE:
		break;
	case RED:
		Serial.println("RED SEQUENCE");
		sequence = JLedSequence(JLedSequence::eMode::PARALLEL, redPattern).Repeat(5);
		sequence.Reset();
		break;
	case YELLOW:
		sequence = JLedSequence(JLedSequence::eMode::PARALLEL, yellowPattern).Repeat(5);
		sequence.Reset();
		break;
	case GREEN:
		break;
	default:
		break;
	}
}

void onLeaving()
{
	Serial.println("Leaving State");
	int nextState = stateMachine.CurrentState()->nextState->index;
	// rotate(nextState);
}

void rotateLeds(const byte firstLed)
{

	byte *startPointer = chasePosition;
	Serial.println("startPointer initial position");
	Serial.println(*startPointer);
	Serial.println("firstLed");
	Serial.println(firstLed);
	// While the value of start pointer doesnt 
	// match the led we're starting with, increment the pointer
	while (*startPointer != firstLed)
	{
		startPointer++;
		Serial.println("*startPointer after increment");
		Serial.println(*startPointer);
		// Serial.println("*startPointer index");
		// Serial.println(startPointer - chasePosition);
	}
	int len = sizeof(chasePosition) / sizeof(chasePosition[0]);

	Serial.println("len");
	Serial.println(len);
	// JLed chaseTo[4];
	for (int x = 0; x < len; x++)
	{

		Serial.println("*(startPointer in chase)");
		Serial.println(startPointer - chasePosition);
		// Serial.println(*startPointer);
		// if (*(startPointer + 1) <
		// 	(chasePosition[len - 1]))
		// {
		// 	Serial.println("chasePosition[x] now");
		// 	Serial.println(chasePosition[x]);
		// 	Serial.println("startPointer now");
		// 	Serial.println(*startPointer);
		// 	// chaseTo[x] = JLed(startPointer++).Breathe(100, 500, 100).DelayBefore(x * 200 + 100);
		// }
	}
}

void handleClick(enum State buttonId)
{
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
	redButton.attachClick(handleClick, (void *)intptr_t(RED));
	greenButton.attachClick(handleClick, (void *)intptr_t(GREEN));
	blueButton.attachClick(handleClick, (void *)intptr_t(BLUE));
	yellowButton.attachClick(handleClick, (void *)intptr_t(YELLOW));
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
	// Reset the state trigger variable so we don't return
	// to this state without another button press
	// stateMachine.AddAction(RED, YA_FSM::R, redActive);
	stateMachine.AddAction(GREEN, YA_FSM::R, greenActive);
	stateMachine.AddAction(BLUE, YA_FSM::R, blueActive);
	stateMachine.AddAction(YELLOW, YA_FSM::R, yellowActive);
}

void setup()
{

	Serial.begin(115200);
	setupButtons();
	setupStateMachine();
	pinMode(12, INPUT_PULLUP);
	Serial.println("Setup");
	Serial.println(stateMachine.ActiveStateName());
	// Serial.println("Rotate 0");
	// rotateLeds(RED_LED);
	Serial.println("Rotate 2");
	Serial.println(BLUE_LED);
	rotateLeds(BLUE_LED);
	// Serial.println("Rotate 1");
	// rotateLeds(GREEN_LED);
}

void buttonTick()
{
	for (size_t i = 0; i < (sizeof(buttons) / sizeof(OneButton)); i++)
	{
		buttons[i].tick();
	}
}

void loop()
{
	redButton.tick();
	greenButton.tick();
	blueButton.tick();
	yellowButton.tick();
	sequence.Update();

	if (stateMachine.Update())
	{
		Serial.print(F("Active state: "));
		Serial.println(stateMachine.ActiveStateName());
	}
}