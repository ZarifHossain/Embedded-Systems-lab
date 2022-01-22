// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 11/15/2021 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "DAC.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
   // execute user task
}

int main1(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};







//create sprites needed for the game (at least 3)

//user sprite
struct user_sprite {
	int32_t x; //x coordinate 0-127
	int32_t y ; //y coordinate 0-63
	const int16_t *image ; // image
	int32_t life ; //is user dead or alive 0 = dead, 1=alive
	int32_t lost ; // make user disappear if they lose
} ;
typedef struct user_sprite user1; //lets us refer to this struct with just user

//CPU alien sprite
struct cpu_sprite {
	int32_t x;
	int32_t y;
	const int16_t  *image;
	int32_t life ; 
	int32_t lost ; //if enemys are shot they disappear
};
typedef struct cpu_sprite cpu1;

struct laser_sprite {
	int32_t x;
	int32_t y;
	const int16_t *image;
	int32_t life ;
	int32_t lost ;
};
typedef struct laser_sprite laser1;


//move functions

//cpu 
//initial enemies = 4 

cpu1 Enemys[4];
laser1 Lasers[4];
int NeedToDraw;

//init for sprites (page 336 as example)

//use structs for variable declaration
user1 user;
cpu1 cpu;
laser1 laser;

void Init(void) {
	int i;
	
	//initial enemies = 4 
	int enemycount = 4;
	
	//initial user position on screen
	
	user.x = 50 ;
	user.y = 146;
	user.life = 1 ; //user is still alive
	user.lost = 0 ; //hasn't been hit yet
	
	user.image = PlayerShip0 ;
	
	//initial cpu position
	
	//4 enemys
	
	for(i = 0 ; i<enemycount ; i++) {
		Enemys[i].x = 20*i +12 ;
		Enemys[i].y = 10 ;
		Enemys[i].image = SmallEnemy10pointA ;
		Enemys[i].life = 1 ; //enemys are alive at the start 
	}
	
//send images to LCD
	
	
}



//draw player and enemy

	void DrawUser(void) {
		
		//only draw if user is alive 
		if(user.life == 1) {
			ST7735_DrawBitmap(user.x, user.y, user.image, 18, 8) ;
		}
		
		else if(user.lost) {
			ST7735_DrawBitmap(user.x, user.y, BigExplosion0, 18, 8); //if user gets hit too much draw them blowing up
		};
		
	}
	
	void DrawCPU(void) {
		
		int i;
		
		for(i = 0; i<4 ; i++ ) {
			if(Enemys[i].life == 1) {
				ST7735_DrawBitmap(Enemys[i].x, Enemys[i].y, Enemys[i].image, 16, 10) ; //only draw enemies if they are alive ;
			}
			else{
				if(Enemys[i].life) {
					ST7735_DrawBitmap(Enemys[i].x, Enemys[i].y, SmallExplosion1, 16, 10) ;
				}
			};
		};
	}

	//Draw Laser
	
	void DrawLaser(void){
		
		//total 4 lasers for 4 enemies
		
		int i;
		for(i = 0 ; i<4; i++) {
			if(Lasers[i].life == 1) {
				ST7735_DrawBitmap(Lasers[i].x,Lasers[i].y, Lasers[i].image, 18, 8) ;
			}
		}
	}
	
	
	//move User
	
	uint32_t Convert(uint32_t input){
  // write this
	int Position;
	//from calibration:
	
	//y = 
	// d = (1500*data)/4096+0 
	
	Position = ((1500*input)/4096+0) ; //converts data to fixed point distance

	
	return Position ;
	}
	
	void MoveUser(void) {
		
		uint32_t ADCData = ADC_In(); //gets input data from slide potentiometer  0-4095
		
		user.x = Convert(ADCData) ; //user moves in x direction only
		
	}
	
	//move CPU (page 336)
	
	void MoveCPU(void) {
		int i;
		for( i = 0 ; i<4 ; i++) {
			if(Enemys[i].y < 62) {
				Enemys[i].y += 2; //moves it down
			}
			else{
				Enemys[i].life = 0 ; //at bottom
			}
		}
	}
	
