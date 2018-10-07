#ifndef __CONDITIONING_H__
#define __CONDITIONING_H__

#include "../Models/Message.h"
#include "Arduino.h"

namespace Controls {

    class Conditioning {
    private:
        int temp_pin;
        int fan_pin;

    public:
        Conditioning(int temp, int fan): temp_pin(temp), fan_pin(fan) {};
        void process(Message*);
    };

    void Conditioning::process(Message *msg) {
        float temp_perc = ((float) analogRead(temp_pin)) / 1024;
        float fan_perc  = ((float) analogRead(fan_pin)) / 1024;

        msg->parts[5] = (unsigned int) (temp_perc * 0x0F);
        msg->parts[6] = (unsigned int) (fan_perc  * 0x0E);
    };

}

#endif
