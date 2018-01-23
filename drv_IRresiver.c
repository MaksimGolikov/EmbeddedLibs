/*
 * Name         drv_IRresiver.c
 * Author       Maksim Golikov (SW developer)
 * Created      15 DEC 2017
 * Description  driver for control IR resiver (source file)
 */

#include "DRV/drv_IRresiver.h"
#include "DRV/drv_Timer.h"
#include "UL/ul_RingBuffer.h"


#define HIGH_LEVEL_PULS_QUONTITY_MIN    10 // count puls in mcs
#define HIGH_LEVEL_PULS_QUONTITY_MAX    14 // count puls in mcs
#define LOW_LEVEL_PULS_QUONTITY_MIN      4 // count puls in mcs
#define LOW_LEVEL_PULS_QUONTITY_MAX      8 // count puls in mcs
#define CAPTION_PULS_QUONTITY_MIN       20 // count puls in mcs
#define CAPTION_PULS_QUONTITY_MAX       26 // count puls in mcs

#define LENGTH_OF_COMMAND           23 // bit

typedef struct{
	GPIO_TypeDef * port;
	uint8_t        pin;

	uint32_t       startTime;
	uint32_t       readData;
	uint32_t       returnCommand;
	int8_t         writeBitNumber;
}IR_description_t;


static IR_description_t IRmass[] = {
	//    ptrPort     pin       startTime      returnCommand       writeBit
	{     GPIOA,       0,           0,              0,                 0     },
	{     GPIOA,       0,           0,              0,                 0     },
	{     GPIOA,       0,           0,              0,                 0     },
	{     GPIOA,       0,           0,              0,                 0     },
	{     GPIOA,       0,           0,              0,                 0     },
};

static uint8_t countOfUsedIR = 0;



void drv_IRresiver_Init(GPIO_TypeDef *port, uint8_t pin){
    	IRmass[countOfUsedIR].port           = port;
		IRmass[countOfUsedIR].pin            = GPIO_PIN_0 << pin;
		IRmass[countOfUsedIR].startTime      = drv_Timer_getCurrentTime();
		IRmass[countOfUsedIR].returnCommand  = 0;
		IRmass[countOfUsedIR].readData       = 0;
		IRmass[countOfUsedIR].writeBitNumber = LENGTH_OF_COMMAND;

		countOfUsedIR ++;
}


uint32_t drv_IRresiver_GetLastState(IRSensors_t nameIR){
	
	return IRmass[nameIR].returnCommand;
}


void drv_IRresiver_Read(IRSensors_t nameIR){
		
		GPIO_PinState currentPinState = HAL_GPIO_ReadPin(IRmass[nameIR].port, IRmass[nameIR].pin);

		if (currentPinState == GPIO_PIN_SET) {
			uint32_t periodOfTime = drv_Timer_getCurrentTime();
			periodOfTime -= IRmass[nameIR].startTime;

			if (periodOfTime > CAPTION_PULS_QUONTITY_MIN
					&& periodOfTime < CAPTION_PULS_QUONTITY_MAX) {
				IRmass[nameIR].readData = 0;
				IRmass[nameIR].writeBitNumber = LENGTH_OF_COMMAND;
			}
			if (periodOfTime > HIGH_LEVEL_PULS_QUONTITY_MIN
					&& periodOfTime < HIGH_LEVEL_PULS_QUONTITY_MAX) {
				IRmass[nameIR].readData |= (0x01 << IRmass[nameIR].writeBitNumber);
				IRmass[nameIR].writeBitNumber -= 1;
			}
			if (periodOfTime > LOW_LEVEL_PULS_QUONTITY_MIN
					&& periodOfTime < LOW_LEVEL_PULS_QUONTITY_MAX) {
				IRmass[nameIR].writeBitNumber -= 1;
			}
		}
		IRmass[nameIR].startTime = drv_Timer_getCurrentTime();

		if (IRmass[nameIR].writeBitNumber < 0) {
			IRmass[nameIR].returnCommand = IRmass[nameIR].readData;
		}

}
