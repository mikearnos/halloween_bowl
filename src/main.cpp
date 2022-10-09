#include <arduino.h>
#include "pacifica.h"
#include "hardware.h"

bool nonBlockDelay(unsigned long* last, unsigned int delay);
float voltageMeasurement(void);
void batteryLoop(void);

void setup()
{
    pinMode(IRSENSOR, INPUT);

    pinMode(BATTIN, INPUT);
    analogReference(INTERNAL); // 1.1 volts

    Serial.begin(115200);
    pacificaSetup();
}

unsigned long lastBlink = 0;

void loop()
{
    static unsigned long lastHand;
    int mode = GREEN;

    if (!digitalRead(IRSENSOR)) {
        if (nonBlockDelay(&lastHand, 5000)) {
            Serial.print("hand detected ");
            Serial.println(millis());
            //mode = REDFLASH;
        }
    }

    pacificaLoop(mode);
    batteryLoop();
}

void batteryLoop(void)
{
    static int lowBatteryCount;
    static unsigned long lastBatt;
    if (nonBlockDelay(&lastBatt, 1000)) {
        float batteryVoltage = voltageMeasurement();

        if (batteryVoltage < VOLTAGE_MIN) {
            lowBatteryCount++;
            Serial.print("Low Battery ");
            Serial.println(batteryVoltage);
            Serial.print("v, count: ");
            Serial.println(lowBatteryCount);
        } else {
            Serial.print("Battery: ");
            Serial.println(batteryVoltage);
        }
        if (lowBatteryCount >= 10) {
            Serial.println("Low Battery Shutdown");
            fastLEDLowBattery();
        }
    }
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

float voltageMeasurement(void)
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
