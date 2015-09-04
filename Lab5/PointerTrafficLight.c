// PointerTrafficLight.c
// Runs on LM3S1968
// Use a pointer implementation of a Moore finite state machine to operate
// a traffic light.
// Daniel Valvano
// June 15, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 3.1, Example 3.1

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

// north facing car detector connected to PE1 (1=car present)
// east facing car detector connected to PE0 (1=car present)
// east facing red light connected to PF5
// east facing yellow light connected to PF4
// east facing green light connected to PF3
// north facing red light connected to PF2
// north facing yellow light connected to PF1
// north facing green light connected to PF0

#include "PLL.h"
#include "SysTick.h"
#include "Timer0A.h"
#include "buttons.h"
#include "music.h"
#include "DAC.h"

#define GPIO_PORTE_IN           (*((volatile unsigned long *)0x4002400C)) // bits 1-0
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))

#define GPIO_PORTD_DATA_R				(*((volatile unsigned long *) 0x40007000))
#define PortD1dash4Output				(*((volatile unsigned long *) 0x4000703C))
#define GPIO_PORTD_OUT          (*((volatile unsigned long *)0x400070FC)) // bits 5-0
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001

#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_MIS_R            (*((volatile unsigned long *)0x40030020))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER0_TAR_R            (*((volatile unsigned long *)0x40030048))
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


#define G2 98
#define Ab2 104
#define A2 110
#define Bb2 117
#define B2 123
#define C3 131
#define Db3 139
#define D3 147
#define Eb3 156
#define E3 165
#define F3 175
#define Gb3 185
#define G3 196
#define Ab3 208
#define A3 220
#define Bb3 233
#define B3 247
#define C4 262
#define Db4 277
#define D4 294
#define Eb4 311
#define E4 330
#define F4 349
#define Gb4 370
#define G4 392
#define Ab4 415
#define A4 440
#define Bb4 466
#define B4 494
#define C5 523
#define Db5 554
#define D5 587
#define Eb5 622
#define E5 659
#define F5 698
#define Gb5 740
#define G5 784
#define Ab5 831
#define A5 880
#define Bb5 932
#define B5 988
#define C6 1047

#define C5 523
#define Cs5 554

#define rest 0

int Part1Frequencies[] = {G4, C4, Eb4, F4, G4, C4, Eb4, F4, G4, C4, Eb4, F4, G4, C4, Eb4, F4,
	G4, C4, E4, F4, G4, C4, E4, F4, G4, C4, E4, F4, G4, C4, E4, F4,
	0, D4, G3, Bb3, C4, D4, G3, Bb3, C4, D4, G3, Bb3, C4, D4, G3, Bb3,
	0, C4, F3, Ab3, Bb3, C4, F3, Ab3, Bb3, C4, F3, Ab3, Bb3, C4, F3, Ab3,
	G4, C4, Eb4, F4, G4, C4, Eb4, F4, D5, G4, Bb4, C5, D5, G4, Bb4, C5, D5, G4, Bb4, C5, D5, G4, Bb4,
	F4, Bb3, D4, Eb4, D4, Bb3, C5, F4, Ab4, Bb4, C5, F4, Ab4, Bb4, C5, F4, Ab4, Bb4, C5, F4, Ab4,
	};
int Part1Durations[] = {2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1,
	2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1,
	24, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 
	24, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 
	6, 6, 1, 1, 4, 4, 1, 1,   2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2,
	6, 6, 4, 2, 4, 2,   2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2,
	};
int Part1Length = sizeof(Part1Frequencies)/sizeof(int);

int Part2Frequencies[] = {0, 0, G4, C4, Eb4, F4, G4, C4, Eb4, F4, D4,
	F4, Bb3, Eb4, D4, F4, Bb3, Eb4, D4, C4,
	G5, C5, Eb5, F5, G5, C5, Eb5, F5, D5,
	F5, Bb4, D5, Eb5, D5, Bb4, C5,
	};
int Part2Durations[] = {24, 24, 6, 6, 1, 1, 4, 4, 1, 1, 24,
	6, 6, 1, 1, 4, 6, 1, 1, 22,
	6, 6, 1, 1, 4, 4, 1, 1, 24,
	6, 6, 4, 2, 4, 2, 24,
	};
	
int Part2Length = sizeof(Part2Frequencies)/sizeof(int);

//Tempo should be at least 300-400 bpm

