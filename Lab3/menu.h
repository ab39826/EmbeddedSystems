typedef struct MenuItem {
	char name[20];
	void (*selectFunction) (void);
} MenuItem;

void alarmSet(void);
void timeSet(void);
void modeSet(void);
void alarmOnOff(void);

extern int MENU_SIZE;
extern MenuItem Menu[];
extern volatile int mode;