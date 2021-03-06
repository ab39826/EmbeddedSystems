// Timer0A.h
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

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer0A_Init(unsigned short period);
void Timer0A_Disable_Interrupts(void);
void Timer0A_Enable_Interrupts(unsigned short period);

void Timer1A_Init(unsigned short period);
void Timer1A_Disable_Interrupts(void);
void Timer1A_Enable_Interrupts(unsigned short period);

void Timer2A_Init(unsigned short period);
void Timer2A_Disable_Interrupts(void);
void Timer2A_Enable_Interrupts(unsigned short period);

void Timer3A_Init(unsigned short period);
void Timer3A_Disable_Interrupts(void);
void Timer3A_Enable_Interrupts(unsigned short period);



