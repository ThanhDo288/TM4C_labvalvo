; StringConversion.s
; Student names: Vu Duy Phuong
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; ECE319K lab 7 number to string conversions
;
; Used in ECE319K Labs 7,8,9,10. You write these two functions

 
    EXPORT   Dec2String
    EXPORT   Fix2String
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8

  
;-----------------------Dec2String-----------------------
; Convert a 32-bit number into unsigned decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit unsigned number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
i EQU 0
num EQU 4
delta EQU 8
Dec2String
	PUSH{R4,R5,R6,R7,R8,R9,LR}
	SUB SP,#8
	MOV R4,#0
	STR R4,[SP,#i] ; Initialize i on stack
	MOV R4,R0 ; R4=R0(n)
	CMP R4,#0
	BNE jmp ; -> jmp
	LDR R6,[SP,#i]
	LSL R5,R6,#0
	MOV R7,#0x30
	STR R7,[R1,R5]
	ADD R6,#1
	LSL R5,R6,#0
	MOV R7,#0x0
	STR R7,[R1,R5]
	MOV R6,#0
	STR R6,[SP,#i]
	B jmp_1
jmp
loop_1  ; count the number of digits of R0
	CMP R4,#0x0
	BEQ loop_2
	MOV R5,#10 
	LDR R6,[SP,#i]
	UDIV R4,R4,R5 
	ADD R6,#1 ; i++
	STR R6,[SP,#i]
	B loop_1
loop_2
	LDR R5,[SP,#i]
	LDR R9,[SP,#i]
	;LSL R5,R6,#1 ; 2*i
	;MOV R4,#0x00
	;STR R4,[R1,R6]
	SUB R5,#1
	MOV R6,#1
power_of_10
	CMP R5,#0
	BEQ power_of_10_jump
	MOV R8,#10
	MUL R6,R8
	SUB R5,#1
	B power_of_10
power_of_10_jump
	STR R5,[SP,#i]
	MOV R4,R0 ; R4=R0
loop_3
	;LDR R5,[SP,#i]
	;CMP R5,R9
	CMP R6,#0
	BEQ loop_4
	
	UDIV R7,R4,R6 ; R6 is power of 10
	UDIV R5,R4,R6 
	MUL R8,R6,R5 ; R8=R6*R5
	SUB R4,R4,R8 ; R4=R4-R8
	
	STR R7,[SP,#num] ; Initialize num to stack
	LDR R7,[SP,#num]
	ADD R7,#0x30
	
	MOV R5,#10
	UDIV R6,R6,R5
	
	LDR R5,[SP,#i]
	STR R7,[R1,R5] 
	ADD R5,#1 ; i++
	STR R5,[SP,#i]
	B loop_3	
loop_4	
jmp_1
	ADD SP,#8
	POP{R4,R5,R6,R7,R8,R9,LR}
    BX LR
;* * * * * * * * End of Dec2String * * * * * * * *


; -----------------------Fix2String----------------------
; Create characters for LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
;          R1 pointer to empty array
; Outputs: none
; E.g., R0=0,    then create "0.000 "
;       R0=3,    then create "0.003 "
;       R0=89,   then create "0.089 "
;       R0=123,  then create "0.123 "
;       R0=9999, then create "9.999 "
;       R0>9999, then create "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Fix2String
	PUSH{R4,R5,R6,R7,R8,LR}
	SUB SP,#12
	MOV R4,#0
	STR R4,[SP,#i]
	MOV R4,#1000
	STR R4,[SP,#delta] ; Initialize delta on stack
	MOV R4,R0 ; R4=R0
	;LDR R5,[SP,#delta] ; R5=delta=1000
	MOV R6,#9999
	CMP R4,#0xFFFFFFFF
	BEQ loop_greaterthan
	CMP R4,R6
	BLE lessthan_or_equal ; if(R4<=9999) -> jump
	; > 9999 -> "*.***"
loop_greaterthan
	LDR R4,[SP,#i]
	CMP R4,#5
	BEQ jump
	CMP R4,#1
	BNE jump_1 ; -> jump_1
	MOV R6,#0x2E ; '.'
	;LSL R5,R4,#0
	STR R6,[R1,R4]
	ADD R4,#1
	STR R4,[SP,#i] ; i++
jump_1
	;LSL R5,R4,#0
	MOV R6,#0x2A ; '*'
	STR R6,[R1,R4]
	ADD R4,#1
	STR R4,[SP,#i] ; i++
	B loop_greaterthan

lessthan_or_equal
loop_5
	LDR R5,[SP,#delta]
	;MOV R4,R0
	CMP R5,#0 
	BEQ loop_6 ; delta=0 -> loop_6
	UDIV R6,R4,R5 ; R6=R4/R5=n/delta
	STR R6,[SP,#num]
	LDR R7,[SP,#i]
	CMP R7,#1
	BNE jump_2 ; ->jump_2 if R7!=1
	MOV R6,#0x2E ; '.'
	;LSL R8,R7,#0
	STR R6,[R1,R7]
	ADD R7,#1
	STR R7,[SP,#i]
jump_2
	LDR R6,[SP,#num]
	ADD R6,#0x30
	;LSL R8,R7,#0
	STR R6,[R1,R7]
	; n%=delta
	UDIV R6,R4,R5 ; R6=R4/R5
	MUL R8,R6,R5
	SUB R4,R4,R8
	; delta/=10
	MOV R8,#10
	UDIV R5,R5,R8
	ADD R7,#1
	STR R7,[SP,#i] ; i++
	STR R5,[SP,#delta]
	B loop_5
jump
	MOV R6,#0x0
	STR R6,[R1,R4]
	B skip
loop_6	
	MOV R6,#32
	STR R6,[R1,R7]
skip
	ADD SP,#12
	POP{R4,R5,R6,R7,R8,LR}
	
    BX LR

 


     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
