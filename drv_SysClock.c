/*
 * Name         drv_SysClock.c
 * Author       Maksim Golikov (SW developer)
 * Created      4 JUN 2017
 * Description  driver for System clock (source file)
 */

#include "drv_SysClock.h"


static uint32_t currentTime;



void drv_SysClock_Init(void){

	currentTime=0;	
}

uint32_t drv_SysClock_GetCurrentTime(void){
	return currentTime;
}
uint8_t drv_SysClock_IsTimeSpent(uint32_t startTime,uint32_t delayTime){
	uint8_t returnedValue=0;

	if( (currentTime-startTime) > delayTime)
	{
		returnedValue=1;
	}

	return returnedValue;
}


void HAL_SYSTICK_Callback(void) {
	currentTime +=1;
}

