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
#include "Controls/Conditioning.h"

using Abstractions::SN74HC165N;
using Abstractions::SN74HC595N;
using Controls::DoorLock;
using Controls::Lights;
using Controls::Distribution;
using Controls::Conditioning;
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
Conditioning *aircon;

void setup() {
    Serial.begin(115200);
    Serial.println("Reboot!");

    can = new MCP_CAN(CAN_SELECT);
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

    pins = new SN74HC165N(INPUT_LOAD, INPUT_ENABLE, INPUT_CLOCK, INPUT_CHAINOUT, 2);
    pins->init();

    leds = new SN74HC595N(LED_DATA, LED_CLOCK, LED_LATCH, 2, LED_OUTPUT_ENABLE);
    leds->init();

    locks = new DoorLock(VIPIN_LOCK, pins);
    lights = new Lights(VIPIN_LIGHT, pins);
    dist = new Distribution(
        VIPIN_DIST_FACE,
        VIPIN_DIST_FACE_FOOT,
        VIPIN_DIST_FOOT,
        VIPIN_DIST_WINDSCREEN_DEMISTER,
        VIPIN_DIST_WINDSCREEN_FOOT,
        VIPIN_DIST_REAR_DEMISTER,
        VIPIN_DIST_AIRCON,
        VIPIN_DIST_RECIRCULATE,

        VOPIN_DIST_FACE,
        VOPIN_DIST_FACE_FOOT,
        VOPIN_DIST_FOOT,
        VOPIN_DIST_WINDSCREEN_DEMISTER,
        VOPIN_DIST_WINDSCREEN_FOOT,
        VOPIN_DIST_REAR_DEMISTER,
        VOPIN_DIST_AIRCON,
        VOPIN_DIST_RECIRCULATE,

        pins,
        leds
    );
    aircon = new Conditioning(PIN_TEMP, PIN_FAN);
}

void loop() {
    pins->poll();

    locks->process(msg);
    lights->process(msg);
    dist->process(msg);
    aircon->process(msg);

    msg->send();
    leds->flush();
    delayMicroseconds(20);
}
