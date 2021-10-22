;****************** Lab1.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Zarif Hossain
; Date Created: 1/15/2018 
; Last Modified: 8/28/2021 
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1



 ;Lab 1, EID=ZMH345
;Implement odd parity using switches on PE2,PE1,PE0 and LED on PE5


GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       PRESERVE8 
       AREA   Data, ALIGN=4
; No global variables needed

       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "zmh345",0  ;replace ABC123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 17 ; change to nonzero when ready for grading
           
      EXPORT  Lab1
Lab1 
;Initializations

;turning on the clock

	LDR R0, =SYSCTL_RCGCGPIO_R ; Loads address of the clock into R1
	MOV R1, #0x10 ;moves a 1 into port E address;
	STR R1, [R0]; moves a 1 into the contents of the clock address, enabling the clock for port E
	
	NOP 
	NOP ; waits 2 cycles for the clock to stabilize
	
	
; DIRECTION REGISTER INITIALIZATION     ;output=1 input=0

	LDR R0, =GPIO_PORTE_DIR_R ; loads address of direction register
	LDR R1, [R0] ; Loads contents of direction register in r1
	AND R1, R1, #0xF7 
	ORR R1, R1, #0x20 ;makes Pins 0,1,2 a 0 which are inputs and makes pin 5 a 1 which is an output;
	STRB R1, [R0]; store back to dir register
	
	;DEN REGISTER; 1=working 0= off
	
	LDR R0, =GPIO_PORTE_DEN_R;
	LDR R1, [R0] ; loads contents of enable register;
	
	ORR R1, R1, #0x27; turns on pins 0-2 for input and pin 5 for output
	
	STRB R1, [R0]; Stores back to enable register
	
	
	
	


loop
;input, calculate, output   

	LDR R0, =GPIO_PORTE_DATA_R; loads address of data register
	
	
	;odd parity = when 0s are even, output led is 0.
	
	;ISOLATING RELEVANT INFO (PINS FOR INPUT)
	; R3 = PIN 0
	;R4 = PIN 1 
	;R5 = PIN 2
	
	;working with Pins 0-2 and pin 5:
	
	AND R7, #0; Clears a register for use
	ADD R7, R7, #0x27; pe0-PE2 AND pe5
	
	
	
	;isolate PE0 for input r3
	
	LDR R1, [R0]; get data from data register and load into r1;
	AND R3, R1, #0x1; in LSB ; clears rest of the bits we don't want.
	
	;isolate PE1 (r4)
	
	LDR R1, [R0];
	AND R4, R1, #2; isolates the second bit which corresponds to PE1
	LSR R4, R4, #1; Shifts the PE1 bit to LSB so we can compare
	EOR R3, R3, R4; toggles PE0 and PE1
	
	;isolate PE2 (r5)
	
	LDR R1, [R0] ; Gets data back from data register
	AND R5, R1, #4 ; Isolates the third bit which corresponds to PE2 input
	LSR R5, R5, #2; have to shift over 2 spots now to get into the LSB
	EOR R3, R3, R5 ; toggles PE0 and PE2 for input 
	
	
	
	
	;shift bits to the output LED (PE5); use r6 for LED
	
	LSL R6, R3, #5 ; shifts the input to the LED output (bit 6 or PE5);
	EOR R6, R6, R7; toggle for the LED output
	
	;STORE BACK TO DATA REGISTER
	
	LDR R0, =GPIO_PORTE_DATA_R; Brings back address of data register
	
	STRB R6, [R0]; Stores data in r6 in data register location
	
	
	
	
	

	
	
	

    B    loop


    
    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
               