int trebleFrequencies[] = {A4, E5, D5, Cs5, B4, A5, E5, D5, Cs5, B4, A5, E5, D5, Cs5, D5, B4};
int trebleDurations[] = {6, 3, 1, 1, 1, 6, 3, 1, 1, 1, 6, 3, 1, 1, 1, 6};
int trebleLength = sizeof(trebleFrequencies)/sizeof(int);


int envelopeIndex = 0;


int beautyEnvelope[] = {20, 40, 60, 80, 100, 90, 80, 70, 60, 50, 40 , 30, 20 , 10, 0};

//int trebleFrequencies[] = {A5};
//int trebleDurations[] = {6};
//int trebleLength = sizeof(trebleFrequencies)/sizeof(int);

int bassFrequencies[] = {C5, rest, F4};
int bassDurations[] = {10, 1, 3};
int bassLength = 	sizeof(bassFrequencies)/sizeof(int);

Song starWars = {
	sizeof(trebleFrequencies)/sizeof(int),
	sizeof(bassFrequencies)/sizeof(int),
	trebleFrequencies,
	bassFrequencies,
	trebleDurations,
	bassDurations
};

Song throneGames = {
	sizeof(Part1Frequencies)/sizeof(int),
	sizeof(Part2Frequencies)/sizeof(int),
	Part1Frequencies,
	Part2Frequencies,
	Part1Durations,
	Part2Durations
};

int testFrequencies[] = {C5};
int testDurations[] = {24};

int emptyFrequencies[] = {0};
int emptyDurations[] = {24};

Song test = {
	sizeof(testFrequencies) / sizeof(int),
	0,
	testFrequencies,
	emptyFrequencies,
	testDurations,
	emptyDurations
	};


 short mountainEnvelope (signed short amplitude, int time, int length, int next){
	//time left in note, length time point for next note
	if(length-time<(length/2)){
	//start increasing
		amplitude = (amplitude *(length-time))/(length/2);
	}
	else{
		// start decaying
		amplitude = 2*(amplitude *(time))/(length);
	}
	return  amplitude;
}

unsigned short stacattoEnvelope (unsigned short amplitude, int time, int length, int next){
if(time<500){

amplitude = 0;
}
	
	
return amplitude;
}

short simpleEnvelope (short amplitude, int time, int length, int next){	
	
return amplitude;
}

short wubEnvelope (short amplitude, int time, int length, int next){

	if((length-time)<400){
	return amplitude;
	}
return 0;
}

const unsigned short sineTable[128] = {
 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 89, 90, 91, 92, 93, 93, 94, 95, 95, 96,
 97, 97, 98, 98, 98, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99,
 99, 99, 98, 98, 98, 97, 97, 96, 95, 95, 94, 93, 93, 92, 91, 90, 89, 89, 88, 87,
 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 71, 70, 69, 68,
 67, 67, 66, 65, 65, 64, 63, 63, 62, 62, 62, 61, 61, 61, 60, 60, 60, 60, 60, 60,
 60, 60, 60, 61, 61, 61, 62, 62, 62, 63, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70,
 71, 71, 72, 73, 74, 75, 76, 77, 78, 79
};

short sinusoidalEnvelope (short amplitude, int time, int length, int next) {
	return (int)(sineTable[time]) * amplitude / 100;
}

void changeMode(void) {
	envelopeIndex++;
	envelopeIndex %=4;
	switch(envelopeIndex)
	{
		case 0 :
			 playSong(&throneGames, &mountainEnvelope, 400);
		break;
		
		case 1 :
			playSong(&throneGames, &simpleEnvelope, 400);
		break;
		case 2 :
			playSong(&starWars, &mountainEnvelope,120);
		break;
		
		case 3:
			playSong(&starWars, &wubEnvelope,120);
		break;
		default:
			 playSong(&throneGames, &sinusoidalEnvelope, 400);
	}	
	
}



int main(void){
  PLL_Init();                  // configure for 50 MHz clock
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
 
	SysTick_Init();
  buttonInit();
	musicInit();
	DAC_Init(0xC000);
  
  // activate port F and port E
  //  SysTick_Init();              // initialize SysTick timer
  // GPIO_PORTD_DIR_R |= 0x0F;    // make PD3-0 out
  // GPIO_PORTD_AFSEL_R &= ~0x0F; // disable alt func on PF3-0
  // GPIO_PORTD_DEN_R |= 0x0F;    // enable digital I/O on PF3-0
  setSelectFunction(&changeMode);
  setLeftFunction(&rewindSong);
  setRightFunction(&pauseResumeSong);
  playSong(&test, &simpleEnvelope, 120);
	buttonEnableInterrupts();
  while(1){
  }
}
