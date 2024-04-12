// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17
// Last Modified: 1/2/23
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/dac.h"
#include "../inc/tm4c123gh6pm.h"
unsigned long Index=0;
unsigned long int Index_2;
unsigned short Sound_On_Flag;
const uint8_t SineWave[32] = {0,1,2,5,9,14,20,26,32,38,44,50,54,59,61,63,63,63,61,59,54,50,44,38,32,26,20,14,9,5,2,1};

// Sound_Init*******
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none

void Sound_Init(void){

DAC_Init();
NVIC_ST_CTRL_R = 0; // disable SysTick during setup

NVIC_ST_RELOAD_R = 39999; // reload value for 500us (assuming 80MHz)

NVIC_ST_CURRENT_R = 0; // any write to current clears it

NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0

NVIC_ST_CTRL_R = 0x00000007; // enable with core clock and interrupts

//EnableInterrupts();

}

// Sound_Start*******
// Start sound output, and set Systick interrupt period
// Sound continues until Sound_Start called again, or Sound_Off is called
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
// Units of period to be determined by YOU
// Maximum period to be determined by YOU
// Minimum period to be determined by YOU
// if period equals zero, disable sound output
// Output: none
void Sound_Start(uint32_t period){
// this routine sets the RELOAD and starts SysTick
//Sound_On_Flag = 1;
NVIC_ST_CTRL_R = 0; // disable SysTick during setup
NVIC_ST_RELOAD_R = period - 1; //reload value
NVIC_ST_CURRENT_R = 0;// any write to current clears it
NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0
NVIC_ST_CTRL_R = 0x00000007; // enable with core clock and interrupts

}

// Sound_Voice*******
// Change voice
// EE319K optional
// Input: voice specifies which waveform to play
// Pointer to wave table
// Output: none
void Sound_Voice(const uint8_t voice){
// optional
}
// Sound_Off******
// stop outputing to DAC
// Output: none
void Sound_Off(void){
// this routine stops the sound output
Sound_On_Flag = 0;
//GPIO_PORTB_DATA_R = 0;

}
// Sound_GetVoice*******
// Read the current voice
// EE319K optional
// Input:
// Output: voice specifies which waveform to play
// Pointer to current wavetable
const uint8_t Sound_GetVoice(void){
// if needed
return 0; // replace
}
#define PF4 (((volatile uint32_t )0x40025040))
#define PF3 (((volatile uint32_t )0x40025020))
#define PF2 (((volatile uint32_t )0x40025010))
#define PF1 (((volatile uint32_t )0x40025008))
#define PF0 (((volatile uint32_t *)0x40025004))

// Interrupt service routine
// Executed every 12.5ns*(period)
int count=0;
void SysTick_Handler(void){
//Index = (Index+1)&0x3F; //index increments from 0 to 31 and then starts back at 0 again
Sound_On_Flag=1;
if(Sound_On_Flag){
DAC_Out(SineWave[(Index++)&0x1F]);

	}
	else{
		DAC_Out(0);
	}
//GPIO_PORTB_DATA_R ^= 0x3F;
}