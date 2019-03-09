///////////////////////////////////////
// stm32f3_DMA code file             //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_DMA.h"
#include "stdio.h"

void DMA_Start(void){
	//turn on the clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//clear all the interrupt flags
	DMA1->IFCR |= 0x0fffffff;
	return;
}
static IRQn_Type dma_ch(DMA_Channel_TypeDef* dma){
	 if (dma == DMA1_Channel1){
		return DMA1_Channel1_IRQn;
	} else if (dma == DMA1_Channel2){
		return DMA1_Channel2_IRQn;
	} else if (dma == DMA1_Channel3){
		return DMA1_Channel3_IRQn;
	} else if (dma == DMA1_Channel4){
		return DMA1_Channel4_IRQn;
	} else if (dma == DMA1_Channel5){
		return DMA1_Channel5_IRQn;
	} else if (dma == DMA1_Channel6){
		return DMA1_Channel6_IRQn;
	} else {
		return DMA1_Channel7_IRQn;
	}
}

void DMA_MemCopy(DMA_Channel_TypeDef* dma, uint32_t src,
	               uint32_t dst, uint16_t count){
	//disable the channel
	dma->CCR &= ~DMA_CCR_EN;
	//configure the channel: mem2m2m, 32 bit, incr mode,
	//read from mem, enable transfer complete and error intr
	dma->CCR |= DMA_CCR_MEM2MEM | DMA_CCR_PSIZE_1 |
				      DMA_CCR_MSIZE_1 | DMA_CCR_PINC |
				      DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE |
				      DMA_CCR_TEIE;
	//set the count
    dma->CNDTR = count;
	//set mem addr
	dma->CMAR = src;
	//set dst addr
	dma->CPAR = dst;
	//enable NVIC
    NVIC_EnableIRQ(dma_ch(dma));	
	//enable the channel
	dma->CCR |= DMA_CCR_EN;
	return;
}
								 
void DMA_MemSet(DMA_Channel_TypeDef* dma, uint32_t dst,
	               uint32_t data, uint16_t count){
	uint32_t* data_ptr = &data;

	//disable the channel
	dma->CCR &= ~DMA_CCR_EN;
	//configure the channel: mem2m2m, 32 bit, periph incr mode,
	//read from mem, enable transfer complete and error intr
	dma->CCR |= DMA_CCR_MEM2MEM | DMA_CCR_PSIZE_1 |
				      DMA_CCR_MSIZE_1 | DMA_CCR_PINC |
				      DMA_CCR_DIR | DMA_CCR_TCIE | DMA_CCR_TEIE;
	//set the count
    dma->CNDTR = count;
	//set src addr	
	dma->CMAR = (uint32_t)data_ptr;
	//set dst addr
	dma->CPAR = dst;
	//enable NVIC
	NVIC_EnableIRQ(dma_ch(dma));  
	//enable the channel
	dma->CCR |= DMA_CCR_EN;
	return;
}
								 
void DMA1_Channel1_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF1) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
	}
   	NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
	return;
}

void DMA1_Channel2_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF2) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF2;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
	return;
}

void DMA1_Channel3_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF3) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
	return;
}

void DMA1_Channel4_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF4) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
	return;
}

void DMA1_Channel5_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF5) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF5;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
	return;
}

void DMA1_Channel6_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF6) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
	return;
}

void DMA1_Channel7_IRQHandler(void){
	//detect complete transfer interrupt
	if ((DMA1->ISR & DMA_ISR_TCIF7) != 0){
		//clear the interrupt
		DMA1->IFCR |= DMA_IFCR_CTCIF7;
	}
	NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn);
	return;
}


