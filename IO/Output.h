#ifndef __IO_OUTPUT_H__
#define __IO_OUTPUT_H__

#include "../Models/Message.h"

namespace IO {

    using Models::Message;

    class Output {
        public:
            virtual void write(Message);
    };
}

#endif
