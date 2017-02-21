#ifndef __IO_INPUT_H__
#define __IO_INPUT_H__

#include "../Models/Message.h"

namespace IO {

    using Models::Message;

    class Input {
        public:
            virtual bool available() const;
            virtual Message read() const;
    };
}

#endif
