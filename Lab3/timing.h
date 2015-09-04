void timerTickInit(unsigned long period);
void timerTickSetPeriod(unsigned long period); //sets the period for the ticker
void timerTickEnable(unsigned long period);
void timerTickDisable(void);
void Delay(unsigned long ulCount);

extern unsigned volatile long clockTime; // this is seconds into the day
extern unsigned volatile long alarmTime; //this is seconds into the day.
extern volatile int alarmOn; //1 if on, 0 if off
extern volatile int mode;

#define DAY 86400