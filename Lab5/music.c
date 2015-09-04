#include "music.h"
#include "Timer0A.h"
#include "DAC.h"

#define SINE_TABLE_LENGTH 128


#define SINE_TABLE_AVERAGE 800
#define TIMER2_PERIOD 2000


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



unsigned short melodyOutput;
int melodyEnabled;
int melodySineIndex;
int melodyTime;
unsigned short melodyTimerPeriod;
int melodyFinished;
unsigned short harmonyOutput;
int harmonyEnabled;
int harmonySineIndex;
int harmonyTime;
unsigned short harmonyTimerPeriod;
int harmonyFinished;

int numberInterrupts;
int currentMelodyNote;
int nextMelodyNote;
int currentHarmonyNote;
int nextHarmonyNote;

int harmonyNoteIndex;
int melodyNoteIndex;


int paused;

int beatsPerMinute;
Song* currentSong;
Envelope currentEnvelope;


void musicInit(void) {
	melodyOutput = 0;
	harmonyOutput = 0;
	melodyFinished = 1;
	harmonyFinished = 1;
	melodyEnabled = 0;
	harmonyEnabled = 0;
	Timer0A_Init(1000);
	Timer1A_Init(1000);
	Timer2A_Init(TIMER2_PERIOD);
}

void playSong(Song* song, Envelope envelope, int tempo) {
	currentSong = song;
	currentEnvelope = envelope;
	beatsPerMinute = tempo;
	rewindSong();
	resumeSong();
}

void pauseSong(void) {
	paused = 1;
}

void startCurrentMelodyNote(void) {	
	if (melodyNoteIndex >= currentSong->melodyLength) {
		melodyFinished = 1;
		melodyEnabled = 0;
		if (harmonyFinished) {
			rewindSong();
		}
		return;
	}
	if (currentSong->melodyFrequencies[melodyNoteIndex]) {
		melodyTimerPeriod = 50000000 / (SINE_TABLE_LENGTH * currentSong->melodyFrequencies[melodyNoteIndex]);
		melodyEnabled = 1;
		Timer0A_Enable_Interrupts(melodyTimerPeriod);
	} else {
		melodyEnabled = 0;
		//Timer0A_Disable_Interrupts();
	}
	currentMelodyNote = nextMelodyNote;
	nextMelodyNote += currentSong->melodyDurations[melodyNoteIndex] * (50000000 / TIMER2_PERIOD) * 60 / (beatsPerMinute);	
}

void startCurrentHarmonyNote(void) {
	if (harmonyNoteIndex >= currentSong->harmonyLength) {
		harmonyFinished = 1;
		harmonyEnabled = 0;
		if (melodyFinished) {
			rewindSong();
		}
		return;
	}
	if (currentSong->harmonyFrequencies[harmonyNoteIndex]) {
		harmonyTimerPeriod = 50000000 / (SINE_TABLE_LENGTH * currentSong->harmonyFrequencies[harmonyNoteIndex]);
		harmonyEnabled = 1;
		Timer1A_Enable_Interrupts(harmonyTimerPeriod);
	} else {
		harmonyEnabled = 0;
		//Timer1A_Disable_Interrupts();
	}
	currentHarmonyNote = nextHarmonyNote;
	nextHarmonyNote += currentSong->harmonyDurations[harmonyNoteIndex] * (50000000 / TIMER2_PERIOD) * 60 / (beatsPerMinute);	
}

void rewindSong(void) {
	numberInterrupts = 0;
	nextMelodyNote = 0;
	nextHarmonyNote = 0;
	melodyNoteIndex = 0;
	melodySineIndex = 0;
	harmonyNoteIndex = 0;
	harmonySineIndex = 0;
	melodyFinished = 0;
	harmonyFinished = 0;
	startCurrentMelodyNote();
	startCurrentHarmonyNote();
	pauseSong();
}

void pauseResumeSong(void) {
	if (paused) {
		resumeSong();
	} else {
		pauseSong();
	}
}

void resumeSong(void) {
	paused = 0;
}


void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  melodySineIndex = (melodySineIndex + 1) % SINE_TABLE_LENGTH;
  melodyOutput = sinWaveTable[melodySineIndex];
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout
  harmonySineIndex = (harmonySineIndex + 1) % SINE_TABLE_LENGTH;
  harmonyOutput = sinWaveTable[harmonySineIndex];
}

void Timer2A_Handler(void){
	unsigned short melody_dac = SINE_TABLE_AVERAGE;
	unsigned short harmony_dac = SINE_TABLE_AVERAGE;
	unsigned short tempura = sinWaveTable[0];
	
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer2A timeout
	if (paused) {
		return;
	}
  numberInterrupts++;
  if (numberInterrupts == nextMelodyNote) {
		melodyNoteIndex++;
		startCurrentMelodyNote();
  }
  if (numberInterrupts == nextHarmonyNote) {
    harmonyNoteIndex++;
		startCurrentHarmonyNote();
  }
	if (melodyEnabled) {
		melody_dac = 800 + currentEnvelope(((signed short) (melodyOutput) - 800), nextMelodyNote-numberInterrupts, nextMelodyNote-currentMelodyNote ,nextMelodyNote );
	}
	if (harmonyEnabled) {
		harmony_dac = 800 + currentEnvelope(((signed short) (harmonyOutput) - 800), nextHarmonyNote-numberInterrupts, nextHarmonyNote - currentMelodyNote, nextHarmonyNote );
	}
	
  DAC_Out(melody_dac + harmony_dac);
}