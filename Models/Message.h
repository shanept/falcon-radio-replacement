#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "mcp_can.h"
#include "Arduino.h"

using ::MCP_CAN;

namespace Models {
    enum Mode {
        OFF,    // KEY OFF
        ACC,    // ACCESSORIES
        KOEO,   // KEY ON ENGINE OFF
        KOER,   // KEY ON ENGINE RUNNING
    };

    class Message {
    protected:
        INT32U id;
        INT8U  parts[8];// = {0, 0, 0, 0, 0, 0, 0, 0};

        MCP_CAN *can;
        unsigned long last_sent;

    public:
        Message(MCP_CAN *c, INT32U _id=0x307): id(_id), can(c), last_sent(0) {
            reset();
        };

        bool send();
        void reset();

        INT8U  operator[](int) const;
        INT8U &operator[](int);
    };
}

#endif
