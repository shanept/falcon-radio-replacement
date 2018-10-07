#ifndef __DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include "../Abstractions/SN74HC165N/SN74HC165N.h"
#include "../Abstractions/SN74HC595N/SN74HC595N.h"
#include "../Models/Message.h"
#include "Arduino.h"

using Abstractions::SN74HC165N;
using Abstractions::SN74HC595N;

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
        int face_input_pin;
        int face_foot_input_pin;
        int foot_input_pin;
        int windscreen_demister_input_pin;
        int windscreen_foot_input_pin;
        int rear_demister_input_pin;
        int aircon_input_pin;
        int recirculate_input_pin;

        int face_output_pin;
        int face_foot_output_pin;
        int foot_output_pin;
        int windscreen_demister_output_pin;
        int windscreen_foot_output_pin;
        int rear_demister_output_pin;
        int aircon_output_pin;
        int recirculate_output_pin;

        SN74HC165N *pins;
        SN74HC595N *leds;

        int status;
        int oldstatus;
    public:
        Distribution(
            int in_face_p,     // Face
            int in_ff_p,       // Face and Feet
            int in_foot_p,     // Feet
            int in_wd_pin,     // Windscreen Demister
            int in_wf_pin,     // Windscreen Demister with Feet
            int in_rd_pin,     // Rear Demister
            int in_ac_pin,     // Aircon
            int in_rc_pin,     // Recirculate

            int out_face_p,     // Face
            int out_ff_p,       // Face and Feet
            int out_foot_p,     // Feet
            int out_wd_pin,     // Windscreen Demister
            int out_wf_pin,     // Windscreen Demister with Feet
            int out_rd_pin,     // Rear Demister
            int out_ac_pin,     // Aircon
            int out_rc_pin,     // Recirculate

            SN74HC165N *pins,
            SN74HC595N *leds

         ): face_input_pin(in_face_p),
            face_foot_input_pin(in_ff_p),
            foot_input_pin(in_foot_p),
            windscreen_demister_input_pin(in_wd_pin),
            windscreen_foot_input_pin(in_wf_pin),
            rear_demister_input_pin(in_rd_pin),
            aircon_input_pin(in_ac_pin),
            recirculate_input_pin(in_rc_pin),

            face_output_pin(out_face_p),
            face_foot_output_pin(out_ff_p),
            foot_output_pin(out_foot_p),
            windscreen_demister_output_pin(out_wd_pin),
            windscreen_foot_output_pin(out_wf_pin),
            rear_demister_output_pin(out_rd_pin),
            aircon_output_pin(out_ac_pin),
            recirculate_output_pin(out_rc_pin),

            pins(pins),
            leds(leds),

            status(0),
            oldstatus(0)
        {};

        void process(Message*);
    };

    void Distribution::process(Message *msg) {
        if (HIGH == pins->readPin(face_input_pin)) {
            status = face_output_pin;
            msg->parts[0] = FACE;
        } else if (HIGH == pins->readPin(face_foot_input_pin)) {
            status = face_foot_output_pin;
            msg->parts[0] = FACE_FOOT;
        } else if (HIGH == pins->readPin(foot_input_pin)) {
            status = foot_output_pin;
            msg->parts[0] = FOOT;
        } else if (HIGH == pins->readPin(windscreen_demister_input_pin)) {
            status = windscreen_demister_output_pin;
            msg->parts[0] = WINDSCREEN;   // Turns on A/C as well
        } else if (HIGH == pins->readPin(windscreen_foot_input_pin)) {
            status = windscreen_foot_output_pin;
            msg->parts[0] = WINDSCREEN_FOOT;
        } else if (HIGH == pins->readPin(rear_demister_input_pin)) {
            status = rear_demister_output_pin;
            msg->parts[0] = REAR;
        } else if (HIGH == pins->readPin(aircon_input_pin)) {
            status = aircon_output_pin;
            msg->parts[0] = AIRCON;
        } else if (HIGH == pins->readPin(recirculate_input_pin)) {
            status = recirculate_output_pin;
            msg->parts[0] = RECIRCULATE;
        }

        // Now we change the LEDs to reflect the new distribution
        if (status != oldstatus) {
            if (status != face_output_pin) leds->write(face_output_pin, false);
            if (status != face_foot_output_pin) leds->write(face_foot_output_pin, false);
            if (status != foot_output_pin) leds->write(foot_output_pin, false);
            if (status != windscreen_demister_output_pin) leds->write(windscreen_demister_output_pin, false);
            if (status != windscreen_foot_output_pin) leds->write(windscreen_foot_output_pin, false);
            if (status != rear_demister_output_pin) leds->write(rear_demister_output_pin, false);
            if (status != aircon_output_pin) leds->write(aircon_output_pin, false);
            if (status != recirculate_output_pin) leds->write(recirculate_output_pin, false);

            leds->write(status, true);
            oldstatus = status;
        }
    };

};

#endif
