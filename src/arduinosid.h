#pragma once

#ifndef ARDUINOSID_ARDUINOSID_H
#define ARDUINOSID_ARDUINOSID_H

/******************************************************************************

    Arduino	6581
    =======	====
    A0		A0
    A1		A1
    A2		A2
    A3		A3
    A4		A4

    D2		D0
    D3		D1
    D4		D2
    D5		D3
    D6		D4
    D7		D5
    D8		D6
    D9		D7

    D10
    D11
    D12
    D13		phi/2

    A5
    A6
    A7

    GND		R/W

******************************************************************************/

#include "sid.h"

// just for compilation test purposes without the Arduino IDE

#ifndef PIN_A0

#define PIN_A0	(14)
#define PIN_A1	(15)
#define PIN_A2	(16)
#define PIN_A3	(17)
#define PIN_A4	(18)
#define PIN_A5	(19)
#define PIN_A6	(20)
#define PIN_A7	(21)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

#endif // PIN_A0

// Arduino pin numbers for SID pins

static const uint8_t SID_A0 = A0;
static const uint8_t SID_A1 = A1;
static const uint8_t SID_A2 = A2;
static const uint8_t SID_A3 = A3;
static const uint8_t SID_A4 = A4;

static const uint8_t NUM_SID_AX

static const uint8_t SID_D0 = 0;
static const uint8_t SID_D1 = 1;
static const uint8_t SID_D2 = 2;
static const uint8_t SID_D3 = 3;
static const uint8_t SID_D4 = 4;
static const uint8_t SID_D5 = 5;
static const uint8_t SID_D6 = 6;
static const uint8_t SID_D7 = 7;

static const uint8_t SID_PHI_2 = 13;

static const std::array<uint8_t, SIDArray::MAX_NUM_SIDS> SID_CS = { A5, A6, A7, 10, 11, 12 };

#endif //ARDUINOSID_ARDUINOSID_H
