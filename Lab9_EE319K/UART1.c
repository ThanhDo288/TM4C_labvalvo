// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// 1/2/2023

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART1.h"
#include "../inc/tm4c123gh6pm.h"

int RxCounter=0;


// Initialize UART1
// Baud rate is 2000 bits/sec
void UART1_Init(void){
    int delay;
	Fifo_Init();
	SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
 	__asm ("nop");
 	__asm ("nop");
 	__asm ("nop");
 	__asm ("nop");
  SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
	delay=0x5000;
		while(delay){
		delay--;
	}
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 50;     // IBRD = int(80,000,000/(16*100,000)) = int(50)
  UART1_FBRD_R = 0;     // FBRD = round(0 * 64) = 0
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
	UART1_IFLS_R = UART1_IFLS_R&(~0x38)+0x10;
	UART1_IM_R |= 0x10;
  UART1_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF0FFFFF) | 0x00600000;
	NVIC_EN0_R = 0x40;
	
}

//------------UART1_InChar------------
// Wait for new input, 
// Use your FIFO, do not access UART1 hardware
// then return ASCII code
// Input: none
// Output: char read from UART
char UART1_InChar(void){
   while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0
  return((char)(UART1_DR_R&0xFF));
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until > is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
  // write this if you need it
}

//------------UART1_OutChar------------
// Output 8-bit to serial port
// Do not use interrupts or FIFO, use busy wait
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(char data){
 while((UART1_FR_R&0x0020) != 0);      // wait until TXFF is 0

  UART1_DR_R = data;
}
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// RTRIS UART1 receiver has timed out
// Put data into your FIFO
void UART1_Handler(void){char letter;
  // write this
//0) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	GPIO_PORTF_DATA_R ^= 0x8;
//1) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
		GPIO_PORTF_DATA_R ^= 0x8;

//2) as long as the RXFE bit in the UART1_FR_R is zero (should be exactly 8 bytes, but could be more or less)
	if ((UART1_FR_R&0x0010)==0){
//   -Read a byte from UART1_DR_R 
		letter=UART1_InChar();
//   -Put the byte into your software FIFO 
		Fifo_Put(letter);}
//3) Increment a RxCounter, used as debugging monitor of the number of UART messages received
		RxCounter++;
//4) acknowledge the interrupt by clearing the flag which requested the interrupt 
  UART1_ICR_R = 0x40;   // this clears bit 6 (RTRIS) in the RIS register
// 5) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	GPIO_PORTF_DATA_R^=0x08;
}





