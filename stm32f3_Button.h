///////////////////////////////////////
// stm32f3_Button header file        //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_Button_H__
#define __stm32f3_Button_H__

//declaring prototypes
void Button_Init(void);
void Button_Shutdown(void);
void EXTI0_IRQHandler(void);
void Button_Task(void);

#endif
