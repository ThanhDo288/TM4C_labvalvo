// Lab5.c starter program EE319K Lab 5, Spromg 2023
// Runs on TM4C123
// Put your names here

// 12/31/2022


/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south facing red light connected to bit 2
// north/south facing yellow light connected to bit 1
// north/south facing green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/Debug.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "20210184"; //  ;replace abc123 with your EID
char EID2[] = "20210184"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);
// linked data structure
struct State{
	uint32_t OutB;
	uint8_t OutF;
	uint32_t Time;
	uint32_t Next[8];
	};
typedef const struct State State_t;
	
#define goS 				0
#define waitS 		  1
#define allstopS		2
#define goW 				3
#define waitW 			4
#define allstopW 		5
#define gowalk 			6
#define hurry_on1 	7
#define hurry_off1 	8
#define hurry_on2 	9
#define hurry_off2 	10
#define allstopWalk 11

const State_t FSM[12] ={
	{0x21,0x2, 200, {waitS,	waitS,	goS,	waitS,	waitS,	waitS,	waitS,	waitS}},// goS
	{0x22,0x2, 200, {allstopS,	allstopS,	allstopS,	allstopS,	allstopS,	allstopS,	allstopS,	allstopS}},//waitS
	{0x24,0x2, 100, {goS,	goW,		goW,	gowalk,	gowalk,	gowalk,	gowalk}},//allstopS
	{0x0C,0x2, 200, {waitW,	goW,	waitW,	waitW,	waitW,	waitW	,waitW,	waitW}}, //goW
	{0x14,0x2, 200, {allstopW,	allstopW,	allstopW,	allstopW,	allstopW,	allstopW,	allstopW,	allstopW }}, //waitW
	{0x24,0x2, 100, {goW,	goW,	goS,	goS,	gowalk,	gowalk,	goS,goS}},//allstopW
	{0x24,0xE, 200, {gowalk	,hurry_on1,	hurry_on1,	hurry_on1,	gowalk	,hurry_on1	,hurry_on1,	hurry_on1}},//gowalk
	{0x24,0x2, 100, {hurry_off1,	hurry_off1,	hurry_off1,	hurry_off1,	hurry_off1,	hurry_off1,	hurry_off1,	hurry_off1}},//hurry_on1
	{0x24,0x0, 100, {hurry_on2,	hurry_on2,	hurry_on2,	hurry_on2,	hurry_on2,	hurry_on2,	hurry_on2,	hurry_on2}},//hurry_off1
	{0x24,0x2, 100, {hurry_off2,	hurry_off2,	hurry_off2,	hurry_off2,	hurry_off2,	hurry_off2,	hurry_off2,	hurry_off2}},//hurry_on2
	{0x24,0x2, 100, {allstopWalk,	allstopWalk,allstopWalk,	allstopWalk	,allstopWalk	,allstopWalk,allstopWalk	,allstopWalk}},//hurry_off2
	{0x24,0x2, 100, {gowalk,	goW,goS,	goW	,gowalk	,goW,goW	,goW}},//allstopWalk

};

uint32_t input;
uint8_t S;
int main(void){ 
  DisableInterrupts();
  TExaS_Init(GRADER);
  Debug_Init(); // Index=0 means buffer is empty
// Initialize SysTick for software waits
	//PLL_Init(); 
	SysTick_Init();
// Initialize GPIO ports
	volatile int delay;
	
	SYSCTL_RCGCGPIO_R |= 0x02;   // B
	delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
  delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	
	SYSCTL_RCGCGPIO_R |= 0x01;   // A
	delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
  delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	
	
	
	SYSCTL_RCGCGPIO_R |= 0x20;   // F
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock
	 delay = SYSCTL_RCGCGPIO_R;   // no need to unlock

	GPIO_PORTA_DIR_R &= ~0x1C;   // inputs on PA4-2
  GPIO_PORTA_DEN_R |= 0x1C;    // enable digital on PA4-2

  GPIO_PORTB_DIR_R |= 0x3F;    // outputs on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;    // enable digital on PB5-0
	
	GPIO_PORTF_DIR_R |= 0x0E;    // outputs on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;    // enable digital on PF3-1
// Specify initial atate

	 S =goS;
  EnableInterrupts(); // grader, scope, logic analyzer need interrupts
  while(1){
    // set traffic lights
		GPIO_PORTB_DATA_R= FSM[S].OutB;
		GPIO_PORTF_DATA_R= FSM[S].OutF;
		SysTick_Wait10ms(FSM[S].Time);
		input= (GPIO_PORTA_DATA_R>>2)&0x03;
		S=FSM[S].Next[input];
    // set walk lights
    // call Debug_Dump with your state, input, traffic, walk information
    // wait
    // read input
    // next state depends on input and current state  
  }
}



