///////////////////////////////////////
// stm32f3_Flash header file         //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_Flash_H__
#define __stm32f3_Flash_H__

static void FlashUnlock(void);
static void Flashlock(void);
uint32_t FlashWrite16(uint32_t addr, uint16_t data);
uint32_t FlashErasePage(uint32_t address);

#endif