//move Laser fired by user
	
	void MoveLaser(void) {
		
		int i ;
		for(i=0 ; i<4; i++) {
			if(Lasers[i].life == 1) {}
			else {
				Lasers[i].y -= 2 ; //moves the positioning of missile
				
			}}}
	

			
//functionality of the game ;

			//check if lasers hit CPU enemy

	void Accuracy(void) {
		
		int i;
		
		if(Lasers[i].life == 1) {
			for(i = 0 ; i< 4 ; i++) {
				
				if((Lasers[i].x > Enemys[i].x) && (Lasers[i].y == Enemys[i].y)) {
					Lasers[i].life = 0;
				}
			}
		}
	}
	
	
	
	
	
	
	
	

	
	
//Hardware for circuit

//PORT E enable pe0,pe1
	
void Button1Init(void) {
	
	SYSCTL_RCGCGPIO_R  |= 0x10; //turns on the clock for Port E
	
	int delay = SYSCTL_RCGCGPIO_R ;  // waits for clock to stabilize
		
	//DIR register set PB5-0 as outputs
	
	GPIO_PORTE_DIR_R &= ~0x03 ; // sets PE0,1 as input 
	
	//digital enable for port B
	
	GPIO_PORTE_DEN_R |= 0x03 ; //enable for pe0-1
	
}

uint32_t PortEIn(void) {
	
	return GPIO_PORTE_DATA_R &0x03;
}

	
	
	
	
	


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		
//different game states 
		
	//win
	int CPUlife;
		int win;
		void Win(void) {
			if(CPUlife == 0) {
				win = 1 ;
			}
		}
		
	//loss(game finished)
		int Score; //score for the game
		
		
		void Loss(void) {
			ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutString("Final Score:");
			//from above
	LCD_OutDec(Score);  
		}
		
		
	void StartScreen(void) {
				ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("Welcome!");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Press 1 for English.");
		  ST7735_SetCursor(1, 3);

		  ST7735_OutString("Press 2 for Spanish");
  ST7735_SetCursor(1, 4);
  ST7735_OutString("Go Back!");
  ST7735_SetCursor(2, 4);
		
	}
		
	//check if user lost the game 
	int death;
	void CheckLoss(void) {
		
		int i;
		
		int scorecount = 0;
		
		for(i=0; i<4 ; i++ ) {
			if(Enemys[i].life == 0) {
				scorecount++ ;
			}
		}
		if(scorecount == 4 || user.life == 0) {
			
			death = 1;
		}
		else{
			death = 0;
		}
		
	}
		
		
		
//draw the game elements
		
		void Draw(void) {
			
			DrawUser();
			DrawCPU();
			DrawLaser();
			MoveUser();
			MoveCPU();
			
		}
		
		
	
		
			
		
		
				
		
		
		
		
	
		
	
	
	
	


	
	
	




















int lastE;
int semaphore;
int main(void){ char l;
	
	
	
	
	
	
  DisableInterrupts();
	  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	Random_Init(1);
	ADC_Init();
	DAC_Init();
	semaphore = 1;
	Output_Init();
	ST7735_FillScreen(0x0000); //sets screen to black
	Timer1_Init(80000000/30,4) ;
	//Init();
	Button1Init();
	//StartScreen();
	Loss();
	//Init();
	lastE = PortEIn();
	if((lastE & 0x01) == 0x01) {
		Init();
	}
	
	//PortEIn();

	EnableInterrupts();
	
	
  while(1){

							//	semaphore = 1;

		lastE = PortEIn();
		MoveUser();
		MoveCPU();
		
		if(semaphore) {
			Draw();
			MoveUser();
			semaphore = 0;
			//check if loss
			
		CheckLoss();
			if(death == 1) {
				Loss(); //display loss screen
		}
			Draw();

		
		
		

      }
    }
	}
	
	int thisPortE ;
	void Timer1AHandler(void) {
		TIMER1_ICR_R = TIMER_ICR_TATOCINT;
		uint32_t thisPortE = PortEIn();
		
		if(((lastE &0x01) == 0x01) && (thisPortE & 0x01) == 0x01) {

			uint32_t i = 0;
	}
		MoveUser();
	MoveCPU();
	MoveLaser();
	
		
}
