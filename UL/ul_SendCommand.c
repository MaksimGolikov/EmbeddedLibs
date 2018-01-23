/*
 * Name         ul_SendCommand.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  utility for send command by IR led (source file)
 */



#include "UL/ul_SendCommand.h"
#include "DRV/drv_Timer.h"
#include "DRV/drv_PWM.h"


#define PAUSE_PULS_TIME                            6  // count puls dev 100 
#define CAPTION_PULS_TIME                          24 // count puls dev 100 
#define LOW_PULS_TIME                              6  // count puls dev 100 
#define HIGH_PULS_TIME                             12 // count puls dev 100 
#define COMMAND_LENGTH                             23 // count of bits

#define COUNT_OF_IR                                5


uint8_t PWM_turnON(uint8_t numPWM, uint16_t time); /* return 1 if pwm stopped else 0*/
uint8_t PWM_pause(uint8_t numPWM, uint16_t time); /*return 1 if time spent else 0*/



typedef struct{
	uint32_t   startTime;
	uint32_t   commandForSend;
    uint8_t    sendCommand_state;
	int8_t     bitShift;
	uint8_t    needPause;
	
	uint8_t    PWMmode;	
}IR_transmitter_t;


static IR_transmitter_t IRmass[]={
// startTime  commandForSend    sendComandState    bitShift   needPause     PWMmode
      {0,            0,            0,                 0,              0,        0}, // 1 PWM
      {0,            0,            0,                 0,              0,        0}, // 2 PWM
      {0,            0,            0,                 0,              0,        0}, // 3 PWM
      {0,            0,            0,                 0,              0,        0}, // 4 PWM
      {0,            0,            0,                 0,              0,        0}  // 5 PWM
};

static uint8_t countOfusedPWM = 0;




void ul_SendCommand_Init(){
  	
	countOfusedPWM = drv_PWM_GetCurrentQuontityOfPWM();
	
	for(uint8_t i = 0; i < countOfusedPWM; i++){
		 IRmass[i].startTime = drv_Timer_getCurrentTime();
	}
}

void ul_SendCommand_Send(uint8_t numIR, uint32_t command){
	
	assert_param(numIR > 0);
	
	IRmass[numIR - 1].commandForSend = command;
	IRmass[numIR - 1].sendCommand_state = 0;
	IRmass[numIR - 1].bitShift = COMMAND_LENGTH;
	IRmass[numIR - 1].needPause = 0;
}


void ul_SendCommand_Run(void){
	
	for(uint8_t i = 0; i < countOfusedPWM; i++){
			switch(IRmass[i].sendCommand_state){
			case 0:
				IRmass[i].sendCommand_state	+= PWM_turnON(i + 1, CAPTION_PULS_TIME);
				break;
			case 1:
				IRmass[i].sendCommand_state +=  PWM_pause(i + 1 , PAUSE_PULS_TIME);
				break;
			case 2:
					switch(IRmass[i].needPause){
						case 0:
							if( (0x01 << IRmass[i].bitShift) & IRmass[i].commandForSend){
								IRmass[i].needPause = PWM_turnON(i + 1, HIGH_PULS_TIME);
								IRmass[i].bitShift -= IRmass[i].needPause;
							 }else{
								IRmass[i].needPause = PWM_turnON(i + 1, LOW_PULS_TIME);
								IRmass[i].bitShift -= IRmass[i].needPause;
							 }
							break;
						case 1:
							IRmass[i].needPause -= PWM_pause(i + 1, PAUSE_PULS_TIME);
							break;
						default:
							break;
					}


				if(IRmass[i].bitShift < 0){
					IRmass[i].sendCommand_state += 1;
					IRmass[i].commandForSend = 0;
				}
			break;
			default:
				drv_PWM_Stop(i + 1);
				break;
		}
	}
	drv_PWM_Run();
}





uint8_t PWM_turnON(uint8_t numPWM, uint16_t time){
   uint8_t stopped = 0;	
		
	  drv_PWM_Start(numPWM);
	if(PWM_pause(numPWM, time)){
		drv_PWM_Stop(numPWM);	
		stopped = 1;			
	}	
	
	return stopped;
}


uint8_t PWM_pause(uint8_t numIR, uint16_t time){		
	uint8_t isPauseFinish = 0;
	
	switch(IRmass[numIR].PWMmode){
		case 0:
			IRmass[numIR].startTime = drv_Timer_getCurrentTime();
		  IRmass[numIR].PWMmode =1;
		break;
		case 1:
			if( drv_Timer_isTimeSpent(IRmass[numIR].startTime, time) ){
				isPauseFinish = 1;
				IRmass[numIR].PWMmode = 0;
			}
			break;
		default:
			IRmass[numIR].PWMmode = 0;
		break;
	}
	return isPauseFinish;
}
