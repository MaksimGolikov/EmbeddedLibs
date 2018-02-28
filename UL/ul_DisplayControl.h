/*
 * Name         ul_DisplayCntrol.h
 * Author       Maksim Golikov (SW developer)
 * Created      13 FEB 2018
 * Description  utility for work with display (header file)
 */
 
 
#ifndef UL_DISPLAY_CONTROL_H_
#define UL_DISPLAY_CONTROL_H_

#include "stm32f1xx_hal.h"


/**
*@brief function wich should call before start communicate 
*@param ptrUART pointer to uart what connect with display
*/
void ul_DisplayControl_Init(UART_HandleTypeDef *ptrUART);


/**
*@brief function for send data to display
*@param buffer - pointer to buffer with data
*@param size   - size of buffer data
*/
void ul_DisplayControl_SendData(uint8_t *buffer, uint16_t size);

/**
*@brief function for communicate with display
*/
void ul_DisplayControl_Run(void);



#endif