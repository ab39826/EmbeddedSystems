void buttonInit(void); //sets all buttons to be inputs

void buttonDisableInterrupts(void);

void setChangeTrackFunction(void (*newFunction) (void));
void setRecordFunction(void (*newFunction) (void));
void setPlayBackFunction(void (*newFunction) (void));
void setChangeInstrumentFunction(void (*newFunction) (void));

void toggleLED (int index);




