/*
 * Name        drv_Led.h
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control led  (header file)
 */


#ifndef DRV_DRV_LED_H_
#define DRV_DRV_LED_H_


#include "stdint.h"
#include "stm32f1xx_hal.h"


typedef enum{
	led_HEARTBIT,
	led_Amount
}NameLeds_t;


typedef enum{
  mode_OFF,
  mode_ON,
  mode_BLINK
}Led_WorkMode_t;





/**
*@brief function should be call first
*@param port - name of led port
*@param pin - name of led pin
*@param *@param highLevel  value for turn ON led
*@param lowLevel   value for turn OFF led
*/
void drv_Led_Init(GPIO_TypeDef* port, uint8_t pin, uint8_t highLevel, uint8_t lowLevel);


/**
*@brief function for set work mode of led
*@param led        name of necessary led
*@param newMode    new mode of led
*/
void drv_Led_SetMode(NameLeds_t led, Led_WorkMode_t newMode);


/**
*@brief function for get work mode of led
*@param name of necessary led
*@return current mode
*/
Led_WorkMode_t drv_Led_GetMode(NameLeds_t led);


/**
*@brief function for set blink period of led
*@param period new blink period
*/
void drv_Led_SetBlinkPeriod(uint32_t period);


/**
*@brief function for get blink period of led
*@return period of blink
*/
uint32_t drv_Led_GetBlinkPeriod(void);


/**
*@brief function for control led state
*/
void drv_Led_Run(void);






#endif /* DRV_DRV_LED_H_ */
