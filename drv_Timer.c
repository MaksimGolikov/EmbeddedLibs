/*
 * Name         drv_Timer.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control timer (source file)
 */


#include "DRV/drv_Timer.h"

static uint32_t currentTime;
static TIM_HandleTypeDef *tactTime;;




void drv_Timer_Init(TIM_HandleTypeDef *timer){
	
	tactTime = timer;
	currentTime = 0;	
	HAL_TIM_Base_Start_IT(tactTime);
}
	

uint8_t  drv_Timer_isTimeSpent(uint32_t startTime, uint32_t pauseTime){
	uint8_t  result = 0;
	
	if( currentTime >= (startTime + pauseTime) ){
		result = 1;
	}
	return result;
}
	
uint32_t drv_Timer_getCurrentTime(void){
return currentTime;
}

void drv_Timer_Callback()
{	
  currentTime += 1;
}
