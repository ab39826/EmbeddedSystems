// Timer0A.c
// Runs on LM3S1968
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 14, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
  Program 7.5, example 7.6

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
 
 #include "Timer0A.h"

//this is for timer0A
#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable //need 19,21, 23, 35
#define NVIC_EN0_INT21					0x00200000	
#define NVIC_EN0_INT23					0x00800000
#define NVIC_EN0_INT35					0x00000008 //this should be called EN1 I'll maybe change this later
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_EN1_R							(*((volatile unsigned long *)0xE000E104))  //IRQ 32 to like 45 or whaatever

#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
#define NVIC_PRI5_R             (*((volatile unsigned long *)0xE000E414)) 
#define NVIC_PRI8_R             (*((volatile unsigned long *)0xE000E420))


#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_MIS_R            (*((volatile unsigned long *)0x40030020))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER0_TAR_R            (*((volatile unsigned long *)0x40030048))


//this is supposed to be for timer1A
#define TIMER1_CFG_R            (*((volatile unsigned long *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile unsigned long *)0x40031004))
#define TIMER1_CTL_R            (*((volatile unsigned long *)0x4003100C))
#define TIMER1_IMR_R            (*((volatile unsigned long *)0x40031018))
#define TIMER1_MIS_R            (*((volatile unsigned long *)0x40031020))
#define TIMER1_ICR_R            (*((volatile unsigned long *)0x40031024))
#define TIMER1_TAILR_R          (*((volatile unsigned long *)0x40031028))
#define TIMER1_TAPR_R           (*((volatile unsigned long *)0x40031038))
#define TIMER1_TAR_R            (*((volatile unsigned long *)0x40031048))

//this is supposed to be for timer2A
#define TIMER2_CFG_R            (*((volatile unsigned long *)0x40032000))
#define TIMER2_TAMR_R           (*((volatile unsigned long *)0x40032004))
#define TIMER2_CTL_R            (*((volatile unsigned long *)0x4003200C))
#define TIMER2_IMR_R            (*((volatile unsigned long *)0x40032018))
#define TIMER2_MIS_R            (*((volatile unsigned long *)0x40032020))
#define TIMER2_ICR_R            (*((volatile unsigned long *)0x40032024))
#define TIMER2_TAILR_R          (*((volatile unsigned long *)0x40032028))
#define TIMER2_TAPR_R           (*((volatile unsigned long *)0x40032038))
#define TIMER2_TAR_R            (*((volatile unsigned long *)0x40032048))

//this is supposed to be for timer3A
#define TIMER3_CFG_R            (*((volatile unsigned long *)0x40033000))
#define TIMER3_TAMR_R           (*((volatile unsigned long *)0x40033004))
#define TIMER3_CTL_R            (*((volatile unsigned long *)0x4003300C))
#define TIMER3_IMR_R            (*((volatile unsigned long *)0x40033018))
#define TIMER3_MIS_R            (*((volatile unsigned long *)0x40033020))
#define TIMER3_ICR_R            (*((volatile unsigned long *)0x40033024))
#define TIMER3_TAILR_R          (*((volatile unsigned long *)0x40033028))
#define TIMER3_TAPR_R           (*((volatile unsigned long *)0x40033038))
#define TIMER3_TAR_R            (*((volatile unsigned long *)0x40033048))


#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM TimerA Time-Out Raw
                                            // Interrupt
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control
#define SYSCTL_RCGC1_TIMER1 		0x00020000 //timer 1 Clock Gating Control
#define SYSCTL_RCGC1_TIMER2 		0x00040000 //timer 1 Clock Gating Control
#define SYSCTL_RCGC1_TIMER3 		0x00080000 //timer 1 Clock Gating Control

//0000 0000 0000 0000

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);  // user function
// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none

//this will be for buttons
void Timer0A_Init(unsigned short period){ 
  int x = 0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; // 0) activate timer0
  while(x<4){
		x++;
	}
	TIMER0_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER0_TAILR_R = period;         // 4) reload value
  TIMER0_TAPR_R = 0;              // 5) 1us timer0A
  TIMER0_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER0_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x1FFFFFFF)|0x20000000; // 8) priority 1
  NVIC_EN0_R |= NVIC_EN0_INT19;    // 9) enable interrupt 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
}

void Timer0A_Enable_Interrupts(unsigned short period){
	NVIC_EN0_R |= NVIC_EN0_INT19;
	TIMER0_TAILR_R = period;
}


void Timer0A_Disable_Interrupts(){
NVIC_EN0_R &= ~NVIC_EN0_INT19;
}


//this will be for DAC Timer output
void Timer1A_Init(unsigned short period){  
	 int x = 0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER1; // 0) activate timer0
  while(x<4){
		x++;
	}
	TIMER1_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER1_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER1_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER1_TAILR_R = period;         // 4) reload value
  TIMER1_TAPR_R = 0;              // 5) 1us timer0A
  TIMER1_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER1_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFF1FFFF)|0x00040000; // 8) priority 2 not sure if I set this up properly
  NVIC_EN0_R |= NVIC_EN0_INT21;    // 9) enable interrupt 19 in NVIC
  TIMER1_CTL_R |= 0x00000001;      // 10) enable timer0A
	
	

}
void Timer1A_Enable_Interrupts(unsigned short period){
	
	
	NVIC_EN0_R |= NVIC_EN0_INT21;
	TIMER1_TAILR_R = period;
}


void Timer1A_Disable_Interrupts(){
	int yuck = 0;
	yuck = NVIC_EN0_R & ~NVIC_EN0_INT21;
	NVIC_EN0_R = yuck;
}

//harmony line
void Timer2A_Init(unsigned short period){
	 int x = 0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER2; // 0) activate timer0
  while(x<4){
		x++;
	}
	TIMER2_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER2_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER2_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER2_TAILR_R = period;         // 4) reload value
  TIMER2_TAPR_R = 0;              // 5) 1us timer0A
  TIMER2_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER2_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF1FFF)|0x00006000; // 8) priority 3
  NVIC_EN0_R |= NVIC_EN0_INT23;    // 9) enable interrupt 23 in NVIC
  TIMER2_CTL_R |= 0x00000001;      // 10) enable timer0A
	
	

}

void Timer2A_Enable_Interrupts(unsigned short period){
	NVIC_EN0_R |= NVIC_EN0_INT23;
	TIMER2_TAILR_R = period;
}


void Timer2A_Disable_Interrupts(){
NVIC_EN0_R &= ~NVIC_EN0_INT23;
}
//melody line
void Timer3A_Init(unsigned short period){
	 int x = 0;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER3; // 0) activate timer0
  while(x<4){
		x++;
	}
	TIMER3_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER3_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER3_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER3_TAILR_R = period;         // 4) reload value
  TIMER3_TAPR_R = 0;              // 5) 1us timer0A
  TIMER3_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER3_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0xFFFFFF1F)|0x00000060; // 8) priority 3
  NVIC_EN1_R |= NVIC_EN0_INT35;    // 9) enable interrupt 19 in NVIC
  TIMER3_CTL_R |= 0x00000001;      // 10) enable timer0A
	
	

}

void Timer3A_Enable_Interrupts(unsigned short period){
	NVIC_EN1_R |= NVIC_EN0_INT35;
	TIMER3_TAILR_R = period;
}


void Timer3A_Disable_Interrupts(){
	NVIC_EN1_R &= ~NVIC_EN0_INT35;
}