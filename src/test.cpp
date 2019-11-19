#include "sid.h"
#include <iostream>

const void testCallback(const uint8_t reg, const uint8_t val) {
    std::cout << "reg: " << (unsigned int)reg << " val: " << (unsigned int)val << "\n";
}

int main() {
    SID sid;

    for(uint8_t i = 0; i < SID_NUM_VOICES; i++) {
        sid.getVoice(i).setRegisterWriteCallback(testCallback);
    }

    sid.getFilter().setRegisterWriteCallback(testCallback);

    sid.getFilter().setVolume(15);
    sid.getVoice(1).setAttack(12);
    sid.getVoice(0).setFQ(12000);
}