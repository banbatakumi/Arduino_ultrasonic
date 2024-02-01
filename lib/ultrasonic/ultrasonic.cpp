#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t echo_, uint8_t trig_) {
      echo = echo_;
      trig = trig_;

      pinMode(echo, INPUT);
      pinMode(trig, OUTPUT);
}

void Ultrasonic::Read() {
      digitalWrite(trig, LOW);
      delayMicroseconds(5);
      digitalWrite(trig, HIGH);
      delayMicroseconds(15);
      digitalWrite(trig, LOW);
      duration = pulseIn(echo, HIGH, TIMEOUT);  // 往復にかかった時間が返却される[マイクロ秒]
      duration = duration / 2;                  // 往路にかかった時間
      distance = duration * SPEED_OF_SOUND * 100 / 1000000;
      if (distance > 255) distance = 255;

      rc_distance = rc_distance * RC + distance * (1 - RC);
}

uint8_t Ultrasonic::GetVal() {
      return rc_distance;
}