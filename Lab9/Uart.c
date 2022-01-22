// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  
// Daniel Valvano
// November 11, 2021

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART.h"
#include "../inc/tm4c123gh6pm.h"
#define LF  0x0A

// Initialize UART1 on PC4 PC5
// Baud rate is 1000 bits/sec
// Receive interrupts and FIFOs are used on PC4
// Transmit busy-wait is used on PC5.
void UART_Init(void){
  // write this
	//Call Fifo
	
	Fifo_Init();
	
	  SYSCTL_RCGCUART_R |= 0x02;   // 1) activate UART 1
	
	UART1_CTL_R  &= ~0x01 ; //disables Uart
	
	//set IBRD and FBRD
	
//assuming baud rate of 1000 bits/sec
//ibrd = 80 Mhz / 16*1000  = 5000
	//fbrd = remaining fraction * 64   = 0 no fraction from IBRD
	
	UART1_IBRD_R = 5000;
	UART1_FBRD_R = 0x00 ; //set to 0 bc fraction part is 0
	
	UART1_LCRH_R = 0x0070 ; //enables FIFO, 8 bits and no parity
	
	UART1_CTL_R |= 0x0301 ; //sets rxe and txe along with enable of Uart1 
	
	  SYSCTL_RCGCGPIO_R |= 0x04;    // 2) activate clock for Port C
		int delay;
		delay = SYSCTL_RCGCGPIO_R ;
		delay = SYSCTL_RCGCGPIO_R ; //SHORT DELAY
	
	GPIO_PORTC_AFSEL_R |= 0x30 ; //PC4 and PC5
	GPIO_PORTC_DEN_R |= 0x30 ; //digital enable for PC4,5
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) +0x00220000 ; //
	GPIO_PORTC_AMSEL_R = ~0x30 ; //disables bits for pc4/pc5
	
	UART1_IM_R = 0x10 ; //ARM RX RIS
	UART1_IFLS_R = (UART1_IFLS_R & ~0x28) + 0x10 ; 
	
	NVIC_PRI1_R = (NVIC_PRI1_R & 0x00FFFFFF) | 0x40000000  ; //sets priority to 2
	
	NVIC_EN0_R |= 0x40 ;
	
	////////
	
	
}

//------------UART_InChar------------
// Receive new input, interrupt driven
// Input: none
// Output: return read ASCII code from UART, 
// Reads from software FIFO (not hardware)
// Either return 0 if no data or wait for data (your choice)
unsigned char UART1_InChar(void){
  // write this
	
	//page 307
	
	while((UART1_FR_R &0x0010) != 0){

	}; //waits until rxfe is 0 
	
	return ((char) (UART1_DR_R % 0xFF)) ;
	
	
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until LR is received
//    or until max length of the string is reached.
// Reads from software FIFO (not hardware)
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
// optional implement this here or in Lab 9 main
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
// busy-wait; if TXFF is 1, then wait
// Transmit busy-wait is used on PC5.
void UART_OutChar(char data){
  // write this
	
	//page 307
	
	while((UART1_FR_R &0x0020) != 0) {
	} ; //wait until txff is 0
	
	UART1_DR_R = data ; 

}
#define PF2       (*((volatile uint32_t *)0x40025010))
// hardware RX FIFO goes from 7 to 8 or more items
// Receive interrupts and FIFOs are used on PC4
void UART1_Handler(void){
  PF2 ^= 0x04;  // Heartbeat
// write this
	
		//toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
  PF2 ^= 0x04;  // Heartbeat
	
	

//as long as the RXFE bit in the UART1_FR_R is zero , Read bytes from UART1_DR_R 
	while((UART1_FR_R & 0x0010) == 0) {
		
		//read bytes from Uart1_DR_R		
		//check if error
		if(Fifo_Put(UART1_DR_R &0xFF) <= 0) {
			
//need global error count for if fifo is full. If empty this isn't an error so 0 is fine.
			int error = 0;
			error++ ;
		}
		
		
	}  //repeatedly reads from Uart1_dr_r to see if there is anything in FIFO
	
	
//Put all bytes into your software FIFO (should be exactly 8 bytes, but could be more)
		

//Increment a RxCounter, used as debugging monitor of the number of UART messages received
		int counter ;
		
		counter = counter +1 ; //increments the counter to keep track of how many messages received each TIMER0 this is called
		
		
//acknowledge the interrupt by clearing the flag which requested the interrupt 
		
		UART1_ICR_R = 0x10;   // this clears bit 4 (RXRIS) in the RIS register   

		
//toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	PF2 ^= 0x04 ; //heartbeat for PF2
		
//return from interrupt

	//full FIFO
	
	
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  // if needed
  
}



