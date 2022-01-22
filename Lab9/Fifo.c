// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 11/11/2021 
// Student names: change this to your names or look very silly
#include <stdint.h>

// Declare state variables for FiFo
//        size, buffer, put and get indexes


// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations

//create globals 

#define SIZE 9 
int FIFO[SIZE] ;
int putI;
int getI;


void Fifo_Init() {
//Complete this
	
	
	
	// set putI and getI to 0 
	putI = 0 ;
	getI = 0 ;
	
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
  //Complete this routine
	//page 326 textbook
	
	//array full when putI + 1 = getI
	//empty when getI = putI
	
	
	
	
	//putI +1 %array size = pointer to start
		
		//check if queue is not full : 

	if(((putI + 1)%SIZE) == getI) {

//array full, return failure 

			return 0 ;
	}
	
	FIFO[putI] = data ; //saves in FIFO
	
	putI = (putI + 1) %SIZE ; //next place to Put
	
	
	
	
	
	
	
	

   return 1; 

}

// *********** Fifo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){
  //Complete this routine
	
	//empty when getI = putI
	
	if(putI == getI ) {
		
		return 0 ; //failure array is empty 
	}
	
	*datapt = FIFO[getI] ; //retrieves the data
	getI = (getI +1 ) %SIZE ; //next place to get
	

	
	
	//page 326 
	

   return 1;
}




