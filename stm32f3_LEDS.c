///////////////////////////////////////
// stm32f3_LEDS code                 //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_LEDS.h"


//defining the functions

void LEDS_Init(void) {
	// clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	// set all LEDs to output
	GPIOE->MODER |= GPIO_MODER_MODER8_0;
	GPIOE->MODER |= GPIO_MODER_MODER9_0;
	GPIOE->MODER |= GPIO_MODER_MODER10_0;
	GPIOE->MODER |= GPIO_MODER_MODER11_0;
	GPIOE->MODER |= GPIO_MODER_MODER12_0;
	GPIOE->MODER |= GPIO_MODER_MODER13_0;
	GPIOE->MODER |= GPIO_MODER_MODER14_0;
	GPIOE->MODER |= GPIO_MODER_MODER15_0;
	return;
}

void LEDS_Shutdown(void){
	// clock disable
  RCC->AHBENR &= ~RCC_AHBENR_GPIOEEN;
	// set all LEDs to input
	GPIOE->MODER &= ~GPIO_MODER_MODER8_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER9_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER10_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER11_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER12_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER13_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER14_0;
	GPIOE->MODER &= ~GPIO_MODER_MODER15_0;
	return;
}

void LEDS_On(LEDS_ENUM led){
	GPIOE->ODR |= led;
	return;
}

void LEDS_Off(LEDS_ENUM led){
	GPIOE->ODR &= ~led;
	return;
}

void LEDS_Toggle(LEDS_ENUM led){
	GPIOE->ODR ^= led;
	return;
}


