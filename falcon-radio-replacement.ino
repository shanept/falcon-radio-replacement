#include "mcp_can.h"
#include "IO/SerialInput.h"
#include "IO/SerialOutput.h"
#include "Abstractions/SN74HC165N/SN74HC165N.h"
#include "Abstractions/SN74HC595N/SN74HC595N.h"
#include "Models/Message.h"
#include "Controls.h"

#include "Controls/DoorLock.h"
#include "Controls/Lights.h"
#include "Controls/Distribution.h"

using Abstractions::SN74HC165N;
using Abstractions::SN74HC595N;
using Controls::DoorLock;
using Controls::Lights;
using Controls::Distribution;
//using Controls::Conditioning;
using Models::Message;

MCP_CAN *can;

IO::SerialInput *input;
IO::SerialOutput *output;

SN74HC165N *pins;
SN74HC595N *leds;

Message *msg;

// Define radio controls
DoorLock *locks;
Lights *lights;
Distribution *dist;
//Conditioning *aircon;

void setup() {
    Serial.begin(115200);
    Serial.println("Reboot!");

    can = new MCP_CAN(10);
    while (true) {
        if (CAN_OK == can->begin(CAN_500KBPS)) {
            Serial.println("CAN BUS Shield init ok.");
            break;
        }

        Serial.println("CAN BUS Shield init failed. Trying again.");
        delay(100);
    }

    msg = new Message(can);
    msg->reset();

    pins = new SN74HC165N(5, 8, 6, 7, 2);
    pins->init();

    leds = new SN74HC595N(3, 14, 4, 2, 9);
    leds->init();

    locks = new DoorLock(VPIN_LOCK, pins);
    lights = new Lights(VPIN_LIGHT, pins);
    dist = new Dist(
        VPIN_DIST_FACE,
        VPIN_DIST_FACE_FOOT,
        VPIN_DIST_FOOT,
        VPIN_DIST_WINDSCREEN_DEMISTER,
        VPIN_DIST_WINDSCREEN_FOOT,
        VPIN_DIST_REAR_DEMISTER,
        VPIN_DIST_AIRCON,
        VPIN_DIST_RECIRCULATE,
        pins
    );
//    aircon = new Conditioning(PIN_TEMP, PIN_FAN);
}

void loop() {
    pins->poll();

    locks->process(msg);
    lights->process(msg);
    dist->process(msg);
//    aircon->process(msg);

    delay(20);
}
