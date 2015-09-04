//this is a version that doesn't work currently.

// LogoTestMain.c
// Runs on LM3S1968 or LM3S8962
// Joseph Lubars and Anurag Banerjee
// September 17, 2013

/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
 
#include "hw_types.h"
#include "sysctl.h"
#include "lm3s1968.h"
#include "rit128x96x4.h"
#include "logo.h"
#include "speaker.h"
#include "clockOLED.h"
#include "timing.h"
#include "speaker.h"
#include "buttons.h"
#include "menu.h"

#define GPIO_PORTG_DATA_R       (*((volatile unsigned long *)0x400263FC))
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_AFSEL_R      (*((volatile unsigned long *)0x40026420))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOG      0x00000040  // port G Clock Gating Control

int MENU_SIZE = 4;

MenuItem Menu[] = {
	{"Set Alarm", &alarmSet},
	{"Set Time", &timeSet},
	{"Set Mode", &modeSet},
	{"Alarm On/Off", &alarmOnOff}
};

unsigned volatile long clockTime; // this is seconds into the day
unsigned volatile long alarmTime; //this is seconds into the day.
int menuItem = 0;
volatile int alarmOn = 0;
volatile int mode = 0;

void SysTick_Handler(){
	clockTime++;
	clockTime = clockTime%DAY;
	  //GPIO_PORTG_DATA_R = GPIO_PORTG_DATA_R^0x04; // toggle PG2
	
}

void refreshDisplay(int menuItem) {
	RIT128x96x4_ClearImage();
  RIT128x96x4_DrawClock(mode);
	RIT128x96x4_DrawMenuItem(Menu[menuItem].name, 15);
	drawAlarm(alarmOn, alarmTime);
	RIT128x96x4_ShowImage();
}

#define C5 523
#define G4 392
#define Eb5 622
#define G5 784
#define Ab5 831
#define B4 494
#define Ab4 415
#define QUARTER 8000000

void rest(int time) {
	PWM0_Init(C5, 0);
	Delay(time);
}

void songy(){
	unsigned short frequency = 880; //A5
	unsigned int loudness = 100;
	drawWader();
	while(true) {

	PWM0_Init(C5,loudness);
	Delay(QUARTER*7/8);
	rest(QUARTER/8);
	if(getSelectPress()){break;}	
	PWM0_Init(C5,loudness);
	Delay(QUARTER*7/8);
	rest(QUARTER/8);
	if(getSelectPress()){break;}
	PWM0_Init(C5,loudness);
	Delay(QUARTER*7/8);
	rest(QUARTER/8);
	if(getSelectPress()){break;}
	// C C C 
	PWM0_Init(G4, loudness);
	Delay(QUARTER*3/4);
	if(getSelectPress()){break;}	
	PWM0_Init(Eb5,loudness);
	Delay(QUARTER/4); 
	if(getSelectPress()){break;}		
	PWM0_Init(C5,loudness);
	Delay(QUARTER*7/8);
	rest(QUARTER/8);
	if(getSelectPress()){break;}	
	// G Eb C
	PWM0_Init(G4, loudness);
	Delay(QUARTER*3/4);
	if(getSelectPress()){break;}	
	PWM0_Init(Eb5,loudness);
	Delay(QUARTER/4); 
	if(getSelectPress()){break;}		
	PWM0_Init(C5,loudness);
	Delay(QUARTER*2);
	if(getSelectPress()){break;}	
	// G Eb C
	loudness += 50;
	}
	refreshDisplay(menuItem);
	PWM0_Init(frequency,0);	

	
}


int main(void){ 
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
	RIT128x96x4Init(1000000);
	buttonInit();
	timerTickInit(50000000);           // initialize SysTick timer
  GPIO_PORTG_DIR_R |= 0x04; // make PG2 out (built-in LED)
  GPIO_PORTG_AFSEL_R &= ~0x04;// disable alt funct on PG2
  GPIO_PORTG_DEN_R |= 0x04; // enable digital I/O on PG2  
  
	refreshDisplay(menuItem);
   //speakerInit();
  //Delay(1000000000);        // delay 60 sec at 50 MHz 
  //RIT128x96x4DisplayOff(); // screen saver
  buttonEnableInterrupts();
	//songy();
	RIT128x96x4_DrawClock(0);
	while(1){
		GPIO_PORTG_DATA_R |= 0x04; // toggle PG2
		clockTick(clockTime, mode);
		RIT128x96x4_ShowImage();
		GPIO_PORTG_DATA_R &= ~0x04; // toggle PG2
		
		
		if(alarmOn){
		if((alarmTime/60)==(clockTime/60)){
			//get a second offset. If this falls within 10 seconds, then set alarm. else
			signed int alarmSec = alarmTime %60;
			signed int clockSec = clockTime%60;
			signed int timeDiff = clockSec - alarmSec;
			if((timeDiff>-10)&&(timeDiff<10)){
				songy();
			}		
		}
	}
		if (getLeftPress()) {
			RIT128x96x4_DrawMenuItem(Menu[menuItem].name, 0);
			menuItem = menuItem - 1;
			if (menuItem < 0) {
				menuItem += MENU_SIZE;
			}
			RIT128x96x4_DrawMenuItem(Menu[menuItem].name, 15);
		}
		if (getRightPress()) {
			RIT128x96x4_DrawMenuItem(Menu[menuItem].name, 0);
			menuItem = (menuItem + 1) % MENU_SIZE;
			RIT128x96x4_DrawMenuItem(Menu[menuItem].name, 15);
		}
		
		if(getSelectPress()){
			//need to figure out what button was pressed.
			Menu[menuItem].selectFunction();
			refreshDisplay(menuItem);
		}
		
	};
}


