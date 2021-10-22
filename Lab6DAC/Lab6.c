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



