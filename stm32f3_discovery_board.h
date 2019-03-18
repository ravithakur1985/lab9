///////////////////////////////////////
// stm32f3_discovery_board hdr file  //
// Author: Ravi Thakur               //
// Version 1.0                       //
//                                   //
///////////////////////////////////////

#ifndef __stm32f3_discovery_board_H__
#define __stm32f3_discovery_board_H__

#include "stdint.h"

int16_t Get_temperature(void);
uint32_t LSM303DLHC_TIMEOUT_UserCallback(void);


#endif

