#ifndef __SN74HC165N_H__
#define __SN74HC165N_H__

namespace Abstractions {
    class SN74HC165N {
        private:
            int pLoadPin;
            int clockEnablePin;
            int clockPin;
            int dataPin;

            int numChips;
            int pulseWidth;

            unsigned int *buffer;

        public:
            SN74HC165N(int pLoad, int clockEnable, int clock, int data, int chips = 1, int pulse = 5):
                pLoadPin(pLoad),
                clockEnablePin(clockEnable),
                clockPin(clock),
                dataPin(data),
                numChips(chips),
                pulseWidth(pulse) {};

            ~SN74HC165N();

            void init();
            void poll();

            unsigned int readPin(int);
    };
}

#endif
