// Music.c
// playing your favorite song.
//
// For use with the TM4C123
// EE319K lab6 extra credit
// Program written by: put your names here
// 1/6/21

#include "Sound.h"
#include "DAC.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Port B bits 5-0 have the 6-bit DAC
// Port E bits 3-0 have 4 piano keys



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Music_Init(void){


}



// Play song, while button pushed or until end
void Music_PlaySong(void){
 
  
}

// Stop song
void Music_StopSong(void){
  
}





