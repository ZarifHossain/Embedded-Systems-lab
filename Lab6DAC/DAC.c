// DAC.c
// This software configures DAC output
// Lab 6 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 10/15/21 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
  //initialize port B for DAC output
	
	SYSCTL_RCGCGPIO_R  |= 0x02; //turns on the clock for Port b
	
	int delay = SYSCTL_RCGCGPIO_R ;  // waits for clock to stabilize
		
	//DIR register set PB5-0 as outputs
	
	GPIO_PORTB_DIR_R |= 0x3F ; // sets PB5-0 as output 
	
	//digital enable for port B
	
	GPIO_PORTB_DEN_R |= 0x3F ; //enable for pb5-0
	
	GPIO_PORTB_DR8R_R |= 0x3F ; //resistor DAC
	
	
	
	
	
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
	 GPIO_PORTB_DATA_R = data; //Takes the 6 bit input from the data register. 	

}
