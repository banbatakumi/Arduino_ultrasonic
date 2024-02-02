#include <Arduino.h>
#include <avr/io.h>

#include "TimerOne.h"
#include "ir.h"
#include "ultrasonic.h"

Ultrasonic dis(14, 15, 17, 16, 12, 10, 7, 6);
Ir ir(8, 13, 11, 4);

const uint8_t ir_led[4] = {2, 3, 9, 5};

uint8_t count;
bool high;
bool do_ir_led_on;

void timerFire() {
      if (do_ir_led_on == 1) {
            count++;
            high = 1 - high;
            if (count < 16) {
                  if (high == 1) {
                        if (ir.GetDis() == 0) {
                              PORTD |= 0b00000100;  // 0
                              PORTD |= 0b00001000;  // 1
                              PORTB |= 0b00000010;  // 2
                              PORTD |= 0b00100000;  // 3
                        } else {
                              if (abs(ir.GetDir()) <= 90) PORTD |= 0b00000100;  // 0
                              if (ir.GetDir() >= 0) PORTD |= 0b00001000;        // 1
                              if (abs(ir.GetDir()) >= 90) PORTB |= 0b00000010;  // 2
                              if (ir.GetDir() <= 0) PORTD |= 0b00100000;        // 3
                        }
                  } else {
                        PORTD &= ~0b00000100;  // 0
                        PORTD &= ~0b00001000;  // 1
                        PORTB &= ~0b00000010;  // 2
                        PORTD &= ~0b00100000;  // 3
                  }
            } else {
                  PORTD &= ~0b00000100;  // 0
                  PORTD &= ~0b00001000;  // 1
                  PORTB &= ~0b00000010;  // 2
                  PORTD &= ~0b00100000;  // 3
            }
            if (count > 54) count = 0;
      }
}

void setup() {
      Serial.begin(115200);
      for (uint8_t i = 0; i < 4; i++) {
            pinMode(ir_led[i], OUTPUT);
      }
      Timer1.initialize(12);  // マイクロ秒単位で設定
      Timer1.attachInterrupt(timerFire);
}

void loop() {  // モード指定
      dis.Read();
      ir.Read();

      // UART送信
      uint8_t send_byte_num = 8;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = dis.GetVal(0);
      send_byte[2] = dis.GetVal(1);
      send_byte[3] = dis.GetVal(2);
      send_byte[4] = dis.GetVal(3);
      send_byte[5] = ir.GetDir() / 2 + 90;
      send_byte[6] = ir.GetDis();
      send_byte[7] = 0xAA;
      for (uint8_t i = 0; i < send_byte_num; i++) {
            Serial.write(send_byte[i]);
      }

      if (Serial.available() > 0) do_ir_led_on = Serial.read();

      /*
      Serial.print(dis.GetVal(0));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(1));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(2));
      Serial.print(" cm, ");
      Serial.print(dis.GetVal(3));
      Serial.print(" cm, ");
      Serial.print(ir.GetDir());
      Serial.print(", ");
      Serial.println(ir.GetDis());*/
}