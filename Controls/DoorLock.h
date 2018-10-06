#ifndef __DOORLOCK_H__
#define __DOORLOCK_H__

#include "../Abstractions/SN74HC165N/SN74HC165N.h"
#include "../Models/Message.h"

using Abstractions::SN74HC165N;
using Models::Message;

namespace Controls {

    class DoorLock {
        private:
            int pin;
            SN74HC165N* pins;
            bool isPressed;

        public:
            DoorLock(int p, SN74HC165N *reg): pin(p), pins(reg), isPressed(false) {};
            void process(Message*);
    };

    void DoorLock::process(Message *msg) {
        bool isDown = pins->readPin(pin);

        if (isDown != isPressed) {
            isPressed = isDown;

            if (isDown) {
                msg[3] |= 0x40;
            }
        }
    }

};

#endif
