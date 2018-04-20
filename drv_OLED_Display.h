/*
 * Name        drv_OLED_Display.h
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control OLED display ssd1351 (header file)
 */

#ifndef DRV_OLED_DISPLAY_H_
#define DRV_OLED_DISPLAY_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"


enum{
COLOR_WHITE		  =	0xFFFF,
COLOR_BLACK		  =	0x0000,
COLOR_RED		    =	0xF800,
COLOR_PURP		  =	0xC815,
COLOR_GREEN		  =	0x07E0,
COLOR_BLUE		  =	0x04FF,
COLOR_YELLOW	  =	0xFFE0,
COLOR_SIEMENS  	=	0x06F7,
COLOR_MAGENTA  	=	0xf81f,
COLOR_LMAGENTA	=	0xfc1f,
COLOR_TURQUOISE	=	0x36b9,
COLOR_DGREEN	  =	0x7e00
};



/**
 * @Brief function to initialize display
 * @param spi   pointer to spi which should use
 * @param _portDI   port for DI
 * @param _portSCK  port for SCK
 * @param _portCS   port for CS
 * @param _portDC   port for DC
 * @param _portRES  port for RES
 * @param _pinDI    pin for DI
 * @param _pinSCK   pin for SCK
 * @param _pinCS    pin for CS
 * @param _pinDC    pin for DC
 * @param _pinRES   pin for RES
 */
void drv_OLED_Display_Init(SPI_HandleTypeDef *spi,
						               GPIO_TypeDef *_portCS, GPIO_TypeDef *_portDC, GPIO_TypeDef *_portRES,
                           uint8_t _pinCS, uint8_t _pinDC, uint8_t _pinRES);


/**
 * @brief function for clearing all screen
 */
void drv_OLED_Display_Clear(void);


/**
 * @brief Function to fill screen by selected color
 * @param color - color which will  use
 */
void drv_OLED_Display_FillScreen(uint16_t color);


/**
 * @brief function for fill selected area on screen
 * @param x_st   start x  position of rectangle
 * @param y_st   start y  position of rectangle
 * @param x_end  end x  position of rectangle
 * @param y_end  end y  position of rectangle
 * @param color  color which will use
 */
void drv_OLED_Display_FillRectangle(uint8_t x_st, uint8_t y_st,
		                                uint8_t x_end, uint8_t y_end,
								                    uint16_t color);


/**
 * @brief function to draw selected pixel on screen
 * @param x   x position of point
 * @param y   y position of point
 * @param color  color which will use
 */
void drv_OLED_Display_DrawPixel(uint8_t x, uint8_t y, uint16_t color);


/**
 * @brief function for drawing picture by one color
 * @param x_st    start x  position of picture
 * @param y_st    start y  position of picture
 * @param width   width of picture
 * @param height  height of picture
 * @param ptrPicture   pointer to mass of picture
 * @param color        color which will redraw
 * @param paintFon     value show what part should paint element or fon
 */
void drv_OLED_Display_DrawPictire(uint8_t x_st,  uint8_t y_st,
		                              uint8_t width, uint8_t heigh,
								                  uint8_t *ptrPicture, uint16_t color, uint8_t paintFon);


/**
 * @brief Function to drawing image
 * @param x_st           start x  position of image
 * @param y_st           start y  position of image
 * @param width          width of image
 * @param height         height  height of image
 * @param ptrPicture     pointer to mass of image
 */
void  drv_OLED_Display_DrawImage(uint8_t x_st,  uint8_t y_st,
                                 uint8_t width, uint8_t heigh,
		                             uint16_t *ptrPicture);



/**
 * @brief function to scale picture
 * @param ptrPicture  source picture
 * @param base_Width  width of source image
 * @param base_Height height of source image
 * @param scale       necessary value of scale
 * @param ptrScaledPicture pointer to mass which should contain scaled image
 */
void drv_OLED_Display_ScalePicture( uint8_t *ptrPicture, uint8_t base_Width, uint8_t base_Height,
		                                uint8_t scale, uint8_t *ptrScaledPicture);


#endif /* DRV_OLED_DISPLAY_H_ */
