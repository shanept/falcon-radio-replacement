#ifndef __MESSAGE_H__
#define __MESSAGE_H__

namespace Models {
    enum Mode {
        OFF,    // KEY OFF
        ACC,    // ACCESSORIES
        KOEO,   // KEY ON ENGINE OFF
        KOER,   // KEY ON ENGINE RUNNING
    };

    class Message {
        public:
            int id;
            int parts[8] = {0, 0, 0, 0, 0, 0, 0, 0};

            Message(): id(0) {}
    };
}

#endif
