#include <arduino.h>
#include "hardware.h"
#include "battery.h"

extern bool nonBlockDelay(unsigned long*, unsigned int);

void batteryLoop(void)
{
    static unsigned long lastBatt;
    static int lowBatteryCount;

    if (!nonBlockDelay(&lastBatt, 1000)) {
        return;
    }

    float batteryVoltage = voltageMeasurement();

    if (batteryVoltage == 0) {
        Serial.println("Battery calibrating...");
        return;
    }

    if (batteryVoltage < VOLTAGE_MIN) {
        lowBatteryCount++;
        Serial.print("Low Battery ");
        Serial.print(batteryVoltage);
        Serial.print("v, count: ");
        Serial.println(lowBatteryCount);
    } else {
        Serial.print("Battery: ");
        Serial.print(batteryVoltage);
        Serial.println("v");
    }
    if (lowBatteryCount >= 10) {
        Serial.println("Low Battery Shutdown");
        //fastLEDLowBattery();
    }
}

float voltageMeasurement(void)
{
    static float voltageLog[10];
    const float maxVolts = Vref * (R1 + R2) / R2;
    float analogAverage = 0;

    //delay(10); // let voltage stabilize for a moment

    for (int i = 9; i > 0; i--) {
        voltageLog[i] = voltageLog[i - 1];
    }

    voltageLog[0] = analogRead(BATTIN);

    if (voltageLog[9] == 0)
        return 0;

    for (int i = 0; i < 10; i++) {
        analogAverage += voltageLog[i];
    }
    analogAverage /= 10;

    return analogAverage * (maxVolts / 1024);
}
