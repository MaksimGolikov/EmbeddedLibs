/*
 * Name         drv_Button.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (source file)
 */


#include "drv_Button.h"
#include "drv_SysClock.h"
#include "ul_RingBuffer.h"

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





enum{
	key_reset,
	key_set
};

typedef struct{
	GPIO_TypeDef      *port;
	uint16_t           pin;
	Button_Event_t     buttonEvent;	
	uint8_t            keyState;
	uint32_t           startTime;
  uint16_t           counter;
	ButtonWorkMode_t   workMode;
	uint8_t            buffer[SIZE_BUTTON_BUFFER];
	RingBuffer_t       eventsBuffer;

}Key_struct_t;

static uint8_t countOfButton = 0;

static Key_struct_t Buttons[MAX_COUNT_OF_BUTTON]={
	 // port   pin     baseEvent           keyState    startTime     counter           workMode       buffer
     {GPIOA, 0,   BUTTON_RESET_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
	   {GPIOA, 0,   BUTTON_RESET_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA, 0,   BUTTON_RESET_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA, 0,   BUTTON_RESET_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
     {GPIOA, 0,   BUTTON_RESET_EVENT,    key_reset,     0,      KEY_COUNTER_MIN,     ON_PRESS,       {0} },
};


void CountOfHighLevel(Key_struct_t *button);



void drv_Button_Init(GPIO_TypeDef *port, uint8_t  pin, ButtonWorkMode_t mode){
		
	Buttons[countOfButton].port      = port;
	Buttons[countOfButton].pin       = GPIO_PIN_0 << pin;	
	Buttons[countOfButton].startTime = HAL_GetTick();
 	Buttons[countOfButton].workMode  = mode;
	
	ul_RingBuffer_Create( &Buttons[countOfButton].eventsBuffer, Buttons[countOfButton].buffer, SIZE_BUTTON_BUFFER);
	
	countOfButton ++;
}



void drv_Button_Run(void){
	
	for(uint8_t i = 0; i < countOfButton; i++){
	
		 CountOfHighLevel(&Buttons[i]);			
		
		
		 switch(Buttons[i].keyState){
			 case key_reset:	

			 
				 if( Buttons[i].counter > BUTTON_SHORT_PRESS_STATE ){
					 Buttons[i].keyState = key_set;					
					 Buttons[i].startTime = HAL_GetTick();
					 
					 if( Buttons[i].workMode == ON_PRESS){
						 Buttons[i].buttonEvent = BUTTON_SHORT_EVENT;
					 }					 
				 }
				 
				 
				 break;
			 case key_set:						   
			 
			 if(Buttons[i].workMode == ON_PRESS){
				 if( Buttons[i].counter < BUTTON_RESET_STATE ){
						 Buttons[i].keyState = key_reset;
						 Buttons[i].buttonEvent = BUTTON_RESET_EVENT;
					}			 
			    if( drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_2S_PRESS) && Buttons[i].keyState != key_reset && Buttons[i].buttonEvent != BUTTON_2S_EVENT){
						 Buttons[i].buttonEvent = BUTTON_2S_EVENT;
					}		
          if( drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_3S_PRESS) && Buttons[i].keyState != key_reset && Buttons[i].buttonEvent != BUTTON_3S_EVENT){
						 Buttons[i].buttonEvent = BUTTON_3S_EVENT;
					}		
					if( drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_5S_PRESS) && Buttons[i].keyState != key_reset && Buttons[i].buttonEvent != BUTTON_5S_EVENT){
						 Buttons[i].buttonEvent = BUTTON_5S_EVENT;
					}		
					if( drv_SysClock_IsTimeSpent(Buttons[i].startTime, TIME_TO_10S_PRESS) && Buttons[i].keyState != key_reset && Buttons[i].buttonEvent != BUTTON_10S_EVENT){
						 Buttons[i].buttonEvent = BUTTON_10S_EVENT;
					}							
			 }else{
				 if( Buttons[i].counter < BUTTON_RESET_STATE ){
						 Buttons[i].keyState = key_reset;
						
					   uint32_t timeSpent = HAL_GetTick() - Buttons[i].startTime;
					   uint8_t val = 0;
					 
					   if(timeSpent < TIME_TO_2S_PRESS){							 
							 val = BUTTON_SHORT_EVENT;							 
						 }
					   else if(timeSpent >= TIME_TO_2S_PRESS && timeSpent < TIME_TO_3S_PRESS){
							 val = BUTTON_2S_EVENT;							
						 }
					   else if(timeSpent >= TIME_TO_3S_PRESS && timeSpent < TIME_TO_5S_PRESS){
							 val = BUTTON_3S_EVENT;							
						 }
						 else if(timeSpent >= TIME_TO_5S_PRESS && timeSpent < TIME_TO_10S_PRESS){
							 val = BUTTON_5S_EVENT;						
						 }
						 else if(timeSpent >= TIME_TO_10S_PRESS){
							  val = BUTTON_10S_EVENT;						
						 }		

             ul_RingBuffer_Push( &Buttons[i].eventsBuffer, &val );	             
 					}			 
			    
			 }		 
				  
				 break;
			 default:
				 Buttons[i].keyState = key_reset;
				 break;
			}				
   }
		 
 }



Button_Event_t  drv_Button_GetEvent(buttons_t buttonNumber){
	Button_Event_t buttonState  = BUTTON_RESET_EVENT;
	uint16_t       size = 0;
	
	ButtonWorkMode_t mode = Buttons[(uint8_t)buttonNumber].workMode;
	
	if(mode == ON_PRESS){
		buttonState = Buttons[(uint8_t)buttonNumber].buttonEvent;
	}
	if(mode == ON_RELEASE){
		if(Buttons[(uint8_t)buttonNumber].eventsBuffer.fullness > 1){
			
			uint8_t readData = 0;
			ul_RingBuffer_Pop( &Buttons[(uint8_t)buttonNumber].eventsBuffer, &readData, &size);
			buttonState = (Button_Event_t)readData;
		}
		else{
			buttonState = BUTTON_RESET_EVENT;
		}
	}
		
	return buttonState;
}


uint8_t  drv_Button_IsButtonPressed(buttons_t buttonNumber){
	return Buttons[(uint8_t)buttonNumber].keyState;
}


uint32_t drv_Button_GetTimeOfLongPress(buttons_t buttonNumber){   
	uint32_t pressedTime = HAL_GetTick() - Buttons[(uint8_t)buttonNumber].startTime;
	return pressedTime; 
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
