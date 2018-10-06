#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "../Abstractions/SN74HC165N/SN74HC165N.h"
#include "../Models/Message.h"

using Abstractions::SN74HC165N;
using Models::Message;

namespace Controls {

    class Lights {
        private:
            int pin;
            SN74HC165N* pins;
            bool isPressed;

        public:
            Lights(int p, SN74HC165N *reg): pin(p), pins(reg), isPressed(false) {};
            void process(Message*);
    };

    void Lights::process(Message *msg) {
        bool isDown = pins->readPin(pin);

        if (isDown != isPressed) {
            isPressed = isDown;

            if (isDown) {
                msg[3] |= 0x20;
            }
        }
    }

};

#endif
