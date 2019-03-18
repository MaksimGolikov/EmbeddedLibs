/*
 * Name         drv_Button.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (header file)
 */


#ifndef DRV_BUTTON_H_
#define DRV_BUTTON_H_


#include <stdint.h>
#include <stdbool.h>


typedef enum{
	ON_PRESS,
	ON_RELEASE
}ButtonWorkMode_t;

typedef enum{
	key_reset,
	key_set
}ButtonState_t;


typedef struct{
	uint32_t           startTime;

    uint32_t           jitter_time_delay;
	uint32_t           jitter_timeout;
	uint8_t            jitter_status_pin;

	ButtonWorkMode_t   workMode;

	uint8_t            buttonEvent;
	ButtonState_t      keyState;
  
	ButtonState_t      (*PinStateReader)(void);
	uint32_t           (*GetTime)(void);
	void               (*Callback)(uint8_t ev);
}ButtonContext_t;


/**
* @brief function for initialize parameters start value_comp
* @param keyDeff        - pointer to struct which should be initialize
  @param mode           - state for recognize event
  @param handlerPin     - pointer to function which return state of pin function
  @param handlerTimer   - pointer to function which return time
  @param TimeMass       - pointer to massive with timeout deffinitions
  @param numberOfEvents - number of possible events
  @param jitterDelay    - time for digital filters
  @return operation status
*/
int8_t drv_Button_Init(ButtonContext_t *keyDeff, ButtonWorkMode_t mode, 
                       void *handlerPin, void *handlerTimer, uint16_t *TimeMass,
											 uint8_t numberOfEvents, uint8_t jitterDelay);


/**
* @brief function for initialize callback for baton change event
* @param keyDeff        - pointer to struct which should be contain callback
* @param ptrCallback    - pointer to callback. As parameter callback shouuld take uint8_t - number of event
*/
void drv_Button_InitCallback(ButtonContext_t *keyDeff, void* ptrCallback);



/**
 * @brief function for monitoring button state
 * @param ptrToMass - pointer to massive of button
 * @param massCount - count of created button
 */
void drv_Button_Run(ButtonContext_t *ptrToMass, uint8_t countElOfMass);


/**
*@brief  function for get button event
*@param  ptrButton - pointer to necessary button
*@return event of  button
*/
uint8_t  drv_Button_GetEvent(ButtonContext_t *ptrButton);

/**
*@brief  function for get button state
*@param  ptrButton - pointer to necessary button
*@return state of  button
*/
bool  drv_Button_IsButtonPressed(ButtonContext_t *ptrButton);



#endif /* DRV_BUTTON_ */
