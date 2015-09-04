void RIT128x96x4_DrawClock(int mode);
void RIT128x96x4_DrawAnalogClock(void);
void RIT128x96x4_DrawDigitalClock(void);
void twelveHrClockTick(unsigned long seconds);
void RIT128x96x4_DrawMenuItem(const char * name, unsigned char brightness);
void RIT128x96x4_DrawString(const char* string, int x, int y, unsigned char brightness);
void RIT128x96x4_DrawImage(const unsigned char* image, int width, int height, int xOff, int yOff);
void RIT128x96x4_BMP(unsigned long xpos, unsigned long ypos, const unsigned char *Buffer);
void RIT128x96x4_ClearImage(void);
void RIT128x96x4_WhiteImage(void);
void drawWader(void);
void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color);
void RIT128x96x4_ShowImage(void);
void setPixel(int x, int y, unsigned char color);
void analogClockTick(unsigned long seconds);
void digitalClockTick(unsigned long seconds);
void clockTick(unsigned long seconds, int mode);
void drawAlarm(int isAlarmOn, unsigned long alarmTime);


