/*
 * Name         drv_Button.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (source file)
 */


#include "DRV/drv_Button.h"
#include "DRV/drv_SysClock.h"
#include "UL/ul_RingBuffer.h"

#include "Common/Database/Database_callbacks.h"


#define BUTTON_RESET_STATE          20     // count puls for digital filter
#define BUTTON_SHORT_PRESS_STATE    70     // count puls for digital filter

#define KEY_COUNTER_MAX             100    // max quantity of counter
#define KEY_COUNTER_MIN             5      // min quantity of counter

#define SIZE_BUTTON_BUFFER          10     // bits


#define MAX_COUNT_OF_BUTTON         5      // size of button buffer
#define TIME_TO_2S_PRESS            2000   // 2 sec     
#define TIME_TO_3S_PRESS            3000   // 3 sec  
#define TIME_TO_5S_PRESS            5000   // 5 sec  
#define TIME_TO_10S_PRESS           10000  // 10 sec  






typedef struct {
	GPIO_TypeDef     *port;
	uint16_t         pin;
	Button_Event_t   buttonEvent;
	Button_Event_t   previousButtonEvent;
	ButtonState_t    keyState;
	uint32_t         startTime;
	uint16_t         counter;
	ButtonWorkMode_t workMode;
	uint8_t          buffer[SIZE_BUTTON_BUFFER];
	RingBuffer_t     eventsBuffer;

} Key_struct_t;


void CountOfHighLevel(Key_struct_t *button);


static uint8_t countOfButton = 0;

static Key_struct_t Buttons[MAX_COUNT_OF_BUTTON]={
 //   port   pin     baseEvent            previuseButtonEvent    keyState    startTime     counter           workMode       buffer
     {GPIOA,  0,   BUTTON_RESET_EVENT,    BUTTON_SHORT_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
	 {GPIOA,  0,   BUTTON_RESET_EVENT,    BUTTON_SHORT_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA,  0,   BUTTON_RESET_EVENT,    BUTTON_SHORT_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA,  0,   BUTTON_RESET_EVENT,    BUTTON_SHORT_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA,  0,   BUTTON_RESET_EVENT,    BUTTON_SHORT_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
};






void drv_Button_Init(GPIO_TypeDef *port, uint8_t  pin, ButtonWorkMode_t mode){
		
	Buttons[countOfButton].port      = port;
	Buttons[countOfButton].pin       = GPIO_PIN_0 << pin;	
	Buttons[countOfButton].startTime = drv_SysClock_GetCurrentTime();
 	Buttons[countOfButton].workMode  = mode;
	
	ul_RingBuffer_Create( &Buttons[countOfButton].eventsBuffer, Buttons[countOfButton].buffer, SIZE_BUTTON_BUFFER);
	
	countOfButton += 1;
}



void drv_Button_Run(void){
	
	for(uint8_t i = 0; i < countOfButton; i++){
	
		 CountOfHighLevel(&Buttons[i]);			
		 uint8_t val = 0;
		
		 switch(Buttons[i].keyState){
			 case key_reset:


				 if(Buttons[i].buttonEvent > BUTTON_RESET_EVENT){
					 Buttons[i].buttonEvent = BUTTON_RESET_EVENT;
#ifdef COMMON_DATABASE_DATABASE_CALLBACKS_H_
					 Callback_ChangeButtonEvent(i, Buttons[i].buttonEvent);
#endif
				 }


				 if (Buttons[i].counter > BUTTON_SHORT_PRESS_STATE) {
				     Buttons[i].keyState = key_set;
				     Buttons[i].startTime = drv_SysClock_GetCurrentTime();

					 if (Buttons[i].workMode == ON_PRESS) {
						val = BUTTON_SHORT_EVENT;
						ul_RingBuffer_Push(&Buttons[i].eventsBuffer, &val, 1);
						Buttons[i].previousButtonEvent = Buttons[i].buttonEvent;
						Buttons[i].buttonEvent = BUTTON_SHORT_EVENT;
#ifdef COMMON_DATABASE_DATABASE_CALLBACKS_H_
						Callback_ChangeButtonEvent(i, Buttons[i].buttonEvent);
#endif
					 }
			     }
				 
				 break;
			 case key_set:						   
			 
		
			 if(Buttons[i].workMode == ON_PRESS){
				if (Buttons[i].counter < BUTTON_RESET_STATE) {
					Buttons[i].keyState = key_reset;
					Buttons[i].buttonEvent = BUTTON_RESET_EVENT;
				}
				if (drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_2S_PRESS) &&
					Buttons[i].keyState != key_reset &&
					Buttons[i].previousButtonEvent < BUTTON_2S_EVENT) {
					Buttons[i].buttonEvent = BUTTON_2S_EVENT;
				}
				if (drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_3S_PRESS) &&
					Buttons[i].keyState != key_reset &&
					Buttons[i].previousButtonEvent < BUTTON_3S_EVENT) {
					Buttons[i].buttonEvent = BUTTON_3S_EVENT;
				}
				if (drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_5S_PRESS) &&
					Buttons[i].keyState != key_reset &&
					Buttons[i].previousButtonEvent < BUTTON_5S_EVENT) {
					Buttons[i].buttonEvent = BUTTON_5S_EVENT;
				}
				if (drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_10S_PRESS) &&
					Buttons[i].keyState != key_reset &&
					Buttons[i].previousButtonEvent < BUTTON_10S_EVENT) {
					Buttons[i].buttonEvent = BUTTON_10S_EVENT;
				}



				if(Buttons[i].previousButtonEvent != Buttons[i].buttonEvent){
					Buttons[i].previousButtonEvent = Buttons[i].buttonEvent;
					ul_RingBuffer_Push(&Buttons[i].eventsBuffer, &Buttons[i].buttonEvent, 1);
#ifdef COMMON_DATABASE_DATABASE_CALLBACKS_H_
					Callback_ChangeButtonEvent(i, Buttons[i].buttonEvent);
#endif
				}


			}else{
				if (Buttons[i].counter < BUTTON_RESET_STATE) {
					Buttons[i].keyState = key_reset;

					uint32_t timeSpent = drv_SysClock_GetCurrentTime() - Buttons[i].startTime;

					if (timeSpent < TIME_TO_2S_PRESS) {
						val = BUTTON_SHORT_EVENT;
					} else if (timeSpent >= TIME_TO_2S_PRESS
							&& timeSpent < TIME_TO_3S_PRESS) {
						val = BUTTON_2S_EVENT;
					} else if (timeSpent >= TIME_TO_3S_PRESS
							&& timeSpent < TIME_TO_5S_PRESS) {
						val = BUTTON_3S_EVENT;
					} else if (timeSpent >= TIME_TO_5S_PRESS
							&& timeSpent < TIME_TO_10S_PRESS) {
						val = BUTTON_5S_EVENT;
					} else if (timeSpent >= TIME_TO_10S_PRESS) {
						val = BUTTON_10S_EVENT;
					}

					Buttons[i].previousButtonEvent = val;
					ul_RingBuffer_Push(&Buttons[i].eventsBuffer, &val, 1);
#ifdef COMMON_DATABASE_DATABASE_CALLBACKS_H_
					Callback_ChangeButtonEvent(i, Buttons[i].buttonEvent);
#endif
				}
			    
			 }
				  
				 break;
			 default:
				 Buttons[i].keyState = key_reset;
				 break;
	   }
    }
		 
 }



