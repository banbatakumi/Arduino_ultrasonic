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
      for (uint8_t i = 0; i < IR_QTY; i++) {
            unit_vector_x[i] = cos((i * 360.00000 / IR_QTY + 45) * PI / 180.00000);
            unit_vector_y[i] = sin((i * 360.00000 / IR_QTY + 45) * PI / 180.00000);
      }
}

void Ir::Read() {
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
            val[i] = (READ_NUM_OF_TIME - val[i]) * (100.00000 / READ_NUM_OF_TIME);
            result_vector_x += val[i] * unit_vector_x[i];
            result_vector_y += val[i] * unit_vector_y[i];
      }
      dir = atan2(result_vector_y, result_vector_x) / PI * 180.000000;
      dis = sqrt(pow(result_vector_x, 2) + pow(result_vector_y, 2)) * 1.8;
      if (dis > 100) dis = 100;

      DirAve.Compute(&dir);
      DisAve.Compute(&dis);
}

int16_t Ir::GetDir() {
      return dir;
}

uint8_t Ir::GetDis() {
      return dis;
}