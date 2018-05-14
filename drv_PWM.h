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


/**
 * @brief fnction for init PWM
 * @param timer  what timer control PWM * 
 */
void drv_PWM_Init(TIM_HandleTypeDef *timer, uint8_t channel);


/**
*@brief function for set pulse of pwm
*@param numPWM - number necessary pwm
*@param intensity - value which should be set 
*/
void drv_PWM_SetPuls(uint8_t numPWM, uint16_t intensity);


/**
*@brief function for get max pulse of pwm
*@param numPWM - number necessary pwm
*@return max value off pulse
*/
uint16_t drv_PWM_GetPuls(uint8_t numPWM);


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


/**
  @brief  function for take max count of  PWM
	@return max count of  PWM
*/
uint8_t drv_PWM_GetMaxQuontityOfPWM(void);
#endif /* DRV_PWMH_ */
