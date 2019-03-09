///////////////////////////////////////
// stm32f3_GeneralTimer2 header file //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_GeneralTimer2_H__
#define __stm32f3_GeneralTimer2_H__

//declaring prototypes
void TIM2_IRQHandler(void);
void TIM2_Init(void);
void TIM2_Shutdown(void);
void TIM2_Task(void);

#endif
