///////////////////////////////////////
// stm32f3_Watchdog header file      //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_Watchdog_H__
#define __stm32f3_Watchdog_H__

void Watchdog_Init(void);
void Watchdog_Pet(void);
uint32_t Watchdog_POR(void);
void Watchdog_Trigger(void);
void Watchdog_wait(void);

#define IWDG_Enable         0x0000cccc
#define IWDG_EnableAccess   0x00005555

#endif
