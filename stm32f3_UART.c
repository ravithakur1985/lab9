///////////////////////////////////////
// stm32f3_UART code file            //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_UART.h"
#include "stdio.h"

void UART_Init(void){
	// disable UART2
	USART2->CR1 &= ~USART_CR1_UE;
	// enable clock to GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// clock enable UART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// put GPIOS PA2 & PA3 in alternate mode
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;	
	// set alternate funcion as USART for GPIOS PA2 & PA3
	// PA2 USART2_TX and PA3 USART2_RX
	GPIOA->AFR[0] |= 0x7700;
	// configure BAUD rate
	// 36000000/115200 = 'd312 or 'h138
	USART2->BRR |= 0x138;
	// configure UART for 8bits, no parity and 1 stop bit
	USART2->CR1 &= ~WRD_LEN_8BITS_N;
	USART2->CR1 &= ~USART_CR1_PCE;
	USART2->CR2 &= ~USART_CR2_STOP;	
	// also enable transmitter and receiver
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_TE;
	// enable receiver not empty interrupt
	USART2->CR1 |= USART_CR1_RXNEIE;	
	// enable NVIC interrupt
	NVIC_EnableIRQ(USART2_IRQn);
	// enable UART 2
	USART2->CR1 |= USART_CR1_UE;
	return;
}

void UART_Shutdown(void){
	RCC->AHBENR &= ~RCC_APB1ENR_USART2EN;
	return;
}

void USART2_IRQHandler(void){
	// clear the RXNE flag
  USART2->RQR |= USART_RQR_RXFRQ;
	// clear nvic interrupt
	NVIC_ClearPendingIRQ(USART2_IRQn);
	Exec_SetTask(EXEC_TASK_CLI_ADDBYTE);
	return;
}

int fputc(int ch, FILE *f){	
	USART2->TDR = ch;	
	while((USART2->ISR & USART_ISR_TXE) == 0){		
		__nop();
	}
	return ch;
}



