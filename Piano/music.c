#include "music.h"
#include "Timer0A.h"
#include "DAC.h"

#define SINE_TABLE_LENGTH 128


#define SINE_TABLE_AVERAGE 800
#define TIMER2_PERIOD 20000


//this is for timer0A
#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable //need 19,21, 23, 35
#define NVIC_EN0_INT21					0x00200000	
#define NVIC_EN0_INT23					0x00800000
#define NVIC_EN0_INT35					0x00000008 //this should be called EN1 I'll maybe change this later
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_EN1_R							(*((volatile unsigned long *)0xE000E104))  //IRQ 32 to like 45 or whaatever

#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register

#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))



#define TIMER1_ICR_R            (*((volatile unsigned long *)0x40031024))


#define TIMER2_ICR_R            (*((volatile unsigned long *)0x40032024))


#define TIMER3_ICR_R            (*((volatile unsigned long *)0x40033024))



#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM TimerA Time-Out Raw


//BUTTON PORT DEFS YO
#define PD0 (*((volatile unsigned long *) 0x40007004))
#define PD1 (*((volatile unsigned long *) 0x40007008))
#define PD2 (*((volatile unsigned long *) 0x40007010))
#define PD3 (*((volatile unsigned long *) 0x40007020))
#define PD4 (*((volatile unsigned long *) 0x40007040))
#define PD5 (*((volatile unsigned long *) 0x40007080))
#define PD6 (*((volatile unsigned long *) 0x40007100))
#define PD7 (*((volatile unsigned long *) 0x40007200))

#define GPIO_PORTD_RIS_R (*((volatile unsigned long *) 0x40007414))
#define GPIO_PORTD_ICR_R (*((volatile unsigned long *) 0x4000741C))

void startCurrentHarmonyNote(void);
void startCurrentMelodyNote(void);


const unsigned short sinWaveTable[SINE_TABLE_LENGTH] = {
  800, 839, 878, 917, 956, 994, 1032, 1070, 1106, 1142, 1177, 1211, 1244, 1277, 1308, 1337, 
  1366, 1393, 1418, 1443, 1465, 1486, 1506, 1523, 1539, 1553, 1566, 1576, 1585, 1591, 1596, 
  1599, 1600, 1599, 1596, 1591, 1585, 1576, 1566, 1553, 1539, 1523, 1506, 1486, 1465, 1443, 
  1418, 1393, 1366, 1337, 1308, 1277, 1244, 1211, 1177, 1142, 1106, 1070, 1032, 994, 956, 
  917, 878, 839, 800, 761, 722, 683, 644, 606, 568, 530, 494, 458, 423, 389, 356, 323, 292, 
  263, 234, 207, 182, 157, 135, 114, 94, 77, 61, 47, 34, 24, 15, 9, 4, 1, 0, 1, 4, 9, 15, 
  24, 34, 47, 61, 77, 94, 114, 135, 157, 182, 207, 234, 263, 292, 323, 356, 389, 423, 458, 
  494, 530, 568, 606, 644, 683, 722, 761
};



int sineIndex;
int sinceLastTick;
int sinceLastNote;
int isPlaying;

const unsigned short tickLengths[SINE_TABLE_LENGTH] = {
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

Envelope currentEnvelope;

void press0 (void) {
	sineIndex = 0;
	sinceLastTick = 0;
	sinceLastNote = 0;
	isPlaying = 1;
}

void release0 (void) {
	isPlaying = 0;
}

void musicInit(void) {
	
	sineIndex = 0;
	sinceLastTick = 0;
	sinceLastNote = 0;
	isPlaying = 0;
	Timer0A_Init(TIMER2_PERIOD);
}

void Timer0A_Handler(void){
	sinceLastTick++;
	if (sinceLastTick > tickLengths[sineIndex]) {
		sineIndex = (sineIndex + 1)%SINE_TABLE_LENGTH;
	};
	if (isPlaying) {
		DAC_Out(sinWaveTable[sineIndex]);
	} else {
		DAC_Out (SINE_TABLE_AVERAGE);
	}
}

void GPIOPortD_Handler(void){
 if(GPIO_PORTD_RIS_R&0x01){
	if(!PD0) {
	  if (!isPlaying) {
			press0();
		}
	} else {
		if (isPlaying) {
			release0();
		}
	}
	GPIO_PORTD_ICR_R |= 0x01;
}
	
 if(GPIO_PORTD_RIS_R&0x02){
	if(!PD1) {
		
	}
	GPIO_PORTD_ICR_R |= 0x02;
}

 if(GPIO_PORTD_RIS_R&0x04){
	if(!PD2) {
		
	}
	GPIO_PORTD_ICR_R |= 0x04;
}
	
 if(GPIO_PORTD_RIS_R&0x08){
	if(!PD3) {
		
	}
	GPIO_PORTD_ICR_R |= 0x08;
}
	
 if(GPIO_PORTD_RIS_R&0x10){
	if(!PD4) {
		
	}
	GPIO_PORTD_ICR_R |= 0x10;
}
	
 if(GPIO_PORTD_RIS_R&0x20){
	if(!PD5) {
		
	}
	GPIO_PORTD_ICR_R |= 0x20;
}
	

 if(GPIO_PORTD_RIS_R&0x40){
	if(!PD6) {
		
	}
	GPIO_PORTD_ICR_R |= 0x40;
}
	

 if(GPIO_PORTD_RIS_R&0x80){
	if(!PD7) {
		
	}
	GPIO_PORTD_ICR_R |= 0x80;
}
	


}

