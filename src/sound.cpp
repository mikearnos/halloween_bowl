#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include "hardware.h"

SoftwareSerial DF1201SSerial(DFRX, DFTX); //RX  TX
DFRobot_DF1201S DF1201S;

void dfSetup(void)
{
    DF1201SSerial.begin(9600);
    while (!DF1201S.begin(DF1201SSerial)) {
        Serial.print("Init failed, please check the wire connection! ");
        Serial.println(millis());
        delay(10);
    }

    // need to power cycle module after these are changed
    //DF1201S.setBaudRate(9600);
    //DF1201A.setLED(off)
    //DF1201S.setPrompt(false);

    DF1201S.setVol(20);
    Serial.print("VOL:");
    Serial.println(DF1201S.getVol());

    DF1201S.switchFunction(DF1201S.MUSIC);
    DF1201S.setPlayMode(DF1201S.SINGLE);

    Serial.print("PlayMode:");
    Serial.println(DF1201S.getPlayMode());
}

uint16_t dfPlay(int fileID)
{
    DF1201S.playFileNum(fileID);
    uint16_t totalTime = DF1201S.getTotalTime();
    if (totalTime == 0)
        return 1;

    //return totalTime + 1;
    return totalTime;
}
