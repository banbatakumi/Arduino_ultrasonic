#include <Arduino.h>

#include "ultrasonic.h"

Ultrasonic disFront(14, 15);
Ultrasonic disRight(17, 16);
Ultrasonic disBack(12, 10);
Ultrasonic disLeft(7, 6);

void setup() {
      Serial.begin(9600);
}

void loop() {
      disFront.Read();
      disRight.Read();
      disBack.Read();
      disLeft.Read();

      Serial.print(disFront.GetVal());
      Serial.print(" cm, ");
      Serial.print(disRight.GetVal());
      Serial.print(" cm, ");
      Serial.print(disBack.GetVal());
      Serial.print(" cm, ");
      Serial.print(disLeft.GetVal());
      Serial.print(" cm, ");

      delay(10);
}