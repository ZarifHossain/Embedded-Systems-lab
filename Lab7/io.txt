// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on TM4C123
// Program written by: put your names here
// Date Created: March 30, 2018
// Last Modified:  change this or look silly
// Lab number: 7


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF2
	
	//use PF4 as switch and PF2 as an LED

		//turn on clock for port f
	
	SYSCTL_RCGCGPIO_R |= 0x20 ; 
	
	int delay;
	delay = SYSCTL_RCGCGPIO_R ; //waits a few cycles for clock to stabilize
	
	//set PF2 as output and PF4 as input
	
	GPIO_PORTF_DIR_R |= 0x04 ; //PF4 = input, pf2=output

	//digital enable for port F
	
	GPIO_PORTF_DEN_R |= 0x14 ; //enable for pf4 and pf2
	
	//enable pull up resistor
	
	GPIO_PORTF_PUR_R |= 0x10 ; //needed for switch not LED
	
	//regular function 
	
	GPIO_PORTF_AFSEL_R &= ~(0x14) ; //enables regular function on PF2,4
	
	
	
	
	
	
	
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF2 is heartbeat
	
	//toggle LEd
	
	GPIO_PORTF_DATA_R ^= 0x04; //toggle for LEd
	
	
	
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
 // --UUU-- wait for release; delay for 20ms; and then wait for press
	
	//wait for release = switch is not pressed, infinite loop till release
	
	while((GPIO_PORTF_DATA_R & 0x10) == 0 ) { //looks only at pf4
	}  // stays in this loop until release
	
	
	
	//delay 20 ms 
	
	int counter = 0; //delay counter
	int i;
	for(i =0; i<20000 ; i++) {
		counter++ ;
	} //increments counter to 20,000 which is 20 ms passed
	
	//wait for press
	while((GPIO_PORTF_DATA_R & 0x10) == 0x10){  //selects only PF4 
	} //keeps looping to this until press
	
	
}  

