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



//brief function for initialize timer clock
void     drv_Timer_Init(TIM_HandleTypeDef *timer);

//brief  function for check is time after start spent
//param  start time 
//param  time of pause in mcs
//return 
uint8_t  drv_Timer_isTimeSpent(uint32_t startTime, uint32_t pauseTime);

//brief  function for take time after start microcontroller 
//rerturn count periods after start
uint32_t drv_Timer_getCurrentTime(void);

void drv_Timer_Callback(void);

#endif /* DRV_TIMER_H_ */
