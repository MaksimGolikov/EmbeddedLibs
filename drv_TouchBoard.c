/*
 * Name         drv_TouchBoard.h
 * Author       Maksim Golikov (SW developer)
 * Created      26 JUL 2018
 * Description  driver for control touch board (source file)
 */


#include "DRV/drv_TouchBoard.h"



typedef struct {
  GPIO_TypeDef       *port;
  uint8_t            pin;
  uint8_t            detectLevel;
  TouchBoardState_t  sensorState;   
	
}TouchBoadrDescription_t;


static TouchBoadrDescription_t touchBoards[Touch_Amount] = {0};




int8_t drv_TouchBard_Init(touchBoard_t nameTouch, GPIO_TypeDef *port, uint8_t pin, uint8_t detectLevel){
    int8_t initState = -1;

    if(nameTouch < Touch_Amount){
        touchBoards[nameTouch].port        = port;
        touchBoards[nameTouch].pin         = pin;
        touchBoards[nameTouch].detectLevel = detectLevel;
        touchBoards[nameTouch].sensorState = TouchNotDetect;
        initState = 0;
    }

    return initState;
}


TouchBoardState_t drv_TouchBard_GetState(touchBoard_t nameTouch){
	TouchBoardState_t returnedValue = TouchNotDetect;
    
    if(nameTouch < Touch_Amount){
    	 returnedValue = touchBoards[nameTouch].sensorState;
    }

	return returnedValue;
}


void drv_TouchBard_Run(){
	for(uint8_t sensor = 0; sensor < Touch_Amount; sensor ++){		
        uint8_t pinState = HAL_GPIO_ReadPin(touchBoards[sensor].port, (GPIO_PIN_0 << touchBoards[sensor].pin) );
         if(pinState == touchBoards[sensor].detectLevel){
              touchBoards[sensor].sensorState = TouchDetect;
         }else{
              touchBoards[sensor].sensorState = TouchNotDetect;
         }
	}
}



