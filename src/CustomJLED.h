// JLed custom HAL example.
// Copyright 2019 by Jan Delgado. All rights reserved.
// https://github.com/jandelgado/jled

// we include jled_base.h instead of "jled.h" since we define our own JLed
// class using our custom HAL.
#include <jled_base.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// a custom HAL for the Arduino, inverting output and ticking with half
// the speed. In general, a JLed HAL class must satisfy the following
// interface:
//
// class JledHal {
//   public:
//     JledHal(PinType pin);
//     void analogWrite(uint8_t val) const;
//     uint32_t millis() const;
//  }
//
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
    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
    mutable bool setup_ = false;
    PinType pin_;
};

class JLed : public jled::TJLed<CustomHal, JLed>
{
    using jled::TJLed<CustomHal, JLed>::TJLed;
};