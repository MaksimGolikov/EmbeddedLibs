/*
 * Name         drv_TouchBoard.h
 * Author       Maksim Golikov (SW developer)
 * Created      26 JUL 2018
 * Description  driver for control touch board (header file)
 */

#ifndef DRV_TOUCHBOARD_H
#define DRV_TOUCHBOARD_H

#include <stdint.h>


typedef enum{
	TouchDetect,
	TouchNotDetect
}TouchBoardState_t;


typedef enum{
	TOUCH_2HAND,
	Touch_Amount
}touchBoard_t;


/**
*@brief Function for initialize touch board
*@param nameTouch - name of initialize touch board
*@param port - name of nesessary port
*@param pin  - number of pin
*@param detectLevel - state of pin if touch detected
*@return initialize status 0- done, else error
*/
int8_t drv_TouchBard_Init(touchBoard_t nameTouch, GPIO_TypeDef *port, uint8_t pin, uint8_t detectLevel);


/**
*@brief  Function for get state of touch board
*@param  nameTouch - name of nesessary touch board
*@return state of  touch board
*/
TouchBoardState_t drv_TouchBard_GetState(touchBoard_t nameTouch);


/**
*@brief Function for parsing touch board state. Should be call pereodicaly
*/
void drv_TouchBard_Run(void);






 #endif