//
// Created by kfl02 on 27.11.19.
//

#include "arduinosid.h"
#include "sid.h"

SIDArray sidArray(true);
auto &ringBuffer = sidArray.getRingBuffer();

void writeRegister(uint8_t sidReg, uint8_t val) {
    uint8_t sid = (sidReg & 0xf0) >> 4;
    uint8_t reg = sidReg & 0x0f;

    // pull chip select line
    for(uint8_t i = 0; i < SIDArray::MAX_NUM_SIDS; i++) {
        digitalWrite(SID_CS[i], (i == sid) ? LOW : HIGH);
    }

}

void setup() {
    pinMode(SID_A0, OUTPUT);
    pinMode(SID_A1, OUTPUT);
    pinMode(SID_A2, OUTPUT);
    pinMode(SID_A3, OUTPUT);
    pinMode(SID_A4, OUTPUT);

    pinMode(SID_D0, OUTPUT);
    pinMode(SID_D1, OUTPUT);
    pinMode(SID_D2, OUTPUT);
    pinMode(SID_D3, OUTPUT);
    pinMode(SID_D4, OUTPUT);
    pinMode(SID_D5, OUTPUT);
    pinMode(SID_D6, OUTPUT);
    pinMode(SID_D7, OUTPUT);

    pinMode(SID_PHI_2, OUTPUT);
}

void loop() {

}
