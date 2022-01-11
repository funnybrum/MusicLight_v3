#include <Arduino.h>
#include "MCP41xxx.h"
#include "MSGEQ7.h"
#include "LedOutput.h"

#define MCP41_CS      10
#define FEEDBACK_PIN  A5

#define MSGEQ7_ANALOG A0
#define MSGEQ7_STROBE 3
#define MSGEQ7_RESET  2

#define OUTPUT_RED    5
#define OUTPUT_GREEN  6
#define OUTPUT_BLUE   9

MCP41xxx pot(MCP41_CS, FEEDBACK_PIN);
MSGEQ7 msgeq7(MSGEQ7_RESET, MSGEQ7_STROBE, MSGEQ7_ANALOG);
LedOutput red(OUTPUT_RED, 80);
LedOutput green(OUTPUT_GREEN, 80);
LedOutput blue(OUTPUT_BLUE, 80);

uint32_t lastLoopMillis;
uint32_t loopCount;

void setup() {
    Serial.begin(115200);
    pot.begin();
    msgeq7.begin();

    red.begin();
    green.begin();
    blue.begin();

    pot.set(240);

    lastLoopMillis = millis();
    loopCount = 0;
    randomSeed(analogRead(5));
}

void loop() {
    msgeq7.read();

    uint8_t maxBand = 0;
    for (int i = 0; i < 7; i++) {
        maxBand = max(maxBand, msgeq7.getBandValue(i));
    }
    // Serial.println(maxBand);
    
    pot.loop();

    red.loop(max(msgeq7.getBandValue(0), msgeq7.getBandValue(1)));
    green.loop(max(msgeq7.getBandValue(3), msgeq7.getBandValue(4)));
    blue.loop(max(msgeq7.getBandValue(5), msgeq7.getBandValue(6)));

    // loopCount++;
    // if (lastLoopMillis + 1000 < millis()) {

        // char buf[128];
        // sprintf(buf, "%3d %3d %3d %3d %3d %3d %3d", msgeq7.getBandValue(0),
        //                                             msgeq7.getBandValue(1),
        //                                             msgeq7.getBandValue(2),
        //                                             msgeq7.getBandValue(3),
        //                                             msgeq7.getBandValue(4),
        //                                             msgeq7.getBandValue(5),
        //                                             msgeq7.getBandValue(6),
        //                                             msgeq7.getBandValue(7));
        // Serial.println(buf);


    //     Serial.print(loopCount);
    //     Serial.println(" loops/second");
    //     loopCount = 0;
    //     delay(500);
    //     lastLoopMillis = millis();
    // }

}