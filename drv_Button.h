/*
 * Name         drv_Button.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (header file)
 */


#ifndef DRV_BUTTON_H_
#define DRV_BUTTON_H_


#include "stm32f1xx_hal.h"
#include "stdint.h"

typedef enum{
  BUTTON_RESET_EVENT,
  BUTTON_SHORT_EVENT,
  BUTTON_2S_EVENT,
  BUTTON_3S_EVENT,
  BUTTON_5S_EVENT,
  BUTTON_10S_EVENT,
  BUTTON_20S_EVENT,
  BUTTON_30S_EVENT,
  button_events_Amount
}Button_Event_t;


typedef enum{
	BUTTON_POWER,
	BUTTON_FIRE,
	BUTTON_RECHARGE,
	buttons_Amount
}buttons_t;

typedef enum{
	ON_PRESS,
	ON_RELEASE
}ButtonWorkMode_t;

/**
* @brief function for initialize parameters start value_comp
* @param buttonName - name of initialize button
* @param port       - name of necessary port
* @param pin        - number of pin
  @param mode       - state for recognize event
  @return initialization status
*/
int8_t drv_Button_Init(buttons_t buttonName,GPIO_TypeDef *port, uint8_t  pin, ButtonWorkMode_t mode);


/**
*@brief function for monitoring button state 
*/
void drv_Button_Run(void);


/**
*@brief  function for get button event
*@param  number of need button from 
*@return event of  button
*/
Button_Event_t  drv_Button_GetEvent(buttons_t buttonNumber);

/**
*@brief  function for get button state
*@param  number of need button from 
*@return state of  button
*/
uint8_t  drv_Button_IsButtonPressed(buttons_t buttonNumber);

/**
  @brief   function for take press time of button
  @param   buttonNumber name of necessary button
  @return	 count of system tick when button was pressed
*/
uint32_t drv_Button_GetTimeOfLongPress(buttons_t buttonNumber);





#endif /* DRV_BUTTON_ */
