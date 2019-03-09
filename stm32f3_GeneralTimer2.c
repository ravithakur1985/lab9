///////////////////////////////////////
// stm32f3_GeneralTimer2 code        //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_GeneralTImer2.h"
#include "stm32f3_LEDS.h"

void TIM2_IRQHandler(void){
	// Clear the timer interrupt
	TIM2->SR = 0;
	// Clear pending interrupt in NVIC
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	Exec_SetTask(EXEC_TASK_TIM2);		
	return;
}

void TIM2_Init(void){
	// disable timer 2
	TIM2->CR1 &= ~TIM_CR1_CEN;
	// clock enable timer2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// set dir to count down
	TIM2->CR1 |= TIM_CR1_DIR;
	// configure to auto reload
	TIM2->CR1 |= TIM_CR1_ARPE;
	// configure count value
	TIM2->ARR = 71999999;
	// enable timer 2 interrupt
	TIM2->DIER |= TIM_DIER_UIE;
	// enable NVIC interrupt
	NVIC_EnableIRQ(TIM2_IRQn);
	// halt timer when debugging
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
	// enable timer 2
	TIM2->CR1 |= TIM_CR1_CEN;
	return;
}
void TIM2_Shutdown(void){
	// disable timer 2
	TIM2->CR1 &= ~TIM_CR1_CEN;
	// disable timer 2 interrupt
	TIM2->DIER &= ~TIM_DIER_UIE;
	// clock disable timer2
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;	
	return;
}

void TIM2_Task(void){
	LEDS_Toggle(LED_1);
	return;
}
