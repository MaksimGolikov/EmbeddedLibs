/*
 * drvUART.h
 *
 *  Created on: May 18, 2016
 *      Author: sak
 */

#ifndef DRVUART_H_
#define DRVUART_H_

#include <stdint.h>
#include <stddef.h>

typedef enum{
	Buffer_full,
	Buffer_empty
}State_UART_Buffer_t;


/**
 * USART Initialization.
 * This function should be called before using drvUART.
 * There are not any parameters. But this once will be added later.
 */
void drvUART_Init();

/**
 * Function to send data to USART
 * @param uint8_t *ptrData - pointer on data buffet which should be sent
 * @param uint16_t size - numbers of bytes which should be sent
 */
void drvUART_Send(uint8_t *ptrData, uint16_t size);

uint8_t drvUART_getDataToSend(uint8_t *newByte);

State_UART_Buffer_t drvUART_getSTATE_TX_BUFFER();


void drvUART_GetData(uint8_t *ptrData);


uint8_t drvUART_isDataReady(void);

void addData(uint8_t newByte);

void addIdleEvent(void);

void addErrorEvent(void);

#endif /* DRVUART_H_ */

