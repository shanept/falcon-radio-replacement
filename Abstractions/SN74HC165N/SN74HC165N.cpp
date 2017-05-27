#ifndef __SN74HC165N_CPP__
#define __SN74HC165N_CPP__

#include "SN74HC165N.h"
#include "Arduino.h"

namespace Abstractions {
    SN74HC165N::~SN74HC165N() {
        delete[] buffer;
    };

    void SN74HC165N::init() {
        pinMode(pLoadPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(clockEnablePin, OUTPUT);
        pinMode(dataPin, INPUT);

        buffer = new unsigned int[numChips];

        digitalWrite(clockPin, LOW);
        digitalWrite(pLoadPin, HIGH);
    };

    void SN74HC165N::poll() {
        // Clear the buffer
        memset(buffer, 0, numChips + 1);

        // Trigger a parallel Load to latch the state of the data lines,
        digitalWrite(clockEnablePin, HIGH);
        digitalWrite(pLoadPin, LOW);
        delayMicroseconds(pulseWidth);
        digitalWrite(pLoadPin, HIGH);
        digitalWrite(clockEnablePin, LOW);

        /* Loop to read each bit value from the serial out line
         * of the SN74HC165N.
        */
        for (int i = 0; i < (numChips * 8); i++)
        {
            buffer[i / 8] |= (digitalRead(dataPin) << (7 - (i % 8)));

            // Pulse the clock - the rising edge shifts the next bit
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(pulseWidth);
            digitalWrite(clockPin, LOW);
        }
    };

    unsigned int SN74HC165N::readPin(int pin) {
        int bit  = (pin % 8);
        int chip = (pin / 8);

        return ((buffer[chip] & 1 << (bit)) ? 1 : 0);
    };
}

#endif
