#include "timing.h"

#include "../inc/hw_nvic.h"

#define NVIC_ST_CTRL_R             (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_CURRENT_R             (*((volatile unsigned long *)0xE000E018))
#define NVIC_SYS_PRI3_R             (*((volatile unsigned long *)0xE000ED20))
#define NVIC_ST_RELOAD_R             (*((volatile unsigned long *)0xE000E014))

void timerTickInit(unsigned long period){
NVIC_ST_CTRL_R = 0;
NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) |0x40000000; //priority 2
timerTickEnable(period);
}

void timerTickEnable(unsigned long period){
NVIC_ST_RELOAD_R = period-1;
NVIC_ST_CURRENT_R = 0;
NVIC_ST_CTRL_R = 0x00000007;
}

void timerTickDisable(void){
NVIC_ST_CTRL_R = 0x00000000;
}

void timerTickSetPeriod(unsigned long period){
	NVIC_ST_RELOAD_R = period - 1;
	NVIC_ST_CURRENT_R = 0;
}

void timerTickWait(unsigned long cycles){
	
}

__asm (void)
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

	
	