#include "buttons.h"


#define NVIC_EN0_INT1 	0x00000002 //interrupt 1 enable
#define GPIO_PORTB_DIR_R (*((volatile unsigned long *) 0x40005400))
#define GPIO_PORTB_CR_R (*((volatile unsigned long *) 0x40005524))
#define GPIO_PORTB_DATA_R (*((volatile unsigned long *) 0x40005000))

#define PB0to2 (*((volatile unsigned long *) 0x4000501C))
#define PB0 (*((volatile unsigned long *) 0x40005004))
#define PB1 (*((volatile unsigned long *) 0x40005008))
#define PB2 (*((volatile unsigned long *) 0x40005010))
#define PB3 (*((volatile unsigned long *) 0x40005020))
#define PB4 (*((volatile unsigned long *) 0x40005040))
#define PB5 (*((volatile unsigned long *) 0x40005080))
#define PB6 (*((volatile unsigned long *) 0x40005100))

#define GPIO_PORTB_IBE_R (*((volatile unsigned long *) 0x40005408))
#define GPIO_PORTB_IEV_R (*((volatile unsigned long *) 0x4000540C))
#define GPIO_PORTB_IM_R (*((volatile unsigned long *) 0x40005410))
#define GPIO_PORTB_RIS_R (*((volatile unsigned long *) 0x40005414))
#define GPIO_PORTB_IS_R (*((volatile unsigned long *) 0x40005404))
#define GPIO_PORTB_DEN_R (*((volatile unsigned long *) 0x4000551C))
#define GPIO_PORTB_PUR_R (*((volatile unsigned long *) 0x40005510))
#define GPIO_PORTB_ICR_R (*((volatile unsigned long *) 0x4000541C))
#define NVIC_PRI0_R      (*((volatile unsigned long *)0xE000E400))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register //GPIO Port G is interrupt # 47


#define NVIC_EN0_INT3 	0x00000008 //interrupt 3 enable
#define GPIO_PORTD_DIR_R (*((volatile unsigned long *) 0x40007400))
#define GPIO_PORTD_CR_R (*((volatile unsigned long *) 0x40007524))
#define GPIO_PORTD_DATA_R (*((volatile unsigned long *) 0x40007000))

#define PD0 (*((volatile unsigned long *) 0x40007004))
#define PD1 (*((volatile unsigned long *) 0x40007008))
#define PD2 (*((volatile unsigned long *) 0x40007010))
#define PD3 (*((volatile unsigned long *) 0x40007020))
#define PD4 (*((volatile unsigned long *) 0x40007040))
#define PD5 (*((volatile unsigned long *) 0x40007080))
#define PD6 (*((volatile unsigned long *) 0x40007100))
#define PD7 (*((volatile unsigned long *) 0x40007200))


#define GPIO_PORTD_IBE_R (*((volatile unsigned long *) 0x40007408))
#define GPIO_PORTD_IEV_R (*((volatile unsigned long *) 0x4000740C))
#define GPIO_PORTD_IM_R (*((volatile unsigned long *) 0x40007410))
#define GPIO_PORTD_RIS_R (*((volatile unsigned long *) 0x40007414))
#define GPIO_PORTD_IS_R (*((volatile unsigned long *) 0x40007404))
#define GPIO_PORTD_DEN_R (*((volatile unsigned long *) 0x4000751C))
#define GPIO_PORTD_PUR_R (*((volatile unsigned long *) 0x40007510))
#define GPIO_PORTD_ICR_R (*((volatile unsigned long *) 0x4000741C))
#define NVIC_PRI0_R      (*((volatile unsigned long *)0xE000E400))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))


void (*changeTrackFunction) (void);
void (*recordFunction) (void);
void (*playBackFunction) (void);
void (*changeInstrumentFunction) (void);

void doNothing(void) {
	return;
}

void buttonInit(void){

int x = 0;
	 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB + SYSCTL_RCGC2_GPIOD;
	while(x<100){
		x++;
	}
  //maybe pull up resistors probably not (but use PUR if you need to)
	
	GPIO_PORTB_DIR_R &= 0x80;  
	GPIO_PORTB_DIR_R |= 0x07; 	// make Port B 0-2 output, the rest input except pin 7 which we ignore and just leave alone
  GPIO_PORTB_DEN_R |= 0x7F;   // enable digital I/O on  ALL pins of port B
  GPIO_PORTB_IS_R &= ~0x7F;   // Port B is edge-sensitive (default setting)
  GPIO_PORTB_IBE_R &= ~0x7F;  // Port B is not both edges (default setting)
  GPIO_PORTB_IEV_R &= ~0x7F;  // Port B falling edge event (default setting)
  GPIO_PORTB_ICR_R |= 0x7F;    
	 GPIO_PORTB_IM_R |= 0x7F;
	
	 NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00002000; // bits 15-13
	 NVIC_EN0_R |= NVIC_EN0_INT1;
	
  changeTrackFunction = 0;
  recordFunction = 0;
  playBackFunction = 0;
	changeInstrumentFunction = 0;
	
	//note key initialization
	//clock gating already taken care of above.
	
	GPIO_PORTD_DIR_R &= 0x00; 	// make Port B 0-2 output, the rest input except pin 7 which we ignore and just leave alone
  GPIO_PORTD_DEN_R |= 0xFF;   // enable digital I/O on  ALL pins of port D
  GPIO_PORTD_IS_R &= 0x00;   // Port D all edge sensitive for keys
  GPIO_PORTD_IBE_R |= 0xFF;  
  GPIO_PORTD_ICR_R |= 0xFF;    
	 GPIO_PORTD_IM_R |= 0xFF;
	
		 NVIC_PRI0_R = (NVIC_PRI0_R&0x1FFFFFFF)|0x40000000; // bits 31-29
	 NVIC_EN0_R |= NVIC_EN0_INT3;
	 

	
}


void setChangeTrackFunction(void (*newFunction) (void)) {
	changeTrackFunction = newFunction;
}

void setRecordFunction(void (*newFunction) (void)) {
	recordFunction = newFunction;
}

void setPlayBackFunction(void (*newFunction) (void)) {
	playBackFunction = newFunction;
}

void setChangeInstrumentFunction(void (*newFunction) (void)) {
changeInstrumentFunction = newFunction;
	
}



void toggleLED (int index){
	
	//if index = 0 toggle LED1 ...index = 2 toggle LED3
	PB0to2 = 0;
	
	switch(index)
	{
		case 0 :
			PB0 = 1;
			break;
		case 1:
			PB1 = 2;
			break;
		case 2:
			PB2 = 4;
			break;
	}
}

void GPIOPortB_Handler(void){	


if(GPIO_PORTB_RIS_R&0x08){
	if(!PB3) {
		changeInstrumentFunction();
	}
	GPIO_PORTB_ICR_R |= 0x08; //
}


if(GPIO_PORTB_RIS_R&0x10){
	if(!PB4) {
		changeTrackFunction();
	}
	GPIO_PORTB_ICR_R |= 0x10;
}

if(GPIO_PORTB_RIS_R&0x20){
	if(!PB5) {
		recordFunction();
	}
	GPIO_PORTB_ICR_R |= 0x20;
}

if(GPIO_PORTB_RIS_R&0x40){
	if(!PB6) {
		playBackFunction();
	}
	GPIO_PORTB_ICR_R |= 0x40;
}

}

