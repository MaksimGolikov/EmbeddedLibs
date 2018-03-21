/*
 * Name         drv_Timer.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control timer (header file)
 */


#ifndef DRV_TIMER_H_
#define DRV_TIMER_H_

#include "stm32f1xx_hal.h"
#include "stdint.h"


typedef enum {
	TIM_R,
	TIM_G,
	TIM_B,
	TIM_Amount
}timers_t;


/**
*@brief function for initialize timer clock
*@param timer - pointer to timer configuration
*/
void     drv_Timer_Init(TIM_HandleTypeDef *timer);



/**
*@brief  function for check is time after start spent
*@param numTimer - name timer which should use
*@param start time - time from what shout be delay
*@param pauseTime - necessary delay time
*@return 1 if time spent, else 0
*/
uint8_t  drv_Timer_isTimeSpent(timers_t numTimer, uint32_t startTime, uint32_t pauseTime);

/**
*@brief function for take time after start microcontroller 
*@param numTimer - name timer which should use
*@rerturn count periods after start
*/
uint32_t drv_Timer_getCurrentTime(timers_t numTimer);

/**
*@brief function for increnent timer
*@param numTimer - name timer which should use
*/
void drv_Timer_Callback(timers_t numTimer);

#endif /* DRV_TIMER_H_ */
