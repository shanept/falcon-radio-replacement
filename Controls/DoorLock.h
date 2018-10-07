#ifndef __DOORLOCK_H__
#define __DOORLOCK_H__

#include "../Abstractions/SN74HC165N/SN74HC165N.h"
#include "../Models/Message.h"

using Abstractions::SN74HC165N;
using Models::Message;

namespace Controls {

    class DoorLock {
        private:
            int input_pin;
            int output_pin;

            SN74HC165N *pins;

            bool isPressed;
            bool status;

        public:
            DoorLock(int ip, SN74HC165N *reg):
                input_pin(ip), pins(reg), isPressed(false) {};
            void process(Message*);
    };

    void DoorLock::process(Message *msg) {
        bool isDown = pins->readPin(input_pin);

        if (isDown != isPressed) {
            isPressed = isDown;

            if (isDown) {
                msg->parts[3] |= 0x40;
            }
        }


    }

};

#endif
