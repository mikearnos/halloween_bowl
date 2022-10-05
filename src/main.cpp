#include <arduino.h>
#include "pacifica.h"

#define IRSENSOR PD2
#define BATTIN A0

const float R1 = 99400;
const float R2 = 5520;
const float Vref = 1.135; // fine tune the so 7.2 battery voltage matches

#define BATTERY_MAX 3.10
#define BATTERY_MIN 2.50

bool nonBlockDelay(unsigned long* last, unsigned int delay);
float voltageMeasurement(void);

void setup()
{
    pinMode(IRSENSOR, INPUT);
    pinMode(BATTIN, INPUT);
    digitalWrite(BATTIN, LOW);

    analogReference(INTERNAL); // 1.1 volts
    Serial.begin(115200);
    pacificaSetup();

    //int battery = analogRead(BATTIN);
}

unsigned long lastBlink = 0;
unsigned long lastBatt = 0;
void loop()
{
    static int beamCount;
    int mode = GREEN;
    //if (nonBlockDelay(&lastBlink, 5000)) {
    //mode = REDFLASH;
    //}

    if (nonBlockDelay(&lastBatt, 1000)) {
        voltageMeasurement();
    }

    if (redFlashTimer == 0) {
        if (!digitalRead(IRSENSOR)) {
            //beamCount += 2;
            if (beamCount > 100) {
                mode = REDFLASH;
            }
        }
        if (beamCount > 0)
            beamCount--;
    }

    pacificaLoop(mode);
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

float voltageMeasurement()
{
    const float maxVolts = Vref * (R1 + R2) / R2;
    float analogAverage = 0;

    delay(10); // let voltage stabilize for a moment

    for (int i = 0; i < 4; i++) {
        analogAverage += analogRead(BATTIN);
    }
    analogAverage /= 4;

    //int read = analogRead(BATTIN);
    //Serial.println(read);

    float analogVoltage = analogAverage * (maxVolts / 1024);

    Serial.println(analogVoltage);

    return analogVoltage;
}
