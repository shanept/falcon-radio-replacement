#ifndef __SN74HC595N_CPP__
#define __SN74HC595N_CPP__

#include "SN74HC595N.h"

void SN74HC595N::init() {
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);

    if (-1 != outputEnablePin) {
        pinMode(outputEnablePin, OUTPUT);
        digitalWrite(outputEnablePin, 0);
    }

    if (-1 != clearPin) {
        pinMode(clearPin, OUTPUT);
        digitalWrite(clearPin, 1);
    }

    buffer = new byte[numChips];
    memcpy(buffer, 0, numChips);

    // Then zero the outputs
    dirty = true;
    flush();
};

void SN74HC595N::enable() {
    if (-1 == outputEnablePin) {
        // Unsupported Operation
        Serial.println(" * Unsupported operation: SN74HC595N::enable");
        return;
    }

    digitalWrite(outputEnablePin, 0);
}

void SN74HC595N::disable() {
    if (-1 == outputEnablePin) {
        // Unsupported Operation
        Serial.println(" * Unsupported operation: SN74HC595N::disable");
        return;
    }

    digitalWrite(outputEnablePin, 1);
};

void SN74HC595N::write(int pin, bool on) {
    int bit  = (pin % 8);
    int chip = (pin / 8);

    // Do we need to perform an operation?
/*    if (on != (buffer[chip] & (1 << bit))) {
        return;
    }
*/
    dirty = true;

    if (on) {
        buffer[chip] |= (1 << bit);
    } else {
        buffer[chip] &= ~(1 << bit);
    }
};

void SN74HC595N::flush() const {
    if (!dirty) return;

    digitalWrite(latchPin, 0);
    digitalWrite(dataPin, 0);
    digitalWrite(clockPin, 0);

    //NOTICE THAT WE ARE COUNTING DOWN in our for loop
    //This means that %00000001 or "1" will go through such
    //that it will be pin Q0 that lights.
    for (int i = 0; i < (numChips * 8); i++)  {

        int bit  = (i % 8);
        int chip = (i / 8);

        digitalWrite(clockPin, 0);

        if ((buffer[chip] >> (7 - bit)) & 1) {
            digitalWrite(dataPin, 1);
        } else {
            digitalWrite(dataPin, 0);
        }

        //register shifts bits on upstroke of clock pin
        digitalWrite(clockPin, 1);
        //zero the data pin after shift to prevent bleed through
        digitalWrite(dataPin, 0);
    }

    // Stop shifting
    digitalWrite(clockPin, 0);

    // Pulse the latch pin to load on positive edge
    digitalWrite(latchPin, 1);
    delayMicroseconds(5);
    digitalWrite(latchPin, 0);
};

void SN74HC595N::clear() {
    memcpy(buffer, 0, numChips);

    if (-1 != clearPin) {
        // Unsupported Operation
        Serial.println(" * Unsupported Operation: SN74HC595N::clear");
        return;
    }

    // We will have to simulate the clear
    dirty = true;
    flush();
};

#endif
