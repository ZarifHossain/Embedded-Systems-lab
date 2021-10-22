// ****************** Lab2.c ***************
// Program written by: put your names here
// Date Created: 1/18/2017 
// Last Modified: 8/28/2021 
#include "Lab2.h"
// Put your name and EID in the next two lines
char Name[] = "Zarif Hossain";
char EID[] = "zmh345";
// Brief description of the Lab: 
// An embedded system is capturing data from a
// sensor and performing analysis on the captured data.
//   The three analysis subroutines are:
//    1. Calculate the average error
//    2. Perform a linear equation using integer math 
//    3. Check if the captured readings are a monotonic series
// Possibility 1)
//       Return 1 if the readings are in non-increasing order.
//       Examples:
//         10,9,7,7,5,2,-1,-5 is True=1
//         2,2,2,2 is True=1
//         9,7,7,5,6,-1,-5 is False=0 (notice the increase 5 then 6)
//         3,2,1,0,1 is False (notice the increase 0 then 1)
// Possibility 2)
//       Return 1 if the readings are in non-decreasing order.
//       Examples:
//         -5,-1,2,5,7,7,9,10 is True=1
//         2,2,2,2 is True=1
//         -1,6,5,7,7,9,10 is False=0 (notice the decrease 6 then 5)
//         1,2,3,4,3 is False=0 (notice the decrease 4 then 3)
#include <stdint.h>

// Inputs: Data is an array of 32-bit signed measurements
//         N is the number of elements in the array
// Let x0 be the expected or true value
// Define error as the absolute value of the difference between the data and x0
// Output: Average error
// Notes: you do not need to implement rounding
// The value for your x0 will be displayed in the UART window

// 
//EE319K Fall 2021 Lab 2
//Zarif Hossain EID=ZMH345
//Test of your AverageError. x0=383
//No, Input is {419,410,455,410,446,464,410,428,446}
 //Correct Average=49, Your Average=42, Score=0
//Test of your Linear. x1=-100, y1=-70000, x2=100, y2=70000
//No, x=-83, Correct y=-58100, Your y=42, Score=0
//Test of your IsMonotonic. Return true if the readings are in non-increasing order.
//No, Input is {10,9,7,7,5,2,-1,-5}
 //Correct Answer=1, Your Answer=42, Score=0 End of Analysis




int32_t AverageError(const int32_t Readings[], const uint32_t N){
//initilization of variables needed 
	
	int i; 
	int sum = 0;
	int xinitial = 383;
	int avgerror;
	
	//for loop to calculate average error 
	
	for(i = 0; i < N; i++) { 
		int error;
		error = Readings[i] - xinitial;    //this part of code takes the readings and finds the average error by subtracting the reading from the initial x0.
		
		if(error <= 0){
			error = abs(error);
			sum = sum + error;    //checks to see if value is negative. If it is, we want to take the absolute value.
		}
		else{
			sum = sum + error; //if error isn't negative just add it to the average error sum normally. 
		}
		
  
}
	//calculate avg error

	return sum/N; //returns the calculations of average error
}

// Consider a straight line between two points (x1,y1) and (x2,y2)
// Input: x an integer ranging from x1 to x2 
// Find the closed point (x,y) approximately on the line
// Output: y
// Notes: you do not need to implement rounding
// The values of (x1,y1) and (x2,y2) will be displayed in the UART window

int32_t Linear(int32_t const x){
// Replace this following line with your solution
	//x1=-100, y1=-70000, x2=100, y2=70000

	//inilize variables
	
	int x1 = -100;
	int y1 = -70000;
	int x2 = 100;
	int y2 = 70000;
	
	//calculating slope
	int yvalues = 0;
	int xvalues = 0;
	int slope;
	
	yvalues = y2 - y1;
	xvalues = x2-x1;
	slope = yvalues / xvalues;
	//calculate value of constant c
	int constant;
	constant = y2 - (slope*x2);
	
	//return value of y
	
	int y;
	y = (slope*x) + constant;
	
	
	
  return y;
}

// Return 1 or 0 based on whether the readings are a monotonic series
// Inputs: Readings is an array of 32-bit measurements
//         N is the number of elements in the array
// Output: 1 if monotonic, 0 if nonmonotonic
// Whether you need to implement non-increasing or non-decreasing will be displayed in the UART window
int IsMonotonic(int32_t const Readings[], uint32_t const N){
// Replace this following line with your solution
	int32_t i;
	
	int monotonic;
	
	
	//check if values are non-increasing
	
	
		
		for(i = 0; i < N - 1 ; i++) { //check if current element is less than the next element. If it is, then we don't have a non increasing series and want to return a 0
				monotonic = 0;
			if(Readings[i] < Readings[i+1]){  //compares first element in readings array to the next element 
				return monotonic;
			}
			else{
				monotonic = 1;}   // if the current reading is greater than next element, then we have a non-increasing sereis.
		
	}
		return monotonic;
	
}		
