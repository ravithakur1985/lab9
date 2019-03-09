///////////////////////////////////////
// stm32f3_Button code                //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_Button.h"
#include "stm32f3_LEDS.h"

//defining the finctions

void Button_Init(void){
	// clock enable GPIO A	
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// gpio mode is default input
	// gpio PA0 is connected to EXTI0 by default
	// unmask EXTI0 interrupt
	EXTI->IMR |= EXTI_IMR_MR0;
	// look for rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0;
	// enable NVIC interrupt
	NVIC_EnableIRQ(EXTI0_IRQn);
	return;
}

void Button_Shutdown(void){
	// clock disable
  RCC->AHBENR &= ~RCC_AHBENR_GPIOEEN;
	return;
}

void Button_Task(void){
	LEDS_Toggle(LED_0);
	return;
}

void EXTI0_IRQHandler(void){
	// Clear EXTI pending
	EXTI->PR |= EXTI_PR_PR0;
	// Clear pending interrupt in NVIC
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	//run the button task
	Exec_SetTask(EXEC_TASK_BUTTON);	
	return;
}

