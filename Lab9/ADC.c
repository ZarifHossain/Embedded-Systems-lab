// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/2/2021  
// Student names: Zarif Hossain
// Last modification date: 11/10/21
// Labs 8 and 9 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
// from textbook page 292
	
  SYSCTL_RCGCADC_R |= 0x01;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x08;    // 2) activate clock for Port D
 int delay;
	delay = SYSCTL_RCGCGPIO_R ;
		delay = SYSCTL_RCGCGPIO_R ; //SHORT DELAY
delay = SYSCTL_RCGCGPIO_R ;
		delay = SYSCTL_RCGCGPIO_R ; //SHORT DELAY

  GPIO_PORTD_DIR_R &= ~0x04;    // 4) make pd2 input
  GPIO_PORTD_AFSEL_R |= 0x04;   // 5) enable alternate function on Pd2
  GPIO_PORTD_DEN_R &= ~0x04;    // 6) disable digital I/O on Pd2
  GPIO_PORTD_AMSEL_R |= 0x04;   // 7) enable analog functionality on Pd2
		
		

		
 //ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 5;           // 12) set channel 5
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
	
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	// 1) initiate SS3
	
	ADC0_PSSI_R = 0x0008 ; //initate SS3
	
	
	
	
  // 2) wait for conversion done
	
	while((ADC0_RIS_R & 0x08) == 0) {};
	
  // 3) read result
		
		int result;
		result = ADC0_SSFIFO3_R & 0xFFF ; 
		
		
  // 4) acknowledge completion
		
		ADC0_ISC_R = 0x0008 ;
		
  return result;
}


