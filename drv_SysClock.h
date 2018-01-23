/*
 * Name         drv_SysClock.h
 * Author       Maksim Golikov (SW developer)
 * Created      4 JUN 2017
 * Description  driver for System clock (header file)
 */

#ifndef DRV_SYSCLOCK_H_
#define DRV_SYSCLOCK_H_


#include "stm32f1xx_hal.h"

/**
 @brief function for initialize parameterswith default value
*/
void drv_SysClock_Init(void);

/**
@brief  function for take time after start 
@return count of system tick after start power 
*/
uint32_t drv_SysClock_GetCurrentTime(void);

/**
 @brief function   for check is time spent
 @param startTime  time after what mast be pause
 @param delayTime  necessary time of pause (count of sys tick)
 @return if time spent 1 else 0
*/
uint8_t drv_SysClock_IsTimeSpent(uint32_t startTime,uint32_t delayTime);


#endif /* DRV_SYSCLOCK_H_ */
