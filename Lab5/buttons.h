void buttonInit(void); //sets all buttons to be inputs
void buttonEnableInterrupts(void); // sets the NVIC properties of the buttons
void buttonDisableInterrupts(void);

void setSelectFunction(void (*newFunction) (void));
void setLeftFunction(void (*newFunction) (void));
void setRightFunction(void (*newFunction) (void));




