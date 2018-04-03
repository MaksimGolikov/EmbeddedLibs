/*
 * Name        drv_Led.c
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control led  (source file)
 */


#ifndef DRV_DRV_LED_C_
#define DRV_DRV_LED_C_


#include "DRV/drv_LED.h"
#include "DRV/drv_SysClock.h"


 typedef struct{
	 GPIO_TypeDef*   port;
     uint8_t         pin;

     Led_WorkMode_t  workMode;
     Led_WorkMode_t  state;
     uint8_t         highLevel : 1;
     uint8_t         lowLevel  : 1;
	 uint32_t        blinkPeriod;
	 uint32_t        startBlinkTime;
 }LedDescription_t;


void TurnOn_Led(NameLeds_t led);
void TurnOff_Led(NameLeds_t led);



static uint8_t lastLedNumber = 0;


 static LedDescription_t Leds[led_Amount] = {
//   port    pin    workMode    state     highLevel   lowLewel  blinkPeriod   startBlinkTime
	{ 0,      0,    mode_OFF,  mode_OFF,     0,          0,       1000,             0}
 };



 void drv_Led_Init(GPIO_TypeDef* port, uint8_t pin, uint8_t highLevel, uint8_t lowLevel){

	if(lastLedNumber < led_Amount){
		Leds[lastLedNumber].port = port;
		Leds[lastLedNumber].pin  = GPIO_PIN_0 << pin;
		Leds[lastLedNumber].highLevel = highLevel;
		Leds[lastLedNumber].lowLevel =  lowLevel;
		Leds[lastLedNumber].startBlinkTime = drv_SysClock_GetCurrentTime();
		lastLedNumber += 1;
	}
}


 void drv_Led_SetMode(NameLeds_t led, Led_WorkMode_t newMode){
	if(led < led_Amount){
		Leds[led].workMode = newMode;

		switch(Leds[led].workMode){
			default:
			case mode_OFF:
				TurnOff_Led(led);
				break;
			case mode_ON:
				TurnOn_Led(led);
				break;
		}
	}
}


 Led_WorkMode_t drv_Led_GetMode(NameLeds_t led){
	Led_WorkMode_t answer = mode_OFF;
	if(led < led_Amount){
		answer = Leds[led].workMode;
	}
	return answer;
}


 void drv_Led_SetBlinkPeriod(uint32_t period){
	if(period > 0){
		for(uint8_t led = 0; led < led_Amount; led++){
			 Leds[led].blinkPeriod = period;
		}
	}
}


uint32_t drv_Led_GetBlinkPeriod(void){
	return Leds[0].blinkPeriod;
}



void drv_Led_Run(void) {

	for (uint8_t led = 0; led < led_Amount; led++) {

		if (Leds[led].workMode == mode_BLINK) {
			if (drv_SysClock_IsTimeSpent(Leds[led].startBlinkTime,	Leds[led].blinkPeriod)) {
				if (Leds[led].state == mode_ON) {
					Leds[led].state = mode_OFF;
					TurnOn_Led(led);
				} else {
					Leds[led].state = mode_ON;
					TurnOff_Led(led);
				}
				Leds[led].startBlinkTime = drv_SysClock_GetCurrentTime();
			}
		}
	}

}


void TurnOn_Led(NameLeds_t led){
     HAL_GPIO_WritePin(Leds[led].port, Leds[led].pin, Leds[led].highLevel);
}


void TurnOff_Led(NameLeds_t led){
	HAL_GPIO_WritePin(Leds[led].port, Leds[led].pin, Leds[led].lowLevel);
}

#endif /* DRV_DRV_LED_C_ */
