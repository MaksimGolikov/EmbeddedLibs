/*
 * DisplayUtility.h
 *
 *  Created on: Feb 28, 2019
 *      Author: MGO
 */

#ifndef DISPLAYUTILITY_H_
#define DISPLAYUTILITY_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <DisplayDescribe.h>


enum{
	COLOR_WHITE		  =	0xFFFF,
	COLOR_BLACK		  =	0x0000,
	COLOR_RED		  =	0xF800,
	COLOR_DRED        = 0xE000,
	COLOR_GREEN		  =	0x07E0,
	COLOR_DGREEN	  =	0x7e00,
	COLOR_BLUE		  =	0x04FF,
	COLOR_YELLOW	  =	0xFFE0,
	COLOR_DGRAY       = 0x94B2,
	COLOR_GRAY        = 0xB596,
	COLOR_ORANGE      = 0xFB20,
	COLOR_LBLUE       = 0x64DF,
	COLOR_GOLD        = 0xFE46,
	COLOR_SILVER      = 0x94B2,
	COLOR_BRONZE      = 0x9B83,
	COLOR_VIOLET      = 0x9212,
};



void DisplayUtility_Init(SPI_HandleTypeDef *spi, GPIO_TypeDef *PortCS,  uint8_t pinCS,
												 GPIO_TypeDef *PortRES, uint8_t pinRES,
												 GPIO_TypeDef *PortDC,  uint8_t pinDC
												 );


void DisplayUtility_ChangeData(uint8_t elType, uint8_t elName, uint16_t value);

void DisplayUtility_SetScreen(uint8_t screen);



void DisplayUtility_Run(void);




int8_t DisplayUtility_Config_StatusBar(uint8_t name,
		                             uint8_t verticalOrientation,
		                             uint8_t x_pos, uint8_t y_pos,
									 uint8_t width, uint8_t height,
									 uint16_t min_value, uint16_t max_value,
									 uint16_t colorBorder, uint16_t colorFilling);

int8_t DisplayUtility_Config_Picture(uint8_t name,
                                     uint8_t x_pos, uint8_t y_pos,
		                             uint8_t width, uint8_t height,
		                             uint8_t *ptrToPicture,
		                             uint16_t color);


int8_t DisplayUtility_Config_Animation(uint8_t name,
									   uint8_t x_pos, uint8_t y_pos,
									   uint8_t width, uint8_t height,
									   uint8_t *ptrToPicture,
									   uint16_t color, uint16_t sizeImage,
									   uint8_t numberOfImages, uint32_t timeoutChanges);

int8_t DisplayUtility_Config_Label(uint8_t name, uint8_t isItTtext,
								   uint8_t x_pos, uint8_t y_pos,
								   uint8_t scale,
								   const char *ptrToPicture, uint8_t length,
								   uint16_t color);
#endif /* DISPLAYUTILITY_H_ */
