;****************** main.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Place your name here
; Date Created: 1/15/2018 
; Last Modified: 12/30/2022 
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1
GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
	 PRESERVE8 
       AREA   Data, ALIGN=2
; Declare global variables here if needed
	
; with the SPACE assembly directive
       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "ABC123",1  ;replace ABC123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 1 ; change to nonzero when ready for grading
           
      EXPORT  Lab1

Lab1	
 ;initialization
  LDR R0,=SYSCTL_RCGCGPIO_R
	MOV R1,#0x08
	STR R1,[R0]
	LDR R2,=GPIO_PORTD_DIR_R
	MOV R3,#0x10
	STR R3,[R2]
	LDR R4,=GPIO_PORTD_DEN_R
	MOV R5,#0x17
	STR R5,[R4]
loop
	LDR R0,=GPIO_PORTD_DATA_R
	LDR R1,[R0]
	AND R2,R1,#0x01
	AND R3,R1,#0x02
	LSR R3,R3,#0x01
	AND R4,R1,#0x04
	LSR R4,R4,#0x02
	EOR R5,R2,R3
	EOR R6,R4,R5
	EOR R6,R6,0x01
	LSL R6,R6,#0x04
	BIC R1,R1,#0x10
	ORR R1,R1,R6
	STR R1,[R0]
   ;main program loop
    B    loop
	
    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
               