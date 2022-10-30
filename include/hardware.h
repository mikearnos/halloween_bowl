#define BATTIN A0
#define COMMS1 PD3 // pink
#define COMMS2 PD2 // purple
//#define DATA_PIN 4  in pacifica.cpp
#define DFRX PD5
#define DFTX PD6

#define TRIGGER COMMS1
#define ARMED COMMS2

#define SND_CAPCOM 1

const float R1 = 99400;
const float R2 = 14720;
//const float Vref = 1.135; // the voltage measured on AREF
const float Vref = 1.05; // the voltage measured on AREF

#define BATTERY_COUNT 2
#define BATTERY_MAX 4.2
#define BATTERY_MIN 3.0
#define VOLTAGE_MAX (BATTERY_MAX * BATTERY_COUNT)
#define VOLTAGE_MIN (BATTERY_MIN * BATTERY_COUNT)
