#ifndef MBED_ULTRASONIC_H
#define MBED_ULTRASONIC_H

#include "Arduino.h"

#define RC 0.8
#define SPEED_OF_SOUND 340
#define TIMEOUT 1000000

class Ultrasonic {
     public:
      Ultrasonic(uint8_t echo_, uint8_t trig_);
      void Read();
      uint8_t GetVal();

     private:
      uint8_t echo, trig;

      float duration;
      uint16_t distance;
      uint8_t rc_distance;
};

#endif