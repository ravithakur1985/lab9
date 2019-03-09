//Executive code file
#include "stm32f30x.h"
#include "stm32f3_Exec.h"

static uint32_t ExecTaskFlag;
static ExecFunctionPointer ExecTaskList[MAX_TASK_COUNT];

//
// This function will initialize the table of function pointers for the executive
//
void Exec_Init(void){
    uint32_t i;

    //
    // No tasks need to run yet
    //
    ExecTaskFlag = 0;
    
    //
    // Set all tasks to NULL, no task handler present
    //
    for (i=0; i<MAX_TASK_COUNT; i++){
        ExecTaskList[i] = NULL;
    }
    
    //
    // Add the actual tasks you want to run
    //
    ExecTaskList[0] = Button_Task;
    ExecTaskList[1] = TIM2_Task;
		ExecTaskList[2] = CLI_AddByte;
		ExecTaskList[3] = CLI_ProcessCommand;
		ExecTaskList[4] = Watchdog_wait;
    
    return;
}

//
// The executive, we never return from here.  We stay in our loop waiting for a task to be set to
// run.
//
void Exec_Start(void){
    uint32_t i;
    
    //
    // Do this forever!
    //
    while(1){    

	//
	// If ExecTaskFlag is not 0, some task needs to run
	//
        if (ExecTaskFlag){        
	    //
	    // Go through each task and check which bit is set in ExecTaskFlag
	    // 
            for(i=0; i< MAX_TASK_COUNT; i++){
		//
		// If this bit is set, clear the task (in case the task needs to set itself
		// up to run again) and if there is a handler go ahead and call it.
		//
                if (ExecTaskFlag & (0x1 << i)){
                    Exec_ClearTask(0x1 << i);
                    if (ExecTaskList[i]){    //This protects against calling a NULL handler!  That would crash us!
                        (ExecTaskList[i])(); //Call the Task we want to execute  
                    }
                }
            }                
        }else{
            //
            // No tasks are needed, go to sleep -- DO NOT wfi, causes problems with debugger!
            //           
            __nop();
        }
    }
}

//
// Set the given task to run
//
void Exec_SetTask(uint32_t task){
    
    //
    // Set a bit indicating a specific task is needed to run
    //
    ExecTaskFlag |= task;
    return;
}

//
// Clear the given task so it does not run again until set again
//
void Exec_ClearTask(uint32_t task){
    
    //
    // Clear a bit indicating a specific task has executed
    //
    ExecTaskFlag &= ~task;
    return;
}

