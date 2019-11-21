#include "sid.h"
#include <iostream>

const void testCallback(const uint8_t reg, const uint8_t val) {
    std::cout << "reg: " << std::hex << (unsigned int)reg << " val: " << std::hex << (unsigned int)val << "\n";
}

int main() {
    SIDArray sidArray(true);

//    for(uint8_t i = 0; i < SID_ARRAY_MAX_NUM_SIDS; i++) {
//        SID& sid = sidArray.getSID(i);
//
//        for(uint8_t j = 0; j < SID_NUM_VOICES; j++) {
//            sid.getVoice(j).setRegisterWriteCallback(testCallback);
//        }
//
//        sid.getFilter().setRegisterWriteCallback(testCallback);
//    }

    for(uint8_t i = 0; i < SID_ARRAY_MAX_NUM_SIDS; i++) {
        SID& sid = sidArray.getSID(i);

        sid.getFilter().setVolume(15);
        sid.getVoice(1).setAttack(12);
        sid.getVoice(0).setFQ(12000);
        sid.getVoice(2).setTriangle(true);
        sid.getVoice(2).setGate(true);
        sid.getVoice(2).setGate(false);
    }

    for(uint8_t i = 0; i < 255; i++) {
        sidArray.getSID(0).getFilter().setVolume(0);
    }
}