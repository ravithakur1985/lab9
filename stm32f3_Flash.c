///////////////////////////////////////
// stm32f3_Flash code file           //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Flash.h"

static void FlashUnlock(void){
	 if((FLASH->CR & FLASH_CR_LOCK) != 0){
		 __disable_irq();
		 // Authorize the FLASH Registers access
		 FLASH->KEYR = FLASH_KEY1;
		 FLASH->KEYR = FLASH_KEY2;
		 __enable_irq();
  } 
	return;
}

static void Flashlock(void){
	// Setting the LOCK Bit
  FLASH->CR |= FLASH_CR_LOCK;
	return;
}


uint32_t FlashWrite16(uint32_t addr, uint16_t data){
	uint16_t tmpData = 0;
	
	// Unlock Flash
  FlashUnlock();
	//set flag to program the flash
	FLASH->CR |= FLASH_CR_PG;
	//writing the data
	*(uint16_t*)addr = data;
	//wait if busy
	while((FLASH->SR & FLASH_SR_BSY) == 1){
		__nop();
	}
	//disable programming flag
	FLASH->CR &= ~FLASH_CR_PG;
	//Setting the LOCK bit
  Flashlock();
	
	// reading back data to verify write
	tmpData = *(uint16_t*)addr;
	if(tmpData == data){
		return 1;
	} else {
		return 0;
	}
}
uint32_t FlashErasePage(uint32_t address){
	// Unlock Flash
	FlashUnlock();
	//set page erase
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR  = address;
	FLASH->CR |= FLASH_CR_STRT;
	//wait if busy
	while((FLASH->SR & FLASH_SR_BSY) == 1){
		__nop();
	}
	//reset page erase
	FLASH->CR &= ~FLASH_CR_PER;
	//Setting the LOCK bit
  Flashlock();
	//confirming erase
	if (FLASH->SR & FLASH_SR_EOP){
		FLASH->SR |= FLASH_SR_EOP;
		return 1;
	} else {
		return 0;
	}
	
}

