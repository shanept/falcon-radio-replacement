#ifndef __IO_SERIAL_OUTPUT_CPP__
#define __IO_SERIAL_OUTPUT_CPP__

#include "SerialOutput.h"

namespace IO {

    using Models::Message;

    void SerialOutput::write(Message msg) {
        Cereal->println("   Message ID");
        Cereal->println(msg.id, HEX);

        for (int i = 0; i < 8; i++) {
            Cereal->println("   Message part");
            Cereal->print(msg.parts[i], HEX);
            Cereal->print("\t");
        }
        Cereal->println("   End of Message");
        Cereal->println();
        Cereal->println();
    };
}

#endif
