// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 10/15/21  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
// Lab 6, EID1=ZMH345, EID2=AR68433
//Key3=PA5, Key2=PA4, Key1=PA3, Key0=PA2, DAC=PB5-0
//Key0=261.6, Key1=329.6, Key2=392.0, Key3=440.0 Hz



#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "zmh345"; //  ;replace abc123 with your EID
char EID2[] = "ar68433"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;

// lab video Lab6_voltmeter
int voltmetermain(void){ //voltmetermain(void){     
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  DAC_Init(); // your lab 6 solution
  Testdata = 15;
  EnableInterrupts();
  while(1){                
    Testdata = (Testdata+1)&0x3F;
    DAC_Out(Testdata);  // your lab 6 solution
  }
}
// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static
const uint32_t Inputs[16]={0, 1, 7, 8, 15, 16, 17, 18, 31, 32, 33, 47, 48, 49, 62, 63};
int staticmain(void){  
  uint32_t last,now,i;  
  TExaS_Init(NONE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}

//create different notes in an array 
//Key3=PA5, Key2=PA4, Key1=PA3, Key0=PA2, DAC=PB5-0
//Key0=261.6, Key1=329.6, Key2=392.0, Key3=440.0 Hz

const uint32_t Freqnotes[5] = {0, 262, 330, 392, 440, } ;  //convert to period values?
// 0 for no sound
//C = 262 Hz   PA2 = KEY 0
//E = 330 hZ  PA3 = KEY 1
//G = 392 Hz  PA4 = KEY 2
//A = 440 Hz	PA5 = KEY 3

uint32_t PeriodNotes[5] = {0, 4770, 3787,3188, 2840};  //1250000/freq

     
int main(void){       
  TExaS_Init(SIMULATIONGRADER);    // bus clock at 80 MHz
  Key_Init();
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
	
	//calls functions
	uint32_t data;
	
	DAC_Out(data);
	DAC_Init(); 
	

	
	//HeartBeat using PortF
	
	SYSCTL_RCGCGPIO_R |= 0x20 ; //Turns on the clock for Port F
	
	volatile uint32_t delay;
	delay = SYSCTL_RCGCGPIO_R ; //waits for clock to stabilize
	
	//set Port F as output 
	GPIO_PORTF_DIR_R |= 0x08 ; //use PF3 as heartbed LED
	
	//digital enable for Port F;
	GPIO_PORTF_DEN_R |= 0x08 ; //digital enable for PF3
	
	//turn on LED for heartbeat 
	
	
	   EnableInterrupts();
		 
		 uint32_t pianoinput;
  while(1){    

GPIO_PORTF_DATA_R ^= 0x08 ; //write Inputs  to PF3 		
    
		//get inputs from keyboard
		 pianoinput = Key_In() ; // geta the value from the piano keyboard input
		
		//	0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
		
		if(pianoinput == 0) {
			
			Sound_Start(PeriodNotes[0]) ;
		} //play no sound
		
		else if(pianoinput == 0x04) {  //key 0= PA2
			
			Sound_Start(PeriodNotes[1]); 
			
		}  // Plays Note C
		
		else if (pianoinput == 0x08) {  //key 1 = PA3
		
			Sound_Start(PeriodNotes[2]);
			
		}
		//Note E
		
		else if(pianoinput == 0x10) {  //key 2 = PA4
			
			Sound_Start(PeriodNotes[3]);
			
			
		} //Note G
		
		else if(pianoinput == 0x20) {  //Key 3 = PA5
			
			Sound_Start(PeriodNotes[4]);
			
		} //Plays Note A
		
		
	//	0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
		
	
		} //Plays Note A
		
			
		//Output sound to Piano
		
	 // Depending on what key was pressed on piano, sound is played from the different frequencies in the array.
		
		
		//            
	}

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

// Key.c
// This software configures the off-board piano keys
// Lab 6 requires a minimum of 4 keys, but you could have more
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 10/15/21  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: none
void Key_Init(void){ volatile uint32_t delay;

	//initilize port A for key inputs
	
	SYSCTL_RCGCGPIO_R |= 0x01 ; //turns on the clock for Port A
	
	delay = SYSCTL_RCGCGPIO_R  ; //waits for clock to stablilize
	
	//DIR register for ports 5 to 2. Port A input so set to 0
	
	GPIO_PORTA_DIR_R &= ~(0x3C); //sets PA5-2 as input 
	
	//Digital ENable 
	GPIO_PORTA_DEN_R = 0x3C ; //PA5-2 enable
	
	//set port for regular function 
	
	GPIO_PORTA_AFSEL_R &= ~0x3C ; //regular function on PA2-5;
	
	
	
 }
// **************Key_In*********************
// Input from piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
	
	//write to data register
	
	int pianokey;
	pianokey = (GPIO_PORTA_DATA_R & 0x3C) ; //selects only PA5-2 for reading. 
	
  return pianokey; // replace this line

  
}

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
// implement if needed
}


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
// implement if needed
  return 0; // replace   
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of PORTF
// implement if needed

}

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





