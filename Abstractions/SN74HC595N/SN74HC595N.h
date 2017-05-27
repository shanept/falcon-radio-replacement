#ifndef __SN74HC595N_H__
#define __SN74HC595N_H__

#include "Arduino.h"

namespace Abstractions {
    class SN74HC595N {
        private:
            int dataPin;
            int clockPin;
            int latchPin;
            int numChips;
            int outputEnablePin;
            int clearPin;

            bool dirty;

            byte *buffer;
        public:
            SN74HC595N(
                int data,
                int clock,
                int latch,
                int nChips = 1,
                int outputEn = -1,
                int clear = -1
            ):
                dataPin(data),
                clockPin(clock),
                latchPin(latch),
                numChips(nChips),
                outputEnablePin(outputEn),
                clearPin(clear),
                dirty(false)
            {};

            ~SN74HC595N();

            void init();

            void disable();
            void enable(unsigned short);

            void write(int, bool);

            void flush() const;
            void clear();
    };
};

#endif
