// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/6/21 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "../inc/tm4c123gh6pm.h"


// **************Sound_Init*********************
// Initialize digital outputs and SysTick timer
// Called once, with sound/interrupts initially off
// Input: none
// Output: none
void Sound_Init(void){

	//Set up systick 
	NVIC_ST_CTRL_R = 0; //disables systick during setup
	
	//NVIC_ST_RELOAD_R = 80000 ; //sets the reload value
	NVIC_ST_CURRENT_R = 0 ; //write to register to clear it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000 ;  //sets the priority to level 2
	NVIC_ST_CTRL_R = 0x07 ; //enables interrupt with the core clock
	
	//^^^ from EE319k textbook page 239
	DAC_Init(); //CALL TO FUNCTION
  
}

// **************Sound_Start*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Start called again, or Sound_Off is called
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
//           Units of period to be determined by YOU
//           Maximum period to be determined by YOU
//           Minimum period to be determined by YOU
//         if period equals zero, disable sound output
// Output: none
void Sound_Start(uint32_t period){

	if(period == 0) {
		NVIC_ST_RELOAD_R = 0		;  //turns the sound off  by setting reload
		
	}
	else{
	 //plays sound
		NVIC_ST_RELOAD_R = period - 1 ; //Change to reload value plays a sound
		
	}
	
}

// **************Sound_Voice*********************
// Change voice
// EE319K optional
// Input: voice specifies which waveform to play
//           Pointer to wave table
// Output: none
void Sound_Voice(const uint8_t *voice){
  
}
// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(uint32_t period){
NVIC_ST_RELOAD_R = 0;
	

}
// **************Sound_GetVoice*********************
// Read the current voice
// EE319K optional
// Input: 
// Output: voice specifies which waveform to play
//           Pointer to current wavetable
const uint8_t *Sound_GetVoice(void){
  return 0; // replace
}
#define PF4 (*((volatile uint32_t *)0x40025040))
#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
#define PF0 (*((volatile uint32_t *)0x40025004))

// Interrupt service routine
// Executed every 12.5ns*(period)


//create sine wave FOR 6 bit DAC 

 
	
int i = 0;
 const uint8_t sinWave[64] = {32,35,38,41,44,47,49,52,54,56,58,59,61,62,62,63,63,63,62,62,61,59,58,56,54,52,49,47,44,41,38,35,
32,29,26,23,20,17,15,12,10,8,6,5,3,2,2,1,1,1,2,2,3,5,6,8,10,12,15,17,20,23,26,29};	//waveform 	
	int data;

void SysTick_Handler(void){
	DAC_Out(sinWave[i]);
	
	i = (i+1)&0x3f ; //value from Periodic ISR on page 244
	
	TIMER3_ICR_R = 0x01 ; //acknowledge
	
	
	
	
	

}


