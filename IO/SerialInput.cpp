#ifndef __IO_SERIAL_INPUT_CPP__
#define __IO_SERIAL_INPUT_CPP__

#include "SerialInput.h"

namespace IO {
    using Models::Message;

    bool SerialInput::available() {
        return Cereal->available() > 0;
    };

    Message SerialInput::read() {
        char *input = new char;
        int bytes = readBuffer(input);

        for (int i = 0; i < bytes; i++) {
            input[i] -= 48;
        }

        // Convert integers to message
        Message m = (Message&) *input;

        // clean up
        delete[] input;
        input = NULL;

        return m;
    };

    int SerialInput::readBuffer(char *buffer) {
        int bytes = Cereal->available();

        for (int i = 0; i < bytes; i++) {
            buffer[i] = Cereal->read();
        }

        return bytes;
    };
}

#endif
