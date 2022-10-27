#define BATTIN A1
#define TRIGGER PD2
//#define TRIGGER2 PD3 reserved
//#define DATA_PIN 4  in pacifica.cpp
#define DFRX PD5
#define DFTX PD6

#define SND_BARREL 1
#define SND_PIPE 2
#define SND_CAPCOM 3
#define SND_DONT 4
#define SND_TURN_OFF 5
#define SND_MY_MAN 6

const float R1 = 99400;
const float R2 = 14720;
const float Vref = 1.135; // the voltage measured on AREF

#define BATTERY_COUNT 2
#define BATTERY_MAX 4.2
#define BATTERY_MIN 3.0
#define VOLTAGE_MAX (BATTERY_MAX * BATTERY_COUNT)
#define VOLTAGE_MIN (BATTERY_MIN * BATTERY_COUNT)
