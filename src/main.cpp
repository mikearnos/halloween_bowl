#include <arduino.h>
#include "pacifica.h"
#include "hardware.h"
#include "battery.h"

bool nonBlockDelay(unsigned long* last, unsigned int delay);
extern void dfSetup(void);
extern uint16_t dfPlay(int);
bool detectHand(void);

int isPlaying = 0;
volatile int trigger = 0;
volatile unsigned long prevISR = 0;
void triggerISR(void);

void setup()
{
    batterySetup();

    pacificaSetup();

    Serial.begin(115200);

    dfSetup();
    dfPlay(SND_CAPCOM);

    pinMode(TRIGGER, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(TRIGGER), triggerISR, FALLING);
}

volatile unsigned long lastISR = -1;
volatile unsigned long lastMillis = -1;
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
    if (nonBlockDelay(&lastPlaying, 1000)) {
        if (isPlaying)
            isPlaying--;
    }

    if (trigger) {
        isPlaying = dfPlay(random(SND_BARREL, SND_MY_MAN + 1));
        Serial.println(isPlaying);
        trigger = 0;
    }

    int mode = GREEN;

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

/*int redFlashLoop(int flag)
{
    static int mode;
    if (flag == STATUS) {
        return mode;
    }

    return 0;
}*/
