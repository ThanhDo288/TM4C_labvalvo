// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/2/23  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Song.h"
#include "../inc/wave.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "duyphuong"; //  ;replace abc123 with your EID
char EID2[] = "vuduyphuong"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;
uint8_t Key_1=0;
	uint8_t Key_2;
// lab video Lab6_voltmeter
// use this if you have a voltmeter
// 1) connect voltmeter to DACOUT 
// 2) add a breakpoint at i+1
// 3) run and record voltage for each input value
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}
// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static
// use this if you DO NOT have a voltmeter
// you need to implement LaunchPad_Init and LaunchPad_Input
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}
void change_period(uint32_t period){
			Sound_Start(period);
}
#define B  5062   // 493.9 Hz
#define A  5682   // 440.0 Hz
#define Gb  6757 // 370.0 Hz
#define D  11364 // 220Hz

const uint8_t SineWave1[32] = {0,1,2,5,9,14,20,26,32,38,44,50,54,59,61,63,63,63,61,59,54,50,44,38,32,26,20,14,9,5,2,1};
int main(void){       
  DisableInterrupts();
  TExaS_Init(SIMULATIONGRADER);    // bus clock at 80 MHz
//    SCOPE,
//    LOGICANALYZERA,
//    LOGICANALYZERB,
//    LOGICANALYZERC,
//    LOGICANALYZERE,
//    REALBOARDGRADER,
//    SIMULATIONGRADER,
//    NONE
  Key_Init();
  LaunchPad_Init(); // if needed
	Sound_Start(10000);
 Sound_Init();
	
	//Sound_Start(50000);
  Song_Init(); // extra credit 1)
  Wave_Init(); // extra credit 2)
	Testdata = 0;
  EnableInterrupts();
	unsigned long input,previous;	
  while(1){		
DelayMs(1000);		
    input=Key_In();
		if(input&&(previous!=input)){ // just pressed     
			 switch(input){
				case 0x01:
					 change_period(D);      // Play note A wave
				while(Key_In==0x01){}
					 break; 
				case 0x02:   
					 change_period(Gb);      // Play note G wave
					 break; 
				case 0x04:  
					change_period(A);      // Play note F wave
					break;
				case 0x08:
					change_period(B);      // Play note C wave
					break; 
				default:
					break;
			} 
    }
    //if((input==0) && previous){ // just released    
		//	Sound_Off();    // stop sound }
    previous = input; 
       
  }
}
