// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: Zarif Hossain
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 11/10/21

// Specifications:
// Measure distance using slide pot, sample at 10 Hz
// maximum distance can be any value from 1.5 to 2cm
// minimum distance is 0 cm
// Calculate distance in fixed point, 0.001cm
// Analog Input connected to PD2=ADC5
// displays distance on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats (use them in creative ways)
// 

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on ST7735
// main3 adds your convert function, position data is no ST7735

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DEN_R |= 0x0E;

}
uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
int main1(void){       // single step this program and look at Data
  TExaS_Init(SCOPE);       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 5
  }
}
uint32_t startTime,stopTime;
uint32_t ADCtime,OutDectime; // in usec

int main2(void){
  TExaS_Init(SCOPE);  // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  ST7735_InitR(INITR_REDTAB); 
	NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
  NVIC_ST_CURRENT_R = 0;    // any write to current clears it
  NVIC_ST_CTRL_R = 5;

  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    startTime= NVIC_ST_CURRENT_R;
    Data = ADC_In();  // sample 12-bit channel 5
    stopTime = NVIC_ST_CURRENT_R;
    ADCtime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec

    ST7735_SetCursor(0,0);
    startTime= NVIC_ST_CURRENT_R;
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // spaces cover up characters from last output
    stopTime = NVIC_ST_CURRENT_R;
    OutDectime = ((startTime-stopTime)&0x0FFFFFF)/80; // usec
  }
}

// your function to convert ADC sample to distance (0.001cm)
uint32_t Convert(uint32_t input){
  // write this
	
	//from calibration:
	
	//y = 
	// d = (1500*data)/4096+0 
	
	Position = ((1500*input)/4096+0) ; //converts data to fixed point distance
	
	return Position ;

}
int main3(void){ uint32_t time=0;
  TExaS_Init(SCOPE);         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 5
  ST7735_PlotClear(0,2000); 
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 5
    PF3 = 0x08;       // Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
    ST7735_PlotPoint(Position);
    if((time%8)==0){
      ST7735_PlotNextErase();
    }
    time++;    
  }
}   
void SysTick_Init(uint32_t period){
  // write this
	
	// page 239
	
	NVIC_ST_CTRL_R = 0 ; //disables systick during setup
	
	NVIC_ST_RELOAD_R = period -1 ; //sets the reload value
	NVIC_ST_CURRENT_R = 0 ; //write to register to clear it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000 ;  //sets the priority to level 2
	NVIC_ST_CTRL_R = 0x07 ; //enables interrupt with the core clock
	
	
	
	
	
	
	
}
int ADCmail;
int ADCstatus ;

void SysTick_Handler(void){
  // write this
	// toggle heartbeat LED (change from 0 to 1, or from 1 to 0)
 //sample the ADC
// save the 12-bit ADC sample into the mailbox ADCMail
 //set the mailbox flag ADCStatus to signify new data is available
// return from interrupt

	PF1 ^= 0x02 ; //toggles heartbeat for pf1
	
	//get data from ADC
	
	int ADCdata;
	ADCdata = ADC_In(); //gets input data from adc
	
	//save 12 bit sample into mailbox
	
	ADCmail = ADCdata ;
	
	
	//ADC status changed to 1
	
	ADCstatus = 1 ; 
	
	

	
}


int ADCmail ; 
int ADCstatus ; //global variables

int main(void){ // this is real lab 8 main
  // write this
	// 10 Hz sampling in SysTick ISR
	
	//call inits
	
	TExaS_Init(SCOPE) ;
	
	ADC_Init(); //initilizes ADC DONT call ADC_in
	ST7735_InitR(INITR_REDTAB);  //from above, inits display

	
	PortF_Init() ; //initlizes port F
	
	//wait for flag to be set
	SysTick_Init(500000);
	
	
	
	while(1){
		
		
		 //wait for the mailbox flag ADCStatus to be true
 //read the 12-bit ADC sample from the mailbox ADCMail
 ///clear the mailbox flag ADCStatus to signify the mailbox is now empty
 //convert the sample into a fixed-point number (variable integer is 0 to 200)
 //output the fixed-point number on the LCD with units 

		// check semaphore
		// output to LCD
		
		while(ADCstatus == 0) {
	} ; //keep looping until flag becomes 1 or true
	
	//get data from mail
	
	int data ;
	data = ADCmail ;
	
	//clear flag
	ADCstatus = 0 ; //sets flag to 0
	
	//convert 
	Position = Convert(data) ; //converts input data into fixed point
	
	//set cursor
	
	ST7735_SetCursor(0,1) ; 
	
	//OUTPUT TO SCREEN
	LCD_OutFix(Position) ; //outputs converted data to the screen 
	
	
	//units 
	
	ST7735_OutString("cm" );
	}
}

