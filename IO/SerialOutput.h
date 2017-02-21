#ifndef __IO_SERIAL_OUTPUT_H__
#define __IO_SERIAL_OUTPUT_H__

#include "Output.h"
#include "HardwareSerial.h"

namespace IO {

    using Models::Message;
    using ::HardwareSerial;

    class SerialOutput {
        private:
            HardwareSerial *Cereal;

        public:
            SerialOutput(HardwareSerial *c): Cereal(c) {};
            void write(Message);
    };
}

#endif
