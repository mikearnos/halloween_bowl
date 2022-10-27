#include <arduino.h>
#include "pacifica.h"
#include "hardware.h"
#include "battery.h"

bool nonBlockDelay(unsigned long* last, unsigned int delay);
extern void dfSetup(void);
extern uint16_t dfPlay(int);
bool detectHand(void);

void setup()
{
    pinMode(IRSENSOR, INPUT);

    pinMode(BATTIN, INPUT);
    analogReference(INTERNAL); // 1.1 volts, Vref must match for proper calculation

    pacificaSetup();

    Serial.begin(115200);

    dfSetup();
    dfPlay(SND_CAPCOM);
}

#define NOHAND 0
#define TRIGGERED 1
#define RUNNING 2

void loop()
{
    static int processingHand;
    //static unsigned long lastPlayer;
    static unsigned long lastTime;
    //if (nonBlockDelay(&lastPlayer, 10000)) {
    //DF1201S.playFileNum(SND_BARREL);
    //}

    if (detectHand()) {
        if (processingHand == NOHAND) {
            processingHand = TRIGGERED;
        }
        if (processingHand == TRIGGERED) {
            processingHand = RUNNING;
            //Serial.println(dfPlay(random(SND_DONT, SND_MY_MAN + 1)));
            //Serial.println(dfPlay(random(SND_BARREL, SND_MY_MAN + 1)));
            //Serial.print("hand detected ");
            //Serial.println(millis());
        }
        if (processingHand == RUNNING) {
            if (nonBlockDelay(&lastTime, 2000)) {
                processingHand = NOHAND;
            }
        }
    }

    int mode = GREEN;

    pacificaLoop(mode);

    batteryLoop();
}

bool detectHand(void)
{
    static unsigned long lastHand;
    if (!digitalRead(IRSENSOR)) {
        if (nonBlockDelay(&lastHand, 100)) {
            //Serial.print("hand detected ");
            //Serial.println(millis());
            return 1;
        }
    }
    return 0;
}

bool nonBlockDelay(unsigned long* last, unsigned int delay)
{
    unsigned long currentMillis;
    if ((currentMillis = millis()) - *last >= delay) {
        *last = currentMillis;
        return true;
    }
    return false;
}

/*int redFlashLoop(int flag)
{
    static int mode;
    if (flag == STATUS) {
        return mode;
    }

    return 0;
}*/
