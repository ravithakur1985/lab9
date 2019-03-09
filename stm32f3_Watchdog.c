///////////////////////////////////////
// stm32f3_Watchdog code file        //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f3_Exec.h"
#include "stm32f3_Watchdog.h"

//wait for some counts to pet the dog
static uint32_t count = 36000;
//value to refresh the watchdog
static uint32_t refresh = 0x0000aaaa;


void Watchdog_Init(void){
	//assert LSION
	//LSI osc is about 40KHz
	RCC->CSR |= RCC_CSR_LSION;
	//wait for LSIRDY = 1
	while((RCC->CSR & RCC_CSR_LSIRDY) == 0){}
	//confi IWDG with window option disabled
	//enable IWDG
	IWDG->KR = IWDG_Enable;
	//enable register access
	IWDG->KR = IWDG_EnableAccess;
	// (40KHz/32)/625 = 500ms watchdog period
	//set prescaler to 3 which is /32
	IWDG->PR = 0x03;
	//set reload register to 625
	IWDG->RLR = 0x271;
	//wait for prescaler update to finish
	while((IWDG->SR & IWDG_SR_PVU) != 0) {}
	//wait for reload update to finish
	while((IWDG->SR & IWDG_SR_RVU) != 0) {}
	//refresh counter
	IWDG->KR = refresh;
	// halt timer when debugging
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	//call Watchdog_Pet
	Watchdog_Pet();
	return;
}

void Watchdog_Pet(void){
	//refresh counter
	IWDG->KR = refresh;
	Exec_SetTask(EXEC_TASK_WATCHDOG);
	return;
}

uint32_t Watchdog_POR(void){
	//check if watchdog caused reset
	if (RCC->CSR & RCC_CSR_IWDGRSTF){
		//clear the reset flag
		RCC->CSR |= RCC_CSR_RMVF;
		return 1;
	} else {
		return 0;
	}
}

//wait for the global count to expire
//this is executed by Exec_SetTask(EXEC_TASK_WATCHDOG) call
void Watchdog_wait(void){
	volatile uint32_t i;
	
	for(i=0;i<count;i++){
	}
	Watchdog_Pet();
	return;
}

//affecting the value that get written in
//the pet function to cause watchdog timer
//to expire
void Watchdog_Trigger(void){	
	refresh = 0x0000cccc;
	return;
}



