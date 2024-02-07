#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echo_0_, uint8_t trig_0_, uint8_t echo_1_, uint8_t trig_1_,
                       uint8_t echo_2_, uint8_t trig_2_, uint8_t echo_3_, uint8_t trig_3_) {
      echo[0] = echo_0_;
      trig[0] = trig_0_;
      echo[1] = echo_1_;
      trig[1] = trig_1_;
      echo[2] = echo_2_;
      trig[2] = trig_2_;
      echo[3] = echo_3_;
      trig[3] = trig_3_;

      for (uint8_t i = 0; i < 4; i++) {
            pinMode(echo[i], INPUT);
            pinMode(trig[i], OUTPUT);
      }
}

void Ultrasonic::Read() {
      digitalWrite(trig[count], LOW);
      delayMicroseconds(2);
      digitalWrite(trig[count], HIGH);
      delayMicroseconds(10);
      digitalWrite(trig[count], LOW);
      duration[count] = pulseIn(echo[count], HIGH, TIMEOUT);  // 往復にかかった時間が返却される[マイクロ秒]
      duration[count] = duration[count] / 2;                  // 往路にかかった時間
      distance[count] = duration[count] * SPEED_OF_SOUND * 100 / 1000000;
      if (distance[count] > 255) distance[count] = 255;
      if (distance[count] == 0) distance[count] = 255;
      count++;
      if (count > 3) count = 0;
}

uint8_t Ultrasonic::GetVal(uint8_t sensor_num_) {
      return distance[sensor_num_];
}