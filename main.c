///////////////////////////////////////
// Lab4 main.c                       //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_LEDS.h"
#include "stm32f3_Button.h"
#include "stm32f3_GeneralTImer2.h"
#include "stm32f3_UART.h"
#include "cli.h"
#include "stm32f3_Watchdog.h"
#include "stm32f3_DMA.h"
#include "stdio.h"


int main (void){
	
	// running the init commands	
	LEDS_Init();
	Exec_Init();	
	Button_Init();
	TIM2_Init();
	UART_Init();
	DMA_Start();
	CLI_Init();
	
	if (Watchdog_POR()){
		printf("Watchdog Reset\n");
		printf("\n\rRaviThakur>");
	}
	
	Watchdog_Init();
	__enable_irq();	

	// starting the executive
	Exec_Start();	
}

