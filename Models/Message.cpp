#ifndef __MESSAGE_CPP__
#define __MESSAGE_CPP__

#include "Message.h"
#include "Arduino.h"

#define MSG_DELAY 1000

namespace Models {
    bool Message::send() {
        unsigned long secs = millis();

        if ((unsigned long)(secs - last_sent) < MSG_DELAY) {
            return false;
        }

                        // Output contents of message to screen
                        Serial.print("(0x");
                        Serial.print(id, HEX);
                        Serial.print(")");
                        for (int i = 0; i < 8; i++) {
                            Serial.print("\t0x");
                            Serial.print(parts[i], HEX);
                        };
                        Serial.println();

        last_sent = secs;

        return can->sendMsgBuf(id, 0, 0, 8, parts);
    };

    void Message::reset() {
        // Empty message contents
        memset(parts, 0, sizeof(parts));

        id       = 0x307;       // Set radio ID
        parts[7] = 0x1E;        // Last bit always outputs 0x1E
    };

    INT8U Message::operator[](int i) const {
        return parts[i];
    };

    INT8U& Message::operator[](int i) {
        return parts[i];
    };
};

#endif
