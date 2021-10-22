// Lab5.c starter program EE319K Lab 5, Fall 2021
// Runs on TM4C123
// Put your names here
// Last Modified: Oct 4, 2021

/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "zmh345"; //  ;replace abc123 with your EID
char EID2[] = "ar68433"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);


// Lab 5, EID1=ZMH345, EID2=AR68433
//Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
//When all inputs true, ... South, West, Walk, South, West, Walk, South, West, Walk, ...



//State Names for later use



//East Green, North red = PB5-0 = 001100 = 0x0C
//East Yellow, North red = PB5-0 = 0x14; 



struct StateSetup{
	uint8_t TrafficLight;
	uint8_t Pedestrian;
	uint8_t delay;
	uint8_t Next[8];
};
typedef struct StateSetup State;

#define Start 0
#define NorthG 1
#define NorthY 2
#define NorthR 3
#define EastG 4
#define EastY 5 
#define EastR 6
#define walk 7 //  walk button pressed: can walk in either direction both lights red 0x08 no cars on either sensor
#define on1 8
#define off1 9
#define on2 10
#define off2 11
#define on3 12
#define off3 13
#define on4 14



//North Green, east red = PB5-0 = 100001 = 0x21 
//North yellow, east red = PB5-0 = 100010 = 0x22


 

//#define RedEN 10
//#define DontWE 11

//Finite State Machine

State FSM[15] ={

	//PED LED: 0x02 = PF1 red, don't walk
	//PED LED: 0xE = PF1-PF3. walk 
	//flash ped
	
	//0x21 = North green, east red    0x22 = north yellow, east red, 0x24 = north red, east red    0x0C = east green, north red  0x14 = east yellow, north red
	
	
	//start
	{0x24, 0x02, 100, {Start, EastG, NorthG, NorthG, walk, walk, NorthG, NorthG,}},
	
	//North Green
	{0x21, 0x02, 100, {NorthG, NorthY, NorthG,  NorthY, NorthY, NorthY, NorthY, NorthY, }},
	
	//North Y
	{0x22, 0x02, 100, {NorthR, NorthR, NorthR, NorthR,NorthR, NorthR,NorthR, NorthR,}},
	
	//North R
	{0x24, 0x02, 100, {Start, Start, EastG, EastG, walk, EastG, walk, EastG, }},  
	
	//East G
	{0x0C, 0x02, 100, {EastG, EastG, EastY, EastY,EastY,EastY,EastY,EastY,}},
	
	//East Y 
	{0x14, 0x02, 100, {EastR, EastR,EastR,EastR,EastR,EastR,EastR,EastR,}},
	
	// East R
	{0x24, 0x02, 100, {Start, NorthG, NorthG, NorthG, walk, walk, NorthG, walk}},
	
	//walk
	{0x24, 0x0E, 100,{walk,on1,on1, on1, walk, on1, on1, off1,}},
	
	//on1
	{0x24, 0x02, 50, {off1, off1,off1,off1,off1,off1,off1,off1,}},
	
	//off1
	{0x24, 0x00, 50, {on2, on2, on2, on2, on2, on2, on2, on2, }},
	
	//on2
	{0x24, 0x02, 50, {off2, off2,off2,off2,off2,off2,off2,off2,}},
	
	//off2
	{0x24, 0x00, 50, {on3, on3,on3,on3,on3,on3,on3,on3,}},
	
	//on 3
	{0x24, 0x02, 50, {off3, off3,off3,off3,off3,off3,off3,off3,}},
	
	//off 3 
	{0x24, 0x00, 50, {on4, on4,on4,on4,on4,on4,on4,on4,}},
	
	//on 4
	{0x24, 0x02, 50, {Start, Start,Start,Start,Start,Start,Start,Start,}},
	


};

	
	

	//North Green, east red = PB5-0 = 100001 = 0x21 
	
	//North Green = 0x21
	
	int output;
	int Input;
int pedestrianlight;

int main(void){ 
  DisableInterrupts();
  TExaS_Init(5);
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize system
	
	//initilize clock
	
	SYSCTL_RCGCGPIO_R |= 0x32; //turn on the clock for Ports B, E, and F.
	
	int delay = SYSCTL_RCGCGPIO_R;  //wait a few cycles for clock to stablilize
	
	//initlize Ports to use 
	GPIO_PORTE_DIR_R |= 0x0; //switches are inputs 
	GPIO_PORTE_DEN_R |= 0x0E;  //digital enable for PORT E PE3-1
	
	GPIO_PORTB_DIR_R |= 0x3F; //pb5-pb0 are outputs	
	GPIO_PORTB_DEN_R |= 0x3F; //digital enable for Port B pb5-0
	
	GPIO_PORTF_DIR_R |= 0x0E; //Walk LED are outputs 
	GPIO_PORTF_DEN_R |= 0x0E; //digital enable for Port F
	
	
	
	
	
	
	uint8_t CurrentState = NorthG; //initial state     0,1,2,3,4,5,6,7,8,9
	
  EnableInterrupts(); 
  while(1){
    // 1) output
	output = FSM[CurrentState].TrafficLight ; //gets data from PORT B data register for traffic light signal
		GPIO_PORTB_DATA_R = output;  //traffic light signal,
		
		
		
		pedestrianlight = FSM[CurrentState].Pedestrian;
		
		
		GPIO_PORTF_DATA_R = pedestrianlight ; //gets data from pedestrian LED
		
    // 2) wait
		SysTick_Wait10ms(FSM[CurrentState].delay); //delay for finite state machine 
		
    // 3) input
		
		 Input = ((GPIO_PORTE_DATA_R & 0x0E)>>1) ; //PE1-PE3 
		
		
		
		
		 
		
	 
		
		//Input = GPIO_PORTE_DATA_R ; //new values for car sensors 
		
		
    // 4) next update the state
		
		
		CurrentState = FSM[CurrentState].Next[Input] ;
		

  }
}