Button_Event_t drv_Button_GetEvent(buttons_t buttonNumber) {
	Button_Event_t buttonState = BUTTON_RESET_EVENT;
	uint16_t size = 0;

	if (Buttons[(uint8_t) buttonNumber].eventsBuffer.fullness != 0) {
		uint8_t readData = 0;
		ul_RingBuffer_Pop(&Buttons[(uint8_t) buttonNumber].eventsBuffer,
				&readData, &size);
		buttonState = (Button_Event_t) readData;
	} else {
		buttonState = BUTTON_RESET_EVENT;
	}

	return buttonState;
}


Button_Event_t  drv_Button_GetPreviousEvent(buttons_t buttonNumber){
	return Buttons[(uint8_t)buttonNumber].previousButtonEvent;
}


uint8_t  drv_Button_IsButtonPressed(buttons_t buttonNumber){
	return Buttons[(uint8_t)buttonNumber].keyState;
}


uint32_t drv_Button_GetTimeOfLongPress(buttons_t buttonNumber){ 
	uint32_t returnedTime = 0;
	if(Buttons[(uint8_t)buttonNumber].keyState){
		returnedTime = drv_SysClock_GetCurrentTime() - Buttons[(uint8_t)buttonNumber].startTime;
	} 
	return returnedTime; 
}



void CountOfHighLevel(Key_struct_t *button){	
	uint8_t pinState = HAL_GPIO_ReadPin(button->port, button->pin );
	
	if(pinState){
		button->counter ++;		
	}else{
		button->counter --;
	}		
 
	if(button->counter > KEY_COUNTER_MAX){
		button->counter = KEY_COUNTER_MAX;
	}
	if(button->counter < KEY_COUNTER_MIN){
		button->counter = KEY_COUNTER_MIN;
	}
	
}
