#include "menu.h"
#include "timing.h"
#include "buttons.h"
#include "clockOLED.h"

#define TIME_MENU_SIZE 2
MenuItem TimeSetMenu[] = {
	{"Hours", 0},
	{"Minutes", 0}
};

void timeSet(void){
	int menuItem = 0;
	
	RIT128x96x4_ClearImage();
	RIT128x96x4_DrawClock(mode);
	drawAlarm(alarmOn,alarmTime);
	RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
	while(1) {
		clockTick(clockTime, mode);
		RIT128x96x4_ShowImage();
		RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		if (getLeftPress()) {
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 0);
			menuItem = menuItem - 1;
			if (menuItem < 0) {
				menuItem += TIME_MENU_SIZE;
			}
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		}
		if (getRightPress()) {
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 0);
			menuItem = (menuItem + 1) % TIME_MENU_SIZE;
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		}
		if (getUpPress()) {
			//0 is hours, 1 is minutes
			if(menuItem) {
				clockTime += 60;
			} else {
				clockTime += 3600;
			}
			clockTime %= DAY;
		}
		if (getDownPress()) {
			clockTime += DAY;
			if(menuItem) {
				clockTime -= 60;
			} else {
				clockTime -= 3600;
			}
			clockTime %= DAY;
		}
		if(getSelectPress()){
			//need to figure out what button was pressed.
			break;
		}
	}
}

void alarmSet(void){
	int menuItem = 0;
	int alarmTemp = alarmOn;
	alarmOn = 1;
	RIT128x96x4_ClearImage();
	RIT128x96x4_DrawClock(mode);
	drawAlarm(alarmOn,alarmTime);
	RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
	while(1) {
		clockTick(clockTime, mode);
		drawAlarm(alarmOn,alarmTime);
		RIT128x96x4_ShowImage();
		RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		if (getLeftPress()) {
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 0);
			menuItem = menuItem - 1;
			if (menuItem < 0) {
				menuItem += TIME_MENU_SIZE;
			}
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		}
		if (getRightPress()) {
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 0);
			menuItem = (menuItem + 1) % TIME_MENU_SIZE;
			RIT128x96x4_DrawMenuItem(TimeSetMenu[menuItem].name, 15);
		}
		if (getUpPress()) {
			//0 is hours, 1 is minutes
			if(menuItem) {
				alarmTime += 60;
			} else {
				alarmTime += 3600;
			}
			alarmTime %= DAY;
		}
		if (getDownPress()) {
			alarmTime += DAY;
			if(menuItem) {
				alarmTime -= 60;
			} else {
				alarmTime -= 3600;
			}
			alarmTime %= DAY;
		}
		if(getSelectPress()){
			alarmOn = alarmTemp;
			break;
		}
	}
	
}
	
void modeSet(void){
mode ^= 0x01;	
}
	
void alarmOnOff(void){
	alarmOn ^= 0x01; //Toggle the alarm
}