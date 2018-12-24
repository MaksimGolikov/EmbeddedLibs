/*
 * Name         drv_Button.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (source file)
 */

#include "DRV/drv_Button.h"
#include "DRV/drv_SysClock.h"
#include "UL/ul_RingBuffer.h"


#define BUTTON_RESET_STATE          10     // count puls for digital filter
#define BUTTON_SHORT_PRESS_STATE    20     // count puls for digital filter

#define KEY_COUNTER_MAX             60    // max quantity of counter
#define KEY_COUNTER_MIN             5      // min quantity of counter

#define SIZE_BUTTON_BUFFER          10     // bits


const static uint16_t TIMEOUTS_OF_PRESS []={
		0, 1, 2000, 3000, 5000, 10000, 20000,30000
};

enum{
	key_reset,
	key_set
};

typedef struct{
	GPIO_TypeDef      *port;
	uint8_t            pin;

	Button_Event_t     buttonEvent;
	uint8_t            keyState;

	uint32_t           startTime;
    uint8_t            counter;
	ButtonWorkMode_t   workMode;

	uint8_t            buffer[SIZE_BUTTON_BUFFER];
	RingBuffer_t       eventsBuffer;

}Key_struct_t;


static Key_struct_t Buttons[buttons_Amount]={
	 // port   pin     baseEvent             keyState    startTime     counter           workMode       buffer
     {GPIOA, 0,   BUTTON_RESET_EVENT,       key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
	 {GPIOA, 0,   BUTTON_RESET_EVENT,       key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA, 0,   BUTTON_RESET_EVENT,       key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },

};


void CountOfHighLevel(Key_struct_t *button);



int8_t drv_Button_Init(buttons_t buttonName,GPIO_TypeDef *port, uint8_t  pin, ButtonWorkMode_t mode){
	int8_t status = -1;
	
	if(buttonName < buttons_Amount){
		Buttons[buttonName].port      = port;
		Buttons[buttonName].pin       = pin;
		Buttons[buttonName].startTime = drv_SysClock_GetCurrentTime();
		Buttons[buttonName].workMode  = mode;

		ul_RingBuffer_Create( &Buttons[buttonName].eventsBuffer, Buttons[buttonName].buffer, SIZE_BUTTON_BUFFER);
		status = 1;
	}
	return status;
}



void drv_Button_Run(void){
	
	for(uint8_t i = 0; i < buttons_Amount; i++){
		 CountOfHighLevel(&Buttons[i]);			
		 uint8_t val = 0;
		 switch(Buttons[i].keyState){
			 case key_reset:{
				 if( Buttons[i].counter > BUTTON_SHORT_PRESS_STATE ){
					 Buttons[i].keyState = key_set;					
					 Buttons[i].startTime = drv_SysClock_GetCurrentTime();
					 
					 if( Buttons[i].workMode == ON_PRESS){
						 val = BUTTON_SHORT_EVENT;
						 ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val, sizeof(val) );
						 Buttons[i].buttonEvent = BUTTON_SHORT_EVENT;
					 }					 
				 }				 
			 }break;
			 case key_set:{
				 if(Buttons[i].workMode == ON_PRESS){
					 if( Buttons[i].counter < BUTTON_RESET_STATE ){
						 Buttons[i].keyState = key_reset;
						 val = BUTTON_SHORT_EVENT;
						 ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val, sizeof(val) );
						 Buttons[i].buttonEvent = BUTTON_RESET_EVENT;
					 }
					 
					 for(uint8_t ev = BUTTON_2S_EVENT; ev < button_events_Amount; ev ++){
						 if( drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIMEOUTS_OF_PRESS[ev]) &&
							 Buttons[i].keyState != key_reset && Buttons[i].buttonEvent < ev){

							 val = ev;
							 ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val , sizeof(val));
							 Buttons[i].buttonEvent = ev;
						 }
					 }
				 }
				 if(Buttons[i].workMode == ON_RELEASE){
					 if( Buttons[i].counter < BUTTON_RESET_STATE ){
					   Buttons[i].keyState = key_reset;

					   uint32_t timeSpent = drv_SysClock_GetCurrentTime() - Buttons[i].startTime;

					   for(uint8_t ev = BUTTON_2S_EVENT; ev < button_events_Amount; ev ++){
						 if( timeSpent >= TIMEOUTS_OF_PRESS[ev] ){
							 val = ev;
							 ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val , sizeof(val));
							 Buttons[i].buttonEvent = ev;
						 }
					   }
					   ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val , sizeof(val));
					}
				 }
			 }break;
			 default:
				 Buttons[i].keyState = key_reset;
				 break;
			}				
   }
		 
 }



Button_Event_t  drv_Button_GetEvent(buttons_t buttonNumber){
	Button_Event_t buttonState  = BUTTON_RESET_EVENT;
	uint16_t       size = 0;
	
	if(buttonNumber < buttons_Amount){
		if(Buttons[(uint8_t)buttonNumber].eventsBuffer.fullness > 0){
			uint8_t readData = 0;
			ul_RingBuffer_Pop( &Buttons[(uint8_t)buttonNumber].eventsBuffer, &readData, &size);
			buttonState = (Button_Event_t)readData;
		}		
	}
	return buttonState;
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
	uint8_t pinState = HAL_GPIO_ReadPin(button->port, GPIO_PIN_0 << button->pin );
	
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
