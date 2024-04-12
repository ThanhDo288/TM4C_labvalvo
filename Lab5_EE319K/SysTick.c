// SysTick.c
// Runs on TM4C123
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait. 
// Daniel Valvano
// December 31, 2022
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"



// Initialize SysTick with busy wait running at bus clock.
#define NVIC_ST_CTRL_R      (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile uint32_t *)0xE000E018))

void SysTick_Init(void){
    // ECE319K students write this function
	NVIC_ST_CTRL_R = 0; // disable Systick during setup 
	NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
	NVIC_ST_CURRENT_R = 0; // any write to current clears it
	NVIC_ST_CTRL_R = 0x05; // enable Systick with core clock
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){
  // ECE319K students write this function
   NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag

  }

}
// 10000us equals 10ms
void SysTick_Wait10ms(uint32_t delay){
    // ECE319K students write this function
 uint32_t i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

