#include <arduino.h>
#include "pacifica.h"

#define IRSENSOR PD2
#define BATTIN A0

const float R1 = 99400;
const float R2 = 5520;
const float Vref = 1.135; // fine tune the so 7.2 battery voltage matches

#define BATTERY_MAX 8.4
//#define BATTERY_MIN 6.0
#define BATTERY_MIN 7.0

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
}

unsigned long lastBlink = 0;
unsigned long lastBatt = 0;
void loop()
{
    static int beamCount;
    static int lowBatteryCount;
    int mode = GREEN;
    //if (nonBlockDelay(&lastBlink, 5000)) {
    //mode = REDFLASH;
    //}

    if (nonBlockDelay(&lastBatt, 1000)) {
        float batteryVoltage = voltageMeasurement();
        //Serial.println(batteryVoltage);

        if (batteryVoltage < BATTERY_MIN) {
            lowBatteryCount++;
            Serial.print(batteryVoltage);
            Serial.print(" ");
            Serial.println(lowBatteryCount);
        }
        if (lowBatteryCount >= 10) {
            lowBattery();
        }
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

    //delay(10); // let voltage stabilize for a moment

    for (int i = 0; i < 4; i++) {
        analogAverage += analogRead(BATTIN);
    }
    analogAverage /= 4;

    return analogAverage * (maxVolts / 1024);
}
