	; Print.s
; Student names: Zarif Hossain and Ayush 
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

	PRESERVE8
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"
LCD_OutDec

;*********binding phase********

variable1		EQU 	4
	
;initilize stack
	SUB SP, SP, #4
	MOV	R2, #10
	STR R2, [SP,#variable1]
		

;textbook page 281?

	PUSH {R4, LR}
	LDR R2, [SP, #variable1]; divisor
	MOV R4, #0 ; count = r4
	
loopback
	UDIV R3, R0, R2 ; R3 = N/10
	MUL R1, R3, R2 ; R1= N/10 *10
	SUB R1, R0, R1 ; R1 = REMAINDER
	PUSH {R1} ; Push digit for number
	ADD R4, R4, #1 ; count +1
	MOV R0, R3 ; r0 = n/20
	CMP R0, #0 ;
	BNE loopback
again	POP {R0} ;restore into r0
	ADD R0, R0, #0x30 ; ASC CODE CHART
	PUSH {LR, R4}
	BL ST7735_OutChar
	POP {LR, R4}
	SUBS R4, #1 ; COUNT - 1
	CMP R4, #0;
	BNE again
	
	POP {R4, LR}
	
	ADD SP, SP, #4;
	BX LR
		

    	

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix

;******BINDING*****
variable2 EQU 4 

	
	
	;******ALLOCATION***
	SUB SP, SP, #4 ;
	MOV R2, #1000 ; 
	STR R2, [SP, #variable2] ; stores 1000 into variable 2
	
	
	

	
	

	;check id r0 is > 9999   INPUT IS IN R0
	
	MOV R2, #9999 ;
	CMP R0, R2 ;
	BHI outputsymbols   ;if r0 is greater than 9999, go to outputting star symbols
	
	
	;check input and output in fixed-point format
	
;fixed constant is .001
;left of the decimal point (i/1000);
;decimal point (.)
;tenths digit (0x30 + (i%1000) /100 );
;hundreths digit (0x30 + (i%100) /10)
;thousands digit (0x30 +i%10) ;
	
;LEFT OF THE DECIMAL POINT 
;left of the decimal point (i/1000);


	;USE R3 FOR DIVISON 
	;R1 FOR REMAINDER 
	;R0 = INPUT 
	;R2 = MUL
	
	LDR R2, [SP, #variable2] ; r2 = 1000
	UDIV R3, R0, R2 ; R3 = R0/1000
	STR R3, [SP, #variable2]
	MUL R2, R3, R2 ; R2 = R0/1000 *1000
	SUB R1, R0, R2 ; finds remainder 
	LDR R0, [SP, #variable2]
	ADD R0, R0, #0x30 ; asc code
	PUSH {LR, R1}
	BL ST7735_OutChar ; PRINTS left decimal point number
	
	;print out decimal point
	MOV R0, #0x2E
	BL ST7735_OutChar  ;prints . to the screen
	
	POP { LR, R1} ; R1 HAS THE REMAINING DIGITS
	
	;TENTHS DIGIT
	;tenths digit (0x30 + (i%1000) /100 );

	
	MOV R2, #100 ; 
	UDIV R3, R1, R2 ; R3 = R1/100    I%100 / 100
	STR R3, [SP, #variable2]
	MUL R2, R3, R2 ; R2 = r1/100 * 100
	SUB R1, R1, R2 ; FIND REMAINDER
	LDR R0, [SP, #variable2]
	ADD R0, R0, #0x30
	
	PUSH {LR, R1}
	BL ST7735_OutChar ; PRINTS OUT TENTH DIGIT
	POP {LR ,R1} ; R1 HAS REMAINING NUMBER
	
	;HUNDRETHS DIGIT
	;hundreths digit (0x30 + (i%100) /10)
	
	MOV R2, #10 ;
	UDIV R3, R1, R2 ; R3 = R1/10
	STR R3, [SP, #variable2];
	MUL R2, R3, R2 ; R2 = r1/10 * 10
	SUB R1, R1, R2 ; FIND REMAINDER
	LDR R0, [SP, #variable2];
	ADD R0, R0, #0x30 ; ASC CODE
	
	PUSH {LR, R1}
	BL ST7735_OutChar
	POP {LR, R1} ; R1 HAS REMAINING NUMBER
	STR R1, [SP, #variable2]
	
	
	
	;THOUSANDS DIGIT
	;thousands digit (0x30 +i%10) ;
	
	LDR R0, [SP, #variable2] ; remainder value in r0
	ADD R0, R0, #0x30 ;
	PUSH {LR, R1}
	BL ST7735_OutChar
	
	
	POP {LR, R1} ; R1 HAS REMAINING NUMBER
	
	B finish
	
	
	
	
	
	
	
outputsymbols
;ASC CHART: * = 0X2A    . = 0X2E

	PUSH {LR, R1}

	MOV R0, #0x2A ;
	BL ST7735_OutChar ; outputs single character * to screen
	MOV R0, #0x2E ; . ASC CODE
	BL ST7735_OutChar ; ouputs . to screen
	MOV R0, #0x2A ;
	BL ST7735_OutChar ; outputs single character * to screen
	MOV R0, #0x2A ;
	BL ST7735_OutChar ; outputs single character * to screen
	MOV R0, #0x2A ;
	BL ST7735_OutChar ; outputs single character * to screen
	
	POP {LR, R1} 
	
	
	BX LR
; should display *.*** 

		




finish
	ADD SP, SP, #4;

     BX   LR
 
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
