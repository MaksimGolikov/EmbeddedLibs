/*
 * Name         drv_PWM.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control PWM (source file)
 */


#include "DRV/drv_PWM.h"

#define MAX_COUNT_OF_PWM         12      // size of button buffer

enum{
	PWM_deactive,
	PWM_active	
};

typedef struct{
	TIM_HandleTypeDef  *timer;
	uint32_t           channel;	
	uint8_t            pwmStateActiv;	
	uint8_t            oldState;
}PWM_struct_t;

static uint8_t countOfPWM = 0;


static PWM_struct_t PWMs[MAX_COUNT_OF_PWM]={
// ptrTimer   channel   currentState     oldState
   {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
   {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive},
   {0,        0,       PWM_deactive,    PWM_deactive},
	 {0,        0,       PWM_deactive,    PWM_deactive}	 
};



void drv_PWM_Init(TIM_HandleTypeDef *timer, uint8_t newChannel){	
	
	PWMs[countOfPWM].timer = timer;
	
	if(newChannel <= 4){
	 PWMs[countOfPWM].channel = 4 * (newChannel -1);
  }else if (newChannel == 5){
	 PWMs[countOfPWM].channel = 0x00000018U;
  }else{
		PWMs[countOfPWM].channel = 0;
	}	
	
	PWMs[countOfPWM].pwmStateActiv =  PWM_deactive;
	countOfPWM +=1;	
}


void drv_PWM_SetPuls(uint8_t numPWM, uint16_t intensity){
	
	uint16_t newValue = PWMs[numPWM].timer->Init.Period;
	if( intensity < PWMs[numPWM].timer->Init.Period ){
	  newValue  = intensity;
	}
		switch(PWMs[numPWM].channel){
			case 0:
				PWMs[numPWM].timer->Instance->CCR1 = newValue;
				break;
			case 4:
				PWMs[numPWM].timer->Instance->CCR2 = newValue;
				break;
			case 8:
				PWMs[numPWM].timer->Instance->CCR3 = newValue;
				break;
			case 12:
				PWMs[numPWM].timer->Instance->CCR4 = newValue;
				break;
			default:
				PWMs[numPWM].timer->Instance->CCR1 = newValue;
				PWMs[numPWM].timer->Instance->CCR2 = newValue;
				PWMs[numPWM].timer->Instance->CCR3 = newValue;
				PWMs[numPWM].timer->Instance->CCR4 = newValue;
				break;
		}
}


uint16_t drv_PWM_GetPuls(uint8_t numPWM){
	uint16_t result = 0;	
	if(numPWM < countOfPWM){
		result = PWMs[numPWM].timer->Init.Period;
	}
	return result;
}


void drv_PWM_Start(uint8_t numPWM){
	PWMs[numPWM].pwmStateActiv = PWM_active;
}


void drv_PWM_Stop(uint8_t numPWM){
	PWMs[numPWM].pwmStateActiv = PWM_deactive;
}


void drv_PWM_Run(){
	
	for(uint8_t i = 0; i < countOfPWM; i++){
		if(PWMs[i].oldState != PWMs[i].pwmStateActiv){
		  if(PWMs[i].pwmStateActiv == PWM_active){
				HAL_TIM_PWM_Start(PWMs[i].timer, PWMs[i].channel);	
			}else{
				HAL_TIM_PWM_Stop(PWMs[i].timer,PWMs[i].channel);
			}
			PWMs[i].oldState = PWMs[i].pwmStateActiv;
		}		
	}	
}


uint8_t drv_PWM_GetCurrentQuontityOfPWM(){
	return countOfPWM;
}
