/*
 * Name         drv_Timer.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control timer (source file)
 */


#include "DRV/drv_Timer.h"


typedef struct{
	uint32_t currentTime;
	TIM_HandleTypeDef *tactTime;
}TimerDescription_t;


static uint8_t countTimers = 0;

static TimerDescription_t timersMass[] = {
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
	{ 0, 0},
};



void drv_Timer_Init(TIM_HandleTypeDef *timer){
	
	timersMass[countTimers].currentTime = 0;
	timersMass[countTimers].tactTime = timer;
	
	HAL_TIM_Base_Start_IT(timersMass[countTimers].tactTime);
	
	countTimers += 1;
}
	

uint8_t  drv_Timer_isTimeSpent(timers_t numTimer, uint32_t startTime, uint32_t pauseTime){
	uint8_t  result = 0;	
	if( (timersMass[(uint8_t)numTimer].currentTime - startTime) > pauseTime ){
		result = 1;
	}
	return result;
}
	
uint32_t drv_Timer_getCurrentTime(timers_t numTimer){
return timersMass[(uint8_t)numTimer].currentTime;
}

void drv_Timer_Callback(timers_t numTimer)
{	
  timersMass[(uint8_t)numTimer].currentTime += 1;
}
