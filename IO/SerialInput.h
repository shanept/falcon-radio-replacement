#ifndef __IO_SERIAL_INPUT_H__
#define __IO_SERIAL_INPUT_H__

#include "Input.h"
#include "HardwareSerial.h"

namespace IO {
    using Models::Message;
    using ::HardwareSerial;

    class SerialInput {
        private:
            HardwareSerial *Cereal;
            int readBuffer(char*);

        public:
            SerialInput(HardwareSerial *c): Cereal(c) {};
            bool available();
            Message read();
    };
}

#endif
