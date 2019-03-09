///////////////////////////////////////
// stm32f3_DMA header file           //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_DMA_H__
#define __stm32f3_DMA_H__

#include "stm32f30x.h"

void DMA_Start(void);
void DMA_MemCopy(DMA_Channel_TypeDef* dma, uint32_t src,
	               uint32_t dst, uint16_t count);
void DMA_MemSet(DMA_Channel_TypeDef* dma, uint32_t dst,
	               uint32_t data, uint16_t count);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

//returns the IRQ value for the dma channel pointer passed
static IRQn_Type dma_ch(DMA_Channel_TypeDef* dma);

#endif


