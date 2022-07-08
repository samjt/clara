#include <Arduino.h>
#include <YA_FSM.h>
#include "OneButton.h"
//#include <jled_base.h>
//#include <Wire.h>
//#include <Adafruit_PWMServoDriver.h>
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
/*
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

class CustomHal
{
public:
	using PinType = uint8_t;

	explicit CustomHal(PinType pin) noexcept : pin_(pin)
	{
	}

	void analogWrite(uint8_t val) const
	{
		// some platforms, e.g. STM need lazy initialization
		if (!setup_)
		{
			pwm.begin();
			pwm.setPWMFreq(1600);
			Wire.setClock(400000);
			setup_ = true;
		}
		uint8_t mapped = map(val, 0, 255, 0, 4095);
		pwm.setPWM(pin_, 0, mapped);
	}

	uint32_t millis() const { return ::millis(); }

private:
	mutable bool setup_ = false;
	PinType pin_;
};

class JLed : public jled::TJLed<CustomHal, JLed>
{
	using jled::TJLed<CustomHal, JLed>::TJLed;
};
*/
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
   JLed(RED_LED).Breathe(1000, 500).Forever();
JLed(GREEN_LED).Breathe(1000, 500).Forever();
JLed(YELLOW_LED).Blink(1000, 500).Forever();
JLed(BLUE_LED).Breathe(1000, 500).Forever();

  };
  JLed redPattern[] = {
JLed(RED_LED).Candle().Forever();
JLed(GREEN_LED).Blink(300, 300).Repeat(2);
JLed(YELLOW_LED).Blink(300, 300).Repeat(2);
JLed(BLUE_LED).Blink(300, 300).Repeat(2);
  };
 JLed greenPattern[] = {
JLed(RED_LED).Blink(1000, 500).Forever();
JLed(GREEN_LED).Blink(1000, 500).Forever();
JLed(YELLOW_LED).Blink(1000, 500).Forever();
JLed(BLUE_LED).Blink(1000, 500).Forever();
  };
  
   JLed bluePattern[] = {
JLed(RED_LED).Blink(1000, 500).Forever();
JLed(GREEN_LED).Blink(1000, 500).Forever();
JLed(YELLOW_LED).Blink(1000, 500).Forever();
JLed(BLUE_LED).Blink(1000, 500).Forever();
  };

 JLed yellowPattern[] = {
JLed(RED_LED).Blink(1000, 500).Forever();
JLed(GREEN_LED).Blink(1000, 500).Forever();
JLed(YELLOW_LED).Blink(1000, 500).Forever();
JLed(BLUE_LED).Blink(1000, 500).Forever();
  };
  
void onStateHolding()
{

	// const char *stateName = stateMachine.ActiveStateName();
	// Serial.println(stateName);
}

void onStateBlink()
{


	// switch (buttonId)
	// {
	// case BLUE:
	// 	blueActive = true;
	// 	break;
	// case RED:
	// 	redActive = true;
	// 	break;
	// case YELLOW:
	// 	yellowActive = true;
	// 	break;
	// case GREEN:
	// 	greenActive = true;
	// 	break;
	// default:
	// 	break;
	// }
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
	// int red = RED;
	// int green = GREEN;
	// int blue = BLUE;
	// int yellow = YELLOW;

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
	stateMachine.AddTransition(RED, HOLDING, nullptr);
	// Reset the state trigger variable so we don't return
	// to this state without another button press
	stateMachine.AddAction(RED, YA_FSM::R, redActive);
	stateMachine.AddAction(GREEN, YA_FSM::R, greenActive);
	stateMachine.AddAction(BLUE, YA_FSM::R, blueActive);
	stateMachine.AddAction(YELLOW, YA_FSM::R, yellowActive);
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
	led1.Update();
	led2.Update();
	led3.Update();
	led4.Update();

	// Serial.print("redActive");
	// Serial.print(redActive);
	if (stateMachine.Update())
	{
		Serial.print(F("Active state: "));
		Serial.println(stateMachine.ActiveStateName());
	}
}