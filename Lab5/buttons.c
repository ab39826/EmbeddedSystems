#include "buttons.h"


#define NVIC_EN0_INT31           0x80000000  // Interrupt 31 enable
#define GPIO_PORTG_DIR_R (*((volatile unsigned long *) 0x40026400))
#define GPIO_PORTG_CR_R (*((volatile unsigned long *) 0x40026524))
#define GPIO_PORTG_DATA_R (*((volatile unsigned long *) 0x40026000))
#define PG3 (*((volatile unsigned long *) 0x40026020))
#define PG4 (*((volatile unsigned long *) 0x40026040))
#define PG5 (*((volatile unsigned long *) 0x40026080))
#define PG6 (*((volatile unsigned long *) 0x40026100))
#define PG7 (*((volatile unsigned long *) 0x40026200))
#define GPIO_PORTG_IBE_R (*((volatile unsigned long *) 0x40026408))
#define GPIO_PORTG_IEV_R (*((volatile unsigned long *) 0x4002640C))
#define GPIO_PORTG_IM_R (*((volatile unsigned long *) 0x40026410))
#define GPIO_PORTG_RIS_R (*((volatile unsigned long *) 0x40026414))
#define GPIO_PORTG_IS_R (*((volatile unsigned long *) 0x40026404))
#define GPIO_PORTG_DEN_R (*((volatile unsigned long *) 0x4002651C))
#define GPIO_PORTG_PUR_R (*((volatile unsigned long *) 0x40026510))
#define GPIO_PORTG_ICR_R (*((volatile unsigned long *) 0x4002641C))
#define NVIC_PRI7_R      (*((volatile unsigned long *)0xE000E41C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOG      0x00000040  // port G Clock Gating Control
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register //GPIO Port G is interrupt # 47

void (*selectFunction) (void);
void (*leftFunction) (void);
void (*rightFunction) (void);

void doNothing(void) {
	return;
}

void buttonInit(void){

int x = 0;
	 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	while(x<100){
		x++;
	}
  GPIO_PORTG_PUR_R |= 0xF8;
	GPIO_PORTG_DIR_R &= ~0xF8;  // make Port G3-7 in (PC4 built-in button)
  GPIO_PORTG_DEN_R |= 0xF8;   // enable digital I/O on Port G
  GPIO_PORTG_IS_R &= ~0xF8;   // Port G is edge-sensitive (default setting)
  GPIO_PORTG_IBE_R &= ~0xF8;  // Port G is not both edges (default setting)
  GPIO_PORTG_IEV_R &= ~0xF8;  // Port G falling edge event (default setting)
  GPIO_PORTG_ICR_R = 0xF8;    // clear flag4
                             // GPIO PortG=priority 1
	
	

  NVIC_PRI7_R = (NVIC_PRI7_R&0x00FFFFFF)|0x20000000; // bits 29-31
	 NVIC_EN0_R |= NVIC_EN0_INT31;
  selectFunction = 0;
  leftFunction = 0;
  rightFunction = 0;
}

void buttonEnableInterrupts(void){
 GPIO_PORTG_IM_R |= 0xF8;    // enable interrupt on PG3-7
}

void buttonDisableInterrupts(void){
 GPIO_PORTG_IM_R &= ~0xF8;    // enable interrupt on PF0,2,4,6
}

void setSelectFunction(void (*newFunction) (void)) {
	selectFunction = newFunction;
}

void setLeftFunction(void (*newFunction) (void)) {
	leftFunction = newFunction;
}

void setRightFunction(void (*newFunction) (void)) {
	rightFunction = newFunction;
}

void GPIOPortG_Handler(void){	
/*
if(GPIO_PORTG_RIS_R&0x08){
	if(!PG3) {
		upPressed = 1;
	}
	GPIO_PORTG_ICR_R |= 0x08;
}

if(GPIO_PORTG_RIS_R&0x10){
	if(!PG4) {
		downPressed = 1;
	}
	GPIO_PORTG_ICR_R |= 0x10;
}
*/
if(GPIO_PORTG_RIS_R&0x20){
	if(!PG5) {
		leftFunction();
	}
	GPIO_PORTG_ICR_R |= 0x20;
}

if(GPIO_PORTG_RIS_R&0x40){
	if(!PG6) {
		rightFunction();
	}
	GPIO_PORTG_ICR_R |= 0x40;
}

if(GPIO_PORTG_RIS_R&0x80){
	if(!PG7) {
		selectFunction();
	}
	GPIO_PORTG_ICR_R |= 0x80;
}

}
