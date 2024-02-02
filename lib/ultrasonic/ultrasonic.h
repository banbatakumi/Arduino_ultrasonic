#ifndef MBED_ULTRASONIC_H
#define MBED_ULTRASONIC_H

#include "Arduino.h"

#define RC 0.5
#define SPEED_OF_SOUND 340
#define TIMEOUT 100000

class Ultrasonic {
     public:
      Ultrasonic(uint8_t echo_0_, uint8_t trig_0_, uint8_t echo_1_, uint8_t trig_1_,
                 uint8_t echo_2_, uint8_t trig_2_, uint8_t echo_3_, uint8_t trig_3_);
      void Read();
      uint8_t GetVal(uint8_t sensor_num_);

     private:
      uint8_t echo[4], trig[4];

      float duration[4];
      uint16_t distance[4];
      uint8_t rc_distance[4];
      uint8_t count;
};

#endif