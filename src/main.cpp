#include <Arduino.h>

#include "ultrasonic.h"

Ultrasonic dis(14, 15, 17, 16, 12, 10, 7, 6);

const uint8_t ir_pin[4] = {8, 13, 11, 4};
uint16_t ir_val[4];

void setup() {
      Serial.begin(115200);
      for (uint8_t i = 0; i < 4; i++) {
            pinMode(ir_pin[i], INPUT);
      }
}

void loop() {
      dis.Read();
      // UART送信
      uint8_t send_byte_num = 6;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = dis.GetVal(0);
      send_byte[2] = dis.GetVal(1);
      send_byte[3] = dis.GetVal(2);
      send_byte[4] = dis.GetVal(3);
      send_byte[5] = 0xAA;
      for (uint8_t i = 0; i < send_byte_num; i++) {
            Serial.write(send_byte[i]);
      }
      /*
      for (uint8_t i = 0; i < 4; i++) {
            ir_val[i] = 0;
            for (uint16_t j = 0; j < 500; j++) {
                  ir_val[i] += digitalRead(ir_pin[i]);
            }
      }

      Serial.print(dis.GetVal(0));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(1));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(2));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(3));
      Serial.print(" cm, ");
      Serial.print(ir_val[0]);
      Serial.print(", ");
      Serial.print(ir_val[1]);
      Serial.print(", ");
      Serial.print(ir_val[2]);
      Serial.print(", ");
      Serial.println(ir_val[3]);*/
}