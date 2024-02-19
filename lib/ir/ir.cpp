#include "ir.h"

Ir::Ir(uint8_t ir_0_, uint8_t ir_1_, uint8_t ir_2_, uint8_t ir_3_) {
      ir[0] = ir_0_;
      ir[1] = ir_1_;
      ir[2] = ir_2_;
      ir[3] = ir_3_;

      DirAve.SetLength(DIR_MOVING_AVE_NUM);
      DisAve.SetLength(DIS_MOVING_AVE_NUM);

      for (uint8_t i = 0; i < 4; i++) {
            pinMode(ir[i], INPUT);
      }
      for (uint8_t i = 0; i < IR_QTY; i++) {  // 各センサに座標を与える
            unit_vector_x[i] = cos((i * 360.00000 / IR_QTY + 45) * PI / 180.0f);
            unit_vector_y[i] = sin((i * 360.00000 / IR_QTY + 45) * PI / 180.0f);
      }
}

void Ir::Read() {
      // センサそれぞれの値を読む
      for (uint8_t i = 0; i < IR_QTY; i++) val[i] = 0;
      for (uint16_t i = 0; i < READ_NUM_OF_TIME; i++) {
            val[0] += PINB & 0b00000001;         // DigitalRead(ir[0]);
            val[1] += (PINB >> 5) & 0b00000001;  // DigitalRead(ir[1]);
            val[2] += (PINB >> 3) & 0b00000001;  // DigitalRead(ir[2]);
            val[3] += (PIND >> 4) & 0b00000001;  // DigitalRead(ir[3]);
      }

      result_vector_x = 0;
      result_vector_y = 0;
      for (uint8_t i = 0; i < IR_QTY; i++) {
            val[i] = (READ_NUM_OF_TIME - val[i]) * (100.0f / READ_NUM_OF_TIME);  // 100までに補正
            result_vector_x += val[i] * unit_vector_x[i];                        // X成分を合成
            result_vector_y += val[i] * unit_vector_y[i];                        // Y成分を合成
      }

      dir = atan2(result_vector_y, result_vector_x) / PI * 180.0f;          // 角度を出す
      dis = sqrt(pow(result_vector_x, 2) + pow(result_vector_y, 2)) * 1.8;  // 距離を出す
      if (dis > 100) dis = 100;

      // 移動平均を取る
      DirAve.Compute(&dir);
      DisAve.Compute(&dis);
}

int16_t Ir::GetDir() {
      return dir;
}

uint8_t Ir::GetDis() {
      return dis;
}