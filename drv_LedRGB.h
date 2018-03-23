/*
 * Name         drv_LedRGB.h
 * Author       Maksim Golikov (SW developer)
 * Created      19 MARCH 2018
 * Description  driver for control RGB led (header file)
 */

#ifndef DRV_LED_RGB_H_
#define DRV_LED_RGB_H_


#include "stm32f1xx_hal.h"
#include <stdint.h>



typedef enum{
	rgb_INDICATE,
	rgb_Amount	
}rgbLeds_t;


typedef enum{
  mode_OFF,
	mode_ON,
  mode_BLINK ,
  mode_BLINK_SLIDE	
}rgbLed_WorkMode_t;

 typedef enum{
	 color_RED,	
	 color_BLUE,
	 color_YELLOW,
	 color_GREEN,
	 color_ORANGE,
	 color_WHITE,
	 color_Amount
 }PossibleColors_t;
 
/**
*@brief function for should be cal first
*@param timer_Red    pointer to timer which should control red color
*@param channel_Red  number of channel at timer_Red
*@param timer_Green    pointer to timer which should control green color
*@param channel_Green  number of channel at timer_Green
*@param timer_Blue    pointer to timer which should control blue color
*@param channel_Blue  number of channel at timer_Blue
*/
void drv_LedRGB_Init(TIM_HandleTypeDef* timer_Red, uint8_t channel_Red,
                     TIM_HandleTypeDef* timer_Green, uint8_t channel_Green,
					           TIM_HandleTypeDef* timer_Blue,  uint8_t channel_Blue);


/**
*@brief function for set work mode of led
*@param name of necessary led
*@param newMode new mode of led
*/				 
void drv_LedRGB_SetMode(rgbLeds_t led, rgbLed_WorkMode_t newMode);


/**
*@brief function for get work mode of led
*@param name of necessary led
*@param newMode new mode of led
*@return current mode
*/				 
rgbLed_WorkMode_t drv_LedRGB_GetMode(rgbLeds_t led);


/**
*@brief function for set blink period of led
*@param period new blink period
*/				 
void drv_LedRGB_SetBlinkPeriod(uint32_t period);


/**
*@brief function for get blink period of led
*@return period of blink
*/				 
uint32_t drv_LedRGB_GetBlinkPeriod(void);

			 
/**
*@brief function for set color of rgb led
*@param name of necessary led
*@param redPart - part of red color in complex color 
*@param greenPart - part of green color in complex color 
*@param bluePart - part of blue color in complex color 
*/				 
void  drv_LedRGB_SetColor(rgbLeds_t led, uint16_t  redPart, uint16_t greenPart, uint16_t bluePart);


/**
*@brief function for set color of rgb led
*@param name of necessary led
*@param color - necessary color 
*/				 
void  drv_LedRGB_SetDefineColor(rgbLeds_t led, uint8_t  color);


/**
*@brief function for get color of rgb led
*@param name of necessary led
*@param redPart - pointer to value which should contain part of red color in complex color 
*@param greenPart - pointer to value which should contain part of green color in complex color 
*@param bluePart - pointer to value which should contain part of blue color in complex color 
*/	
void drv_LedRGB_GetColor(rgbLeds_t led, uint16_t  *redPart, uint16_t *greenPart, uint16_t *bluePart);


/**
*@brief function for control led state
*/
void drv_LedRGB_Run(void);




#endif
