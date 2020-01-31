//
// Created by kfl02 on 27.11.19.
//

#include "arduinosid.h"
#include "sid.h"

SIDArray sidArray(true);
auto &ringBuffer = sidArray.getRingBuffer();

#if defifed(ARDUINO_ARCH_AVR)
#include "arduino.cpp"
#elif defined(CORE_TEENSY)
#include "teensy.cpp"
#else
#error uknown driver board
#endif

setup() {
    setup_board();
}

loop() {
    loop_board();
}