#include "mcp_can.h"
#include "IO/SerialInput.h"
#include "IO/SerialOutput.h"
#include "Controls/SN74HC165N/SN74HC165N.h"

using Controls::SN74HC165N;

MCP_CAN *can;

IO::SerialInput *input;
IO::SerialOutput *output;

SN74HC165N *pins;

Models::Message m;

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

    pins = new SN74HC165N(5, 8, 6, 7);
    pins->init();

    return;

    pinMode(2, INPUT);

    input = new IO::SerialInput(&Serial);
    output = new IO::SerialOutput(&Serial);
}

unsigned char stmp[8] = {0, 0, 0, 0x40, 0, 0, 0, 0x1E};

void loop() {
    Serial.print("Polling...");
    pins->poll();
    Serial.println("Done!");

    if (pins->readPin(0)) {
        Serial.println("Unlock Door");
        can->sendMsgBuf(307, 0, 8, stmp);
        Serial.println("Unlocked!");
    }

    delay(200);
    return;
    Serial.println("loop");

    if (input->available()) {
        Serial.println(" Reading data");
        m = input->read();
    }

     Serial.println(" Writing data");
    output->write(m);
    Serial.println(" Data written");
    delay(1000);
}
