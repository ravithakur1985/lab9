///////////////////////////////////////
// stm32f3_discovery_board code file //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#include "stm32f3_discovery_board.h"
#include "stm32f3_discovery_lsm303dlhc.h"


int16_t Get_temperature(void){
	uint8_t temp_h, temp_l;
	int16_t temp;
	LSM303DLHC_Read(MAG_I2C_ADDRESS,LSM303DLHC_TEMP_OUT_H_M, &temp_h, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS,LSM303DLHC_TEMP_OUT_L_M, &temp_l, 1);
	
	temp = temp_l | (temp_h << 8);
	temp = (temp >> 4);
	return temp;
}

uint32_t LSM303DLHC_TIMEOUT_UserCallback(void){
	return 0;
}

