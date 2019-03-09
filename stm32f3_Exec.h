//Executive header file
#ifndef __stm32f3_EXEC_H__
#define __stm32f3_EXEC_H__
#include "stm32f30x.h"

#define MAX_TASK_COUNT 32
#define NULL 0
typedef void (*ExecFunctionPointer)(void);

void Exec_Init(void);
void Exec_Start(void);
void Exec_SetTask(uint32_t task);
void Exec_ClearTask(uint32_t task);


extern void Button_Task(void);
extern void TIM2_Task(void);
extern void CLI_AddByte(void);
extern void CLI_ProcessCommand(void);
extern void Watchdog_wait(void);

#define EXEC_TASK_BUTTON      0x00000001
#define EXEC_TASK_TIM2        0x00000002
#define EXEC_TASK_CLI_ADDBYTE 0x00000004
#define EXEC_TASK_CLI_PROCESS 0x00000008
#define EXEC_TASK_WATCHDOG    0x00000010

#endif

