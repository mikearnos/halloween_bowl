#include <arduino.h>
#include "pacifica.h"
#include "hardware.h"
#include <Servo.h>

bool nonBlockDelay(unsigned long* last, unsigned int delay);
float voltageMeasurement(void);
void batteryLoop(void);
extern void dfSetup(void);
extern void dfPlay(int);

Servo servo1;
Servo servo2;
Servo servo3;

void setup()
{
    pinMode(IRSENSOR, INPUT);

    pinMode(BATTIN, INPUT);
    analogReference(INTERNAL); // 1.1 volts, Vref must match for proper calculation

    //servo1.attach(9);
    //servo2.attach(10);
    //servo3.attach(11);

    pacificaSetup();

    Serial.begin(115200);

    dfSetup();
    dfPlay(SND_BARREL);
}

unsigned long lastBlink = 0;
unsigned long lastPlayer = 0;

void loop()
{
    //delay(1000);
    //DF1201S.setVol(/*VOL = */ 10);
    if (nonBlockDelay(&lastPlayer, 10000)) {
        //DF1201S.playFileNum(SND_BARREL);
    }
    //static unsigned long lastHand;
    int mode = GREEN;

    /*if (!digitalRead(IRSENSOR)) {
        if (nonBlockDelay(&lastHand, 5000)) {
            Serial.print("hand detected ");
            Serial.println(millis());
            //mode = REDFLASH;
        }
    }*/

    //servoLoop();
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

void servoLoop(void)
{
    //static unsigned long lastServo;
    static int direction, i;
    //    if (nonBlockDelay(&lastServo, 10)) {
    if (direction == 0) {
        servo1.write(i);
        servo2.write(i);
        servo3.write(i);
        if (i == 150) {
            direction = 1;
        } else {
            i++;
        }
    } else {
        servo1.write(i);
        servo2.write(i);
        servo3.write(i);
        if (i == 0) {
            direction = 0;
        } else {
            i--;
        }
    }
    //}
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
