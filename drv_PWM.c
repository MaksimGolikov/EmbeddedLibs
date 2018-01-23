/*
 * Name         drv_PWM.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  driver for control PWM (source file)
 */


#include "DRV/drv_PWM.h"

#define MAX_COUNT_OF_PWM         5      // size of button buffer

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
	 // *timer   channel   currentState     oldState
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


void drv_PWM_Start(uint8_t numPWM){
	PWMs[numPWM -1].pwmStateActiv = PWM_active;
}



void drv_PWM_Stop(uint8_t numPWM){
	PWMs[numPWM -1].pwmStateActiv = PWM_deactive;
}



void drv_PWM_Run(){
	
	for(uint8_t i = 1; i <= countOfPWM; i++){
		if(PWMs[i - 1].oldState != PWMs[i - 1].pwmStateActiv){
			if(PWMs[i - 1].pwmStateActiv == PWM_active){
				HAL_TIM_PWM_Start(PWMs[i - 1].timer,PWMs[i - 1].channel);	
			}else{
				HAL_TIM_PWM_Stop(PWMs[i - 1].timer,PWMs[i- 1].channel);
			}
			PWMs[i - 1].oldState = PWMs[i - 1].pwmStateActiv;
		}		
	}	
}

uint8_t drv_PWM_GetCurrentQuontityOfPWM(){
	return countOfPWM;
}
