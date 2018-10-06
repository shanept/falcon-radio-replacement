#ifndef __DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include "../Abstractions/SN74HC165N/SN74HC165N.h"
#include "../Models/Message.h"
#include "Arduino.h"

namespace Controls {

    enum DISTRIBUTION_MODE {
        FACE = 0x10,
        FACE_FOOT = 0x04,
        FOOT = 0x08,
        WINDSCREEN = 0x02,
        WINDSCREEN_FOOT = 0x01,
        REAR = 0x20,
        AIRCON = 0x80,
        RECIRCULATE = 0x40
    };

    class Distribution {
    private:
        int face_pin;
        int face_foot_pin;
        int foot_pin;
        int windscreen_demister_pin;
        int windscreen_foot_pin;
        int rear_demister_pin;
        int aircon_pin;
        int recirculate_pin;

        SN74HC165N *pins;
    public:
        Distribution(
            int face_p,     // Face
            int ff_p,       // Face and Feet
            int foot_p,     // Feet
            int wd_pin,     // Windscreen Demister
            int wf_pin,     // Windscreen Demister with Feet
            int rd_pin,     // Rear Demister
            int ac_pin,     // Aircon
            int rc_pin,     // Recirculate
            SN74HC165N *pins
        ): face_pin(face_p),
           face_foot_pin(ff_p),
           foot_pin(foot_p),
           windscreen_demister_pin(wd_pin),
           windscreen_foot_pin(wf_pin),
           rear_demister_pin(rd_pin),
           aircon_pin(ac_pin),
           recirculate_pin(rc_pin),
           pins(pins)
        {};

        void process(Message*);
    };

    void Distribution::process(Message *msg) {
        if (pins->readPin(face_pin) == HIGH) {
            msg[0] = FACE;
        } else if (pins->readPin(face_foot_pin) == HIGH) {
            msg[0] = FACE_FOOT;
        } else if (pins->readPin(foot_pin) == HIGH) {
            msg[0] = FOOT;
        } else if (pins->readPin(windscreen_demister_pin) == HIGH) {
            // Turns on A/C as well
            msg[0] = WINDSCREEN;
        } else if (pins->readPin(windscreen_foot_pin) == HIGH) {
            msg[0] = WINDSCREEN_FOOT;
        } else if (pins->readPin(rear_demister_pin) == HIGH) {
            msg[0] = REAR;
        } else if (pins->readPin(aircon_pin) == HIGH) {
            msg[0] = AIRCON;
        } else if (pins->readPin(recirculate_pin) == HIGH) {
            msg[0] = RECIRCULATE;
        }
    };

};

#endif
