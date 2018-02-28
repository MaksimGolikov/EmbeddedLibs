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

typedef enum {
	BUTTON_RESET_EVENT,
	BUTTON_SHORT_EVENT,
	BUTTON_2S_EVENT,
	BUTTON_3S_EVENT,
	BUTTON_5S_EVENT,
	BUTTON_10S_EVENT,
	ButtonsEvent_Amounnt
} Button_Event_t;


typedef enum{
	TRIGGER_BUTTON,
	RECHARGE_BUTTON,
	POWER_BUTTON,
	Buttons_Amount
}buttons_t;

typedef enum{
	ON_PRESS,
	ON_RELEASE
}ButtonWorkMode_t;


typedef enum{
	key_reset,
	key_set
}ButtonState_t;



/**
* @brief function for initialize parameters start value_comp
* @param port - name of nesessary port
* @param pin - number of pin
  @param mode - state for recognize event
*/
void drv_Button_Init(GPIO_TypeDef *port, uint8_t  pin, ButtonWorkMode_t mode);


/**
*@brief function for monitoring button state 
*/
void drv_Button_Run(void);


/**
*@brief  function for get button event
*@param  buttonNumber - number of needed button
*@return event of  button
*/
Button_Event_t  drv_Button_GetEvent(buttons_t buttonNumber);

/**
*@brief  function for get button state
*@param  buttonNumber - number of needed button
*@return state of  button
*/
ButtonState_t  drv_Button_IsButtonPressed(buttons_t buttonNumber);

/**
  @brief   function for take press time of button
  @param   buttonNumber - number of needed button
  @return	 count of system tick when button was pressed
*/
uint32_t drv_Button_GetTimeOfLongPress(buttons_t buttonNumber);


/**
 * @brief function for take previous event
 * @param buttonNumber - number of needed button
 * @return
 */
Button_Event_t  drv_Button_GetPreviousEvent(buttons_t buttonNumber);



#endif /* DRV_BUTTON_ */
