//
// Created by kfl02 on 31.01.20.
//

#ifndef ARDUINOSID_ARDUINO_H
#define ARDUINOSID_ARDUINO_H

/******************************************************************************

    6581    Arduino
    ====    =======
    A0      A0
    A1      A1
    A2      A2
    A3      A3
    A4      A4

    D0      D2
    D1      D3
    D2      D4
    D3      D5
    D4      D7
    D5      D8
    D6      D9
    D7      D10

    phi/2   D6

    CS #1   A5
    CS #2   D11
    CS #3   D12
    CS #4   D13
    CS #5   N/A
    CS #6   N/A

******************************************************************************/

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
static const uint8_t A6 = PIN_A6;   // not usable as digital on Nano
static const uint8_t A7 = PIN_A7;   // not usable as digital on Nano

#endif // PIN_A0

// Arduino pin numbers for SID pins

static const uint8_t SID_A0 = A0;
static const uint8_t SID_A1 = A1;
static const uint8_t SID_A2 = A2;
static const uint8_t SID_A3 = A3;
static const uint8_t SID_A4 = A4;

static const uint8_t NUM_SID_AX = 5;

static const std::array<uint8_t, NUM_SID_AX> SID_AX = { SID_A0, SID_A1, SID_A2, SID_A3, SID_A4 };

static const uint8_t SID_D0 = 2;
static const uint8_t SID_D1 = 3;
static const uint8_t SID_D2 = 4;
static const uint8_t SID_D3 = 5;
static const uint8_t SID_D4 = 7;
static const uint8_t SID_D5 = 8;
static const uint8_t SID_D6 = 9;
static const uint8_t SID_D7 = 10;

static const uint8_t NUM_SID_DX = 8;

static const std::array<uint8_t, NUM_SID_DX> SID_DX = { SID_D0, SID_D1, SID_D2, SID_D3, SID_D4, SID_D5, SID_D6, SID_D7 };

static const uint8_t SID_PHI_2 = 6;

static const std::array<uint8_t, SIDArray::MAX_NUM_SIDS> SID_CS = { A5, 11, 12, 13 };

#endif //ARDUINOSID_ARDUINO_H
