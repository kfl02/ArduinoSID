//
// Created by kfl02 on 27.11.19.
//

#include "arduinosid.h"
#include "sid.h"

SIDArray sidArray(true);
auto &ringBuffer = sidArray.getRingBuffer();

void writeRegister(uint8_t sid, uint8_t reg, uint8_t val) {
    // TODO: somehow wait for last write to be done

    // pull chip select line
    for(uint8_t i = 0; i < SIDArray::MAX_NUM_SIDS; i++) {
        digitalWrite(SID_CS[i], (i == sid) ? LOW : HIGH);
    }

    // set address lines
    for(uint8_t i = 0, ax = reg; i < NUM_SID_AX; i++, ax >>= 1) {
        digitalWrite(SID_AX[i], (ax & 1) ? HIGH : LOW);
    }

    // set data lines
    for(uint8_t i = 0, dx = val; i < NUM_SID_DX; i++, dx >>= 1) {
        digitalWrite(SID_DX[i], (dx & 1) ? HIGH : LOW);
    }

    // TODO: somehow signal everything is ready to write
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
