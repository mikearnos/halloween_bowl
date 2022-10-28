#define GREEN 0
#define REDFLASH 1

void redFlash(uint8_t);
void purpleFlash(uint8_t);

extern void fastLEDLowBattery(void);
extern void pacificaSetup(void);
extern void pacificaLoop(int);

extern int redFlashTimer;
