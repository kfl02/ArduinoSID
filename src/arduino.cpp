//
// Created by kfl02 on 31.01.20.
//

#if defined(ARDUINO_ARCH_AVR)

constexpr void fastDigitalWrite(const uint8_t pin, const uint8_t val) {
    if(val) {
        if(pin > 13) {
            PORTC |= (1 << (pin - 13));
        } else if(pin < 8) {
            PORTD | = (1 << pin);
        } else {
            PORTB |= (1 << (pin -8 ));
        }
    } else {
        if(pin > 13) {
            PORTC &= ~(1 << (pin - 13));
        } else if(pin < 8) {
            PORTD & = ~(1 << pin);
        } else {
            PORTB &= ~(1 << (pin -8 ));
        }
    }
}

void writeRegister(const uint8_t sid, const uint8_t reg, const uint8_t val) {
    // TODO: somehow wait for last write to be done

    // reset chip select lines
    for(uint8_t i = 0; i < SIDArray::MAX_NUM_SIDS; i++) {
        fastDigitalWrite(SID_CS[i], HIGH);
    }

    // set address lines
    for(uint8_t i = 0, ax = reg; i < NUM_SID_AX; i++, ax >>= 1) {
        fastDigitalWrite(SID_AX[i], (ax & 1) ? HIGH : LOW);
    }

    // set data lines
    for(uint8_t i = 0, dx = val; i < NUM_SID_DX; i++, dx >>= 1) {
        fastDigitalWrite(SID_DX[i], (dx & 1) ? HIGH : LOW);
    }

    // pull chip select line for given SID
    digitalWrite(SID_CS[sid], LOW);

    // TODO: somehow signal everything is ready to write
}

void setup_board() {
    // set up pins as digital output

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

    // initialize timer1

    cli();

    TCCR0A = COM0A0;
    TCCR0B = (1 << CS01) | ( 1 << CS00);
    ...
}

void loop_board() {

}

#endif
