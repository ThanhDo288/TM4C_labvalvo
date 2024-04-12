// EngineControl.c
// Runs on LM4F120/TM4C123
// Use a pointer implementation of a Mealy finite state machine to operate
// an engine with a control input, gas and brake outputs, and two states.
// Daniel Valvano
// December 30, 2022

//  This example accompanies the book
//  "Embedded Systems: Introduction to Arm Cortex M Microcontrollers"
//  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
//  Program 6.9
//
//Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
//   You may use, edit, run or distribute this file
//   as long as the above copyright notice remains
//THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
//OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
//MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
//VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
//OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//For more information about my classes, my research, and my books, see
//http://users.ece.utexas.edu/~valvano/

// control input connected to PB0 (1=go)
// gas output connected to PB1
// brake output connected to PB2

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"

// Linked data structure
struct State {
  uint32_t Out[2];
  uint32_t Delay;
  const struct State *Next[2];};
typedef const struct State State_t;
#define Stop  &FSM[0]
#define Go    &FSM[1]
State_t FSM[2]={
 {{2,0},10,{Stop,Go}},
 {{0,1},10,{Stop,Go}}};

int main(void){ 
  State_t *Pt;  // state pointer
  uint32_t Input;
  PLL_Init(Bus80MHz);          // configure for 80 MHz clock
  SysTick_Init();              // initialize SysTick timer
  // activate port B
  SYSCTL_RCGCGPIO_R |= 0x02;
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
  GPIO_PORTB_DIR_R &= ~0x01;   // make PB0 in
  GPIO_PORTB_DIR_R |= 0x06;    // make PB2-1 out
  GPIO_PORTB_DEN_R |= 0x07;    // enable digital I/O on PB2-0
  Pt = Stop;                   // initial state: stopped
  while(1){
    Input = GPIO_PORTB_DATA_R&0x01; // get new input from Control
    GPIO_PORTB_DATA_R =
    (GPIO_PORTB_DATA_R& (~0x06))|((Pt->Out[Input])<<1);// output to Brake and Gas
    SysTick_Wait10ms(Pt->Delay);// wait 10 ms * current state's Delay value
    Pt = Pt->Next[Input];      // transition to next state
  }
}
