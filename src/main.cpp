#include <Arduino.h>
#include <avr/io.h>

#include "TimerOne.h"
#include "fast_digitalwrite.h"
#include "ir.h"
#include "ultrasonic.h"

Ultrasonic dis(14, 15, 17, 16, 12, 10, 7, 6);
Ir ir(8, 13, 11, 4);

const uint8_t ir_led[4] = {2, 3, 9, 5};

uint8_t count;
bool on_ir_led[4];

void timerFire() {
      count++;
      if (count < 16) {
            if (count % 2 == 0) {
                  if (on_ir_led[0]) High(ir_led[0]);
                  if (on_ir_led[1]) High(ir_led[1]);
                  if (on_ir_led[2]) High(ir_led[2]);
                  if (on_ir_led[3]) High(ir_led[3]);
            } else {
                  if (on_ir_led[0]) Low(ir_led[0]);
                  if (on_ir_led[1]) Low(ir_led[1]);
                  if (on_ir_led[2]) Low(ir_led[2]);
                  if (on_ir_led[3]) Low(ir_led[3]);
            }
      } else if (count == 16) {
            Low(ir_led[0]);
            Low(ir_led[1]);
            Low(ir_led[2]);
            Low(ir_led[3]);
      }
      if (count > 54) count = 0;
}

void setup() {
      Serial.begin(115200);
      for (uint8_t i = 0; i < 4; i++) {
            pinMode(ir_led[i], OUTPUT);
      }
      Timer1.initialize(13);  // マイクロ秒単位で設定
      Timer1.attachInterrupt(timerFire);
}

void loop() {  // モード指定
      while (1) {
            dis.Read();
            ir.Read();

            // UART送信
            const uint8_t send_byte_num = 8;
            uint8_t send_byte[send_byte_num];
            send_byte[0] = 0xFF;
            send_byte[1] = dis.GetVal(0);
            send_byte[2] = dis.GetVal(1);
            send_byte[3] = dis.GetVal(2);
            send_byte[4] = dis.GetVal(3);
            send_byte[5] = ir.GetDir() / 2 + 90;
            send_byte[6] = ir.GetDis();
            send_byte[7] = 0xAA;
            Serial.write(send_byte, send_byte_num);

            uint8_t read_byte;
            if ((read_byte = Serial.read()) != -1) {
                  on_ir_led[0] = read_byte & 1;
                  on_ir_led[1] = (read_byte >> 1) & 1;
                  on_ir_led[2] = (read_byte >> 2) & 1;
                  on_ir_led[3] = (read_byte >> 3) & 1;
            }

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
}