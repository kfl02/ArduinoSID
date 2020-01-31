#pragma once

#ifndef ARDUINOSID_ARDUINOSID_H
#define ARDUINOSID_ARDUINOSID_H

/******************************************************************************

    Arduino IDE macros for boards:

    Arduino Uno/Nano/Mega
    -DF_CPU=16000000L -DARDUINO=10810 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR
    -DF_CPU=16000000L -DARDUINO=10810 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR
    -DF_CPU=16000000L -DARDUINO=10810 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR

    Teensy 3.6/4.0
    -D__MK66FX1M0__ -DTEENSYDUINO=149 -DARDUINO=10810 -DF_CPU=180000000
    -D__IMXRT1062__ -DTEENSYDUINO=149 -DARDUINO=10810 -DF_CPU=600000000

******************************************************************************/

#include "sid.h"

#if defifed(ARDUINO_ARCH_AVR)
#include "arduino.h"
#elif defined(CORE_TEENSY)
#include "teensy.h"
#else
#error uknown driver board
#endif

#endif //ARDUINOSID_ARDUINOSID_H
