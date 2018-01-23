/*
 * Name         drv_PWM.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control PWM (header file)
 */




#ifndef DRV_PWM_H_
#define DRV_PWM_H_

#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "DRV/drv_Timer.h"


/**
 * @brief fnction for init PWM
 * @param timer  what timer control PWM * 
 */
void drv_PWM_Init(TIM_HandleTypeDef *timer, uint8_t channel);


/**
 * @brief function for start generate PWM
 * @param numPWM  what timer control PWM 
 */
void drv_PWM_Start(uint8_t numPWM);

/**
 * @brief function for stop generate PWM
 * @param numPWM  what timer control PWM
 */
void drv_PWM_Stop(uint8_t numPWM);

/**
 * @brief function for launch
 */
void drv_PWM_Run(void);


/**
  @brief  function for take count of used PWM
	@return count of used PWM
*/
uint8_t drv_PWM_GetCurrentQuontityOfPWM(void);


#endif /* DRV_PWMH_ */
