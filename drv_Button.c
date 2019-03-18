/*
 * Name         drv_Button.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control button (source file)
 */

#include "DRV/drv_Button.h"
#include "CommonFunctions.h"


#define KEY_COUNTER_MAX             60  
#define KEY_COUNTER_MIN             5


static uint16_t *TIMEOUTS_OF_PRESS   = 0;
static uint8_t  button_events_Amount = 0;


void CountOfHighLevel(ButtonContext_t *button);


int8_t drv_Button_Init(ButtonContext_t *keyDeff, ButtonWorkMode_t mode, 
                       void *handlerPin, void *handlerTimer, uint16_t *TimeMass,
					   uint8_t numberOfEvents, uint8_t jitterDelay){

	int8_t statusOperation = -1;

	if(TIMEOUTS_OF_PRESS == 0){
	   TIMEOUTS_OF_PRESS    = TimeMass;
		 button_events_Amount = numberOfEvents;
	}	
	
	if(handlerPin != 0 && handlerTimer != 0){
		keyDeff->PinStateReader     = handlerPin;
		keyDeff->GetTime            = handlerTimer;
		keyDeff->workMode           = mode;
		keyDeff->buttonEvent        = 0;
		keyDeff->keyState           = key_reset;
		keyDeff->startTime          = keyDeff->GetTime();
		keyDeff->jitter_status_pin  = 0;
		keyDeff->jitter_time_delay  = 0;
		keyDeff->jitter_timeout     = jitterDelay;	
		statusOperation             = 1;
		keyDeff->Callback           = 0;
	}
	return statusOperation;
}

void drv_Button_InitCallback(ButtonContext_t *keyDeff, void* ptrCallback){
	if(keyDeff != 0 && ptrCallback != 0){
         keyDeff->Callback = ptrCallback;
	}
}

void drv_Button_Run(ButtonContext_t *ptrToMass, uint8_t countElOfMass){

	 for(uint8_t but = 0; but < countElOfMass; but ++){
		 CountOfHighLevel( &ptrToMass[but] );

		switch(ptrToMass[but].keyState){
			 case key_reset:{
				 if( ptrToMass[but].startTime > 0 && IsTimeSpent(ptrToMass[but].startTime, TIMEOUTS_OF_PRESS[1])){
					 ptrToMass[but].keyState  = key_set;				

					 if( ptrToMass[but].workMode == ON_PRESS){
						 ptrToMass[but].buttonEvent = 1;
						 if(ptrToMass[but].Callback != 0){
						 	ptrToMass[but].Callback(ptrToMass[but].buttonEvent);
						 }
					 }
				 }
			 }break;
			 case key_set:{
				 if(ptrToMass[but].workMode == ON_PRESS){
					 if( ptrToMass[but].startTime == 0 ){
						 ptrToMass[but].keyState    = key_reset;
						 ptrToMass[but].buttonEvent = 0;
						 if(ptrToMass[but].Callback != 0){
						 	ptrToMass[but].Callback(ptrToMass[but].buttonEvent);
						 }
					 }
					 for(uint8_t ev = 2; ev < button_events_Amount; ev ++){
						 if( IsTimeSpent(ptrToMass[but].startTime, TIMEOUTS_OF_PRESS[ev]) &&
								 ptrToMass[but].keyState != key_reset && ptrToMass[but].buttonEvent < ev){

							ptrToMass[but].buttonEvent = ev;
							if(ptrToMass[but].Callback != 0){
						 	    ptrToMass[but].Callback(ptrToMass[but].buttonEvent);
						    }
						 }
					 }
				 }
				 if(ptrToMass[but].workMode == ON_RELEASE){
					 if( ptrToMass[but].startTime == 0){
						 ptrToMass[but].keyState = key_reset;

					   uint32_t timeSpent = ptrToMass[but].GetTime() - ptrToMass[but].startTime;
					   for(uint8_t ev = 1; ev < button_events_Amount; ev ++){
							 if( timeSpent >= TIMEOUTS_OF_PRESS[ev] ){
								 ptrToMass[but].buttonEvent = ev;
								if(ptrToMass[but].Callback != 0){
						 	       ptrToMass[but].Callback(ptrToMass[but].buttonEvent);
						        }
							 }
					   }
					 }
				 }
			 }break;
			 default:
				 ptrToMass[but].keyState = key_reset;
				 break;
			}
	 }
}


uint8_t  drv_Button_GetEvent(ButtonContext_t *ptrButton){
	uint8_t event = 0;
	if(ptrButton != 0){
		event = ptrButton->buttonEvent;
	}

	return event;
}


bool  drv_Button_IsButtonPressed(ButtonContext_t *ptrButton){
	bool buttonState = 0;
	if(ptrButton != 0){
		buttonState = (uint8_t)ptrButton->keyState;
	}

	return buttonState;
}



void CountOfHighLevel(ButtonContext_t *button){

	if(button != 0){
		uint8_t pinState = button->PinStateReader();    
		
		if(pinState != button->jitter_status_pin)
		{
			button->jitter_time_delay = button->GetTime();
			button->jitter_status_pin = pinState;			
		}
		if(IsTimeSpent( button->jitter_time_delay, button->jitter_timeout)){
		   if(pinState && button->startTime == 0){
			   button->startTime = button->GetTime();
			 }
			 if(!pinState){
			    button->startTime = 0;
			 }
		}
	}
	
}
