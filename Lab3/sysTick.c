#include "sysTick.h"
#include "../inc/hw_nvic.h"


#define NVIC_ST_CTRL_R             (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_CURRENT_R             (*((volatile unsigned long *)0xE000E018))
#define NVIC_SYS_PRI3_R             (*((volatile unsigned long *)0xE000ED20))
#define NVIC_ST_RELOAD_R             (*((volatile unsigned long *)0xE000E014))

void sysTickInit(void){
SYSCTL_RCGC2_R |=;
Counts = 0;
NVIC_ST_CTRL_R = 0;
NVIC_ST_CURRENT_R = 0;
NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R_R&0x00FFFFFF) |0x40000000; //priority 2
NVIC_ST_CTRL_R = 0x00000000; //enable with core clock and interrupts
EnableInterrupts();
}

void sysTickEnable(unsigned long period){
NVIC_ST_RELOAD_R = period-1;
NVIC_ST_CTRL_R = 0x00000007;
}

void sysTickDisable(void){
NVIC_ST_CTRL_R = 0x00000000;
}

