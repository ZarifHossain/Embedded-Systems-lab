;****************** Lab3.s ***************
; Program written by: Zarif Hossain and Ayush RoyChowdhury
; Date Created: 2/4/2017
; Last Modified: 8/29/2021
; Brief description of the program
;   The LED toggles at 2 Hz and a varying duty-cycle
; Hardware connections (External: Two buttons and one LED)
;  Change is Button input  (1 means pressed, 0 means not pressed)
;  Breathe is Button input  (1 means pressed, 0 means not pressed)
;  LED is an output (1 activates external LED)
; Overall functionality of this system is to operate like this
;   1) Make LED an output and make Change and Breathe inputs.
;   2) The system starts with the the LED toggling at 2Hz,
;      which is 2 times per second with a duty-cycle of 30%.
;      Therefore, the LED is ON for 150ms and off for 350 ms.
;   3) When the Change button is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 30% to 70% to 70%
;      to 90% to 10% to 30% so on
;   4) Implement a "breathing LED" when Breathe Switch is pressed:
; PortE device registers
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608

        IMPORT  TExaS_Init
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID1
EID1   DCB "ar68433",0  ;replace ABC123 with your EID
       EXPORT EID2
EID2   DCB "zmh345",0  ;replace ABC123 with your EID
       ALIGN 4

     EXPORT  Start

Start
; TExaS_Init sets bus clock at 80 MHz, interrupts, ADC1, TIMER3, TIMER5, and UART0
	MOV R0,#2  ;0 for TExaS oscilloscope, 1 for PORTE logic analyzer, 2 for Lab3 grader, 3 for none
	BL  TExaS_Init ;enables interrupts, prints the pin selections based on EID1 EID2
 ; Your Initialization goes here
	LDR R0, =SYSCTL_RCGCGPIO_R 
	LDRB R1, [R0]
	ORR R1, #0x10 ;set bit 4 (Port E) to turn on clock 
	STRB R1, [R0]
	
	NOP
	NOP
	
	;PE1 - Change Switch
	;PE 2 - Breath Switch
	;PE 4 - LED
	
     LDR R0, =GPIO_PORTE_DIR_R
	 LDRB R1, [R0]
	 BIC R1, #0x06 ;output is PE4
	 ORR R1, #0x10
	 STRB R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_DEN_R
	 LDRB R1, [R0]
	 ORR R1, #0x16 ;00010110
	 STRB R1, [R0]
	 
	 LDR R1, =150 ;to use later on as delay for duty cycle

;50, 150, 250, 350, 450


loop  
	LDR R4, =GPIO_PORTE_DATA_R
	LDR R5, [R4]
	AND R2, R5, #0x02 ; change switch is isolated
	LDR R5, [R4]
	AND R3, R5, #0x04 ;breath switch is isolated
	
breath_switch
    PUSH {R1, R4}
	CMP R3, #0x04 ; see if it's on
	BEQ breathe_start    ;check if breath button is pressed 
	POP {R1, R4}
	BNE change_switch

change_switch
	LDR R4, =GPIO_PORTE_DATA_R
	LDR R5, [R4]
	AND R2, R5, #0x02 ; change switch is isolated
	CMP R2, #0x02   ;see if change switch is pressed
	BEQ change_release   ; see if change switch is let go, change
	BNE duty_cycle   ; change duty cycle

change_release
	LDR R4, =GPIO_PORTE_DATA_R
	LDR R5, [R4]
	AND R2, R5, #0x02 ; change switch is isolated
	CMP R2,#0x0
	BEQ increase_percent
	BNE change_release

increase_percent
	LDR R8, =450
	SUBS R7, R8, R1
	BEQ lowest
	ADD R1, R1, #100
	BL duty_cycle

lowest
	LDR R1, =50
	BL duty_cycle

duty_cycle
	PUSH {R0, R1} 
	BL LED_ON    ;turns on LED
	BL delay
	BL LED_OFF    ; turns off LED
	LDR R2, =500
	SUBS R1, R2, R1 
	BL delay
	POP {R0, R1}
	B loop
	
LED_ON ;
	PUSH {R2, R3}
	LDR R2, =GPIO_PORTE_DATA_R
	ORR R3, R3, #0x10 ; LED ON
	STRB R3, [R2]  
	POP {R2, R3}
	
	BX LR
	
LED_OFF
	PUSH {R2, R3}
	LDR R2, =GPIO_PORTE_DATA_R
	BIC R3, R3, #0x10
	STRB R3, [R2]
	POP {R2, R3}
	BX LR
	
delay
	PUSH {R0, R1}
first
	LDR R2, =80000/4
	MUL R2, R2, R1
second
	SUBS R2, R2, #1
	BNE second
	POP {R0, R1}
	BX LR

breathe_start
	PUSH {R0, R1}
	LDR R10, =100 ; 10% 
	
	; NC = FT    C =FT/N
	; F = clock frequency
	;T = time delay 
	; number of cycles
breathe
	BL LED_ON
	BL delay_breathe
	BL LED_OFF
	LDR R2, =1000 ;sets 
	SUBS R1, R2, R1 
	BL delay_breathe
	LDR R4, =GPIO_PORTE_DATA_R
	LDR R5, [R4]
	AND R3, R5, #0x04 ;breath switch is isolated
	CMP R3, #0x04   ; check if breath switch is pressed
	BEQ breathe_increase   
	POP {R0, R1}
	BNE loop
	
breathe_increase
	LDR R9, =900
	SUBS R8, R9, R10
	CMP R8, #0x0
	BEQ breathe_descending_start
	ADD R10, R10, #200
	CMP R3, #0x04 
	BEQ breathe
	BNE loop

breathe_descending_start  ; starts from 90% and work backwards by 10%
	LDR R10, =900                                                              
breathe_descending
	BL LED_ON
	BL delay_breathe
	BL LED_OFF
	LDR R2, =1000
	SUBS R1, R2, R1 
	BL delay_breathe
	LDR R4, =GPIO_PORTE_DATA_R
	LDR R5, [R4]
	AND R3, R5, #0x04 ;breath switch is isolated
	CMP R3, #0x04
	BEQ breathe_decrease
	BNE loop

breathe_decrease
	LDR R9, =100
	SUBS R8, R9, R10
	CMP R8, #0x0
	BEQ breathe
	SUBS R10, R10, #200
	CMP R3, #0x04
	BEQ breathe_descending
	BNE loop

delay_breathe
	PUSH {R0, R1}
	
	;10% on,  90% on, 20% on, 80%  off.... 
first_breathe
	LDR R2, =800/4
	MUL R2, R2, R10
second_breathe
	SUBS R2, R2, #1
	BNE second
	POP {R0, R1}
	BX LR
	
	
	 B    loop
	 
   
  
      
     ALIGN      ; make sure the end of this section is aligned
     END        ; end of file
