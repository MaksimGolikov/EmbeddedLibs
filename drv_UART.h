/*
 * drv_UART.h
 *  Created on: 2 лют. 2018 р.
 *      Author: MaGol
 */

#ifndef DRV_UART_H_
#define DRV_UART_H_

#include "stdint.h"
#include <stddef.h>
//#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal.h"

typedef enum{
	UART1,
	UART2,
	UART3,
	UART4,
	UART_Amount
}ExistUARTs_t;


/**
 * @brief function which mast be called before using UART
 * @param ptrUartConfig - pointer to UART configuration
 */
void drv_UART_Init(UART_HandleTypeDef *ptrUartConfig);


/**
 * @brief function for save data to TX buffer
 * @param uartName name of necessary uart
 * @param ptrData - pointer to necessary data
 * @pram dataSize - size of sending data
 */
void drv_UART_SendData(ExistUARTs_t uartName, uint8_t *ptrData, uint8_t dataSize);

/**
 * @brief function for take data from buffer if they exist
 * @param uartName name of necessary uart
 * @param prtRecievedData - pointer to mass for save read data
 * @param sizeData - size of read data
 */
void drv_UART_ReadData(ExistUARTs_t uartName, uint8_t *ptrRecievedData,uint16_t *sizeData);


/**
 *
 * @param uartName - name of necessary uart
 * @return count of exist data package
 */
uint8_t drv_UART_IsRXcontainData(ExistUARTs_t uartName);




#endif /* DRV_UART_H_ */
