///////////////////////////////////////
// stm32f3_Tools code                //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////
#include "stm32f30x.h"
#include "stm32f3_Tools.h"


//implementing the wait function

void Wait (uint32_t time){
	volatile uint32_t i;
	
	for(i=0;i<time;i++){
	}
	return;
}

