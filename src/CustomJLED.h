// JLed custom HAL example.
// Copyright 2019 by Jan Delgado. All rights reserved.
// https://github.com/jandelgado/jled

// we include jled_base.h instead of "jled.h" since we define our own JLed
// class using our custom HAL.
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <jled_base.h>
// a custom HAL for the using PCA9865 PWM breakout board, In general, a JLed HAL
// class must satisfy the following interface:
//
// class JledHal {
//   public:
//     JledHal(PinType pin);
//     void analogWrite(uint8_t val) const;
//     uint32_t millis() const;
//  }
//

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
class PCA9865Hal {
public:
  using PinType = uint8_t;

  explicit PCA9865Hal(PinType pin) noexcept : pin_(pin) {}

  void analogWrite(uint8_t val) const {
    // some platforms, e.g. STM need lazy initialization
    if (!setup_) {
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

namespace jled {
class JLed : public TJLed<PCA9865Hal, JLed> {
  using TJLed<PCA9865Hal, JLed>::TJLed;
};

// a group of JLed objects which can be controlled simultanously
class JLedSequence : public TJLedSequence<JLed, JLedSequence> {
  using TJLedSequence<JLed, JLedSequence>::TJLedSequence;
};

}; // namespace jled

using JLed = jled::JLed;
using JLedSequence = jled::JLedSequence;

// class JledHal : public jled::TJLed<PCA9865Hal, JLed>
// {
//     using jled::TJLed<PCA9865Hal, JLed>::TJLed;
// };
// //     class JLed : public TJLed<JLedHalType, JLed> {
// //     using TJLed<JLedHalType, JLed>::TJLed;
// // };

// // a group of JLed objects which can be controlled simultanously
// // class JLedSequence : public TJLedSequence<JLed, JLedSequence>
// // {
// //     using TJLedSequence<JLed, JLedSequence>::TJLedSequence;
// // };
