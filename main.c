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
#include "stm32f3_discovery_board.h"
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stdio.h"


int main (void){
	
	LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStruct;
	
	// running the init commands	
	LEDS_Init();
	Exec_Init();	
	Button_Init();
	TIM2_Init();
	UART_Init();
	DMA_Start();
	CLI_Init();	
	
	LSM303DLHC_InitStruct.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_ENABLE;
	LSM303DLHC_MagInit(&LSM303DLHC_InitStruct);
	
	if (Watchdog_POR()){
		printf("Watchdog Reset\n");
		printf("\n\rRaviThakur>");
	}
	
	Watchdog_Init();
	__enable_irq();	

	// starting the executive
	Exec_Start();	
}

