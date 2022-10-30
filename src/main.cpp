#include <arduino.h>
#include "pacifica.h"
#include "hardware.h"
#include "battery.h"

bool nonBlockDelay(unsigned long* last, unsigned int delay);
extern void dfSetup(void);
extern uint16_t dfPlay(int);
bool detectHand(void);

int isPlaying = 1;
volatile int trigger = 0;
volatile unsigned long prevISR = 0;
void triggerISR(void);

void setup()
{
    batterySetup();

    pacificaSetup();

    Serial.begin(115200);

    dfSetup();
    dfPlay(1); // play first mp3 as startup sound

    pinMode(COMMS1, INPUT_PULLUP); // trigger
    pinMode(COMMS2, INPUT_PULLUP); // armed
    attachInterrupt(digitalPinToInterrupt(TRIGGER), triggerISR, FALLING);
}

unsigned long lastISR = -1;
unsigned long lastMillis = -1;
void triggerISR(void)
{
    unsigned long currentMillis;
    if ((currentMillis = millis()) - lastMillis >= 500) {
        lastMillis = currentMillis;
        if (!isPlaying)
            trigger = 1;
    }
}

void loop()
{
    static unsigned long lastPlaying;
    if (trigger) {
        isPlaying = dfPlay(random(2, totalFiles));
        lastPlaying = millis();
        //Serial.println(isPlaying);
        trigger = 0;
    }
    if (nonBlockDelay(&lastPlaying, 1000)) {
        if (isPlaying)
            isPlaying--;
    }

    int mode = GREEN;
    if (isPlaying) {
        mode = REDFLASH;
    }

    pacificaLoop(mode);

    batteryLoop();
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
