//
// Created by kfl02 on 27.11.19.
//

#include "arduinosid.h"
#include "sid.h"

SIDArray sidArray(true);
auto &ringBuffer = sidArray.getRingBuffer();

void writeRegister(uint8_t sid, uint8_t reg, uint8_t val) {
    // TODO: somehow wait for last write to be done

    // reset chip select lines
    for(uint8_t i = 0; i < SIDArray::MAX_NUM_SIDS; i++) {
        digitalWrite(SID_CS[i], HIGH);
    }

    // set address lines
    for(uint8_t i = 0, ax = reg; i < NUM_SID_AX; i++, ax >>= 1) {
        digitalWrite(SID_AX[i], (ax & 1) ? HIGH : LOW);
    }

    // set data lines
    for(uint8_t i = 0, dx = val; i < NUM_SID_DX; i++, dx >>= 1) {
        digitalWrite(SID_DX[i], (dx & 1) ? HIGH : LOW);
    }

    // pull chip select line for given SID
    digitalWrite(SID_CS[sid], LOW);

    // TODO: somehow signal everything is ready to write
}

void setup() {
    // address lines
    for(uint8_t i = 0; i < NUM_SID_AX; i++) {
        pinMode(SID_AX[i], OUTPUT);
    }

    // data lines
    for(uint8_t i = 0; i < NUM_SID_DX; i++) {
        pinMode(SID_DX[i], OUTPUT);
    }

    // clock line
    pinMode(SID_PHI_2, OUTPUT);

    // chip select lines
    for(uint8_t i = 0; i < SIDArray::MAX_NUM_SIDS; i++) {
        pinMode(SID_CS[i], OUTPUT);
    }
}

void loop() {

}
