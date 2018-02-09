/*
 * drv_UART.c
 *
 *  Created on: 2 лют. 2018 р.
 *      Author: MaGol
 */


#include "drv_UART.h"
#include "stm32f4xx_hal.h"
#include "ul_RingBuffer.h"


#define SIZE_OF_TX_BUFFER     50
#define SIZE_OF_RX_BUFFER     50

#define PACKAGE_START_BYTE    0x3A // :
#define PACKAGE_FINISH_BYTE   0x24 // $

typedef struct{
	UART_HandleTypeDef *config;
	uint8_t countPackages     : 7;
    uint8_t rx_packageStarted : 1;

	uint8_t buffer_RX[SIZE_OF_RX_BUFFER];
	uint8_t buffer_TX[SIZE_OF_TX_BUFFER];

	RingBuffer_t rx_RingBuffer;
	RingBuffer_t tx_RingBuffer;
}uartDescribe_t;


void addData(ExistUARTs_t uartName, uint8_t newByte);




static uartDescribe_t UARTs[UART_Amount]={
	// config   countPackages   rxStartPack   rxBuffer  txBuffer
		{0,         0,              0,          {0},     {0} },
		{0,         0,              0,          {0},     {0} },
		{0,         0,              0,          {0},     {0} },
		{0,         0,              0,          {0},     {0} }
};



void drv_UART_Init(UART_HandleTypeDef *ptrUartConfig){

	if (ptrUartConfig->Instance == USART1) {
		UARTs[UART1].config = ptrUartConfig;
		ulRingBuffer_Create(&UARTs[UART1].rx_RingBuffer, UARTs[UART1].buffer_RX,
				SIZE_OF_RX_BUFFER);
		ulRingBuffer_Create(&UARTs[UART1].tx_RingBuffer, UARTs[UART1].buffer_TX,
				SIZE_OF_TX_BUFFER);
	}
	if (ptrUartConfig->Instance == USART2) {
		UARTs[UART2].config = ptrUartConfig;
		ulRingBuffer_Create(&UARTs[UART2].rx_RingBuffer, UARTs[UART2].buffer_RX,
				SIZE_OF_RX_BUFFER);
		ulRingBuffer_Create(&UARTs[UART2].tx_RingBuffer, UARTs[UART2].buffer_TX,
				SIZE_OF_TX_BUFFER);
	}

#ifdef USART3

	if (ptrUartConfig->Instance == USART3) {
		UARTs[UART3].config = ptrUartConfig;
		ulRingBuffer_Create(&UARTs[UART3].rx_RingBuffer, UARTs[UART3].buffer_RX,
				SIZE_OF_RX_BUFFER);
		ulRingBuffer_Create(&UARTs[UART3].tx_RingBuffer, UARTs[UART3].buffer_TX,
				SIZE_OF_TX_BUFFER);
	}
#endif

#ifdef USART4
	if (ptrUartConfig->Instance == USART4) {
		UARTs[UART4].config = ptrUartConfig;
		ulRingBuffer_Create(&UARTs[UART4].rx_RingBuffer, UARTs[UART4].buffer_RX,
				SIZE_OF_RX_BUFFER);
		ulRingBuffer_Create(&UARTs[UART4].tx_RingBuffer, UARTs[UART4].buffer_TX,
				SIZE_OF_TX_BUFFER);
	}
#endif
}



void drv_UART_SendData(ExistUARTs_t uartName, uint8_t *ptrData, uint8_t sizeData) {
	if (UARTs[uartName].config != NULL && ptrData != NULL) {
		ulRingBuffer_CreatePackage(&UARTs[uartName].tx_RingBuffer);
		for (uint8_t i = 0; i < sizeData; i++) {
			ulRingBuffer_PushByte(&UARTs[uartName].tx_RingBuffer, ptrData[i]);
		}
		ulRingBuffer_FinalizePackage(&UARTs[uartName].tx_RingBuffer);
	}
}


void drv_UART_ReadData(ExistUARTs_t uartName, uint8_t *ptrRecievedData,	uint16_t *sizeData){
	if(uartName < UART_Amount && ptrRecievedData != NULL && sizeData != NULL && UARTs[uartName].countPackages > 0){
		ulRingBuffer_Pop(&UARTs[uartName].rx_RingBuffer, ptrRecievedData, sizeData);
	}
}


uint8_t drv_UART_IsRXcontainData(ExistUARTs_t uartName) {
	uint8_t answer = 0;
	if (uartName < UART_Amount) {
		answer = UARTs[uartName].countPackages;
	}
	return answer;
}






void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t writeByte;
	uint16_t size;

	if(huart->Instance == USART1){
       if(UARTs[UART1].tx_RingBuffer.fullness > 0){
    	   ulRingBuffer_Pop(&UARTs[UART1].tx_RingBuffer, &writeByte, &size);
       }
	}
	if(huart->Instance == USART2){
		if (UARTs[UART2].tx_RingBuffer.fullness > 0) {
			ulRingBuffer_Pop(&UARTs[UART2].tx_RingBuffer, &writeByte, &size);
		}
	}
#ifdef USART3
	if(huart->Instance == USART3) {
		if(UARTs[UART3].tx_RingBuffer.fullness > 0) {
			ulRingBuffer_Pop(&UARTs[UART3].tx_RingBuffer, &writeByte, &size);
		}
	}
#endif
#ifdef USART4
	if(huart->Instance == USART4) {
		if(UARTs[UART4].tx_RingBuffer.fullness > 0) {
			ulRingBuffer_Pop(&UARTs[UART4].tx_RingBuffer, &writeByte, &size);
		}
	}
#endif

	HAL_UART_Transmit(huart, &writeByte, &size, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint16_t size;
	uint8_t  newByte;

	HAL_UART_Receive(&huart, &newByte, size, 1);

	if (huart->Instance == USART1) {
		addData(UART1, newByte);
	}
	if (huart->Instance == USART2) {
		addData(UART2, newByte);
	}
#ifdef USART3
	if(huart->Instance == USART3) {
		addData(UART3, newByte);
	}
#endif
#ifdef USART4
	if(huart->Instance == USART4) {
		addData(UART4, newByte);
	}
#endif
}



void addData(ExistUARTs_t uartName, uint8_t newByte) {

	uint8_t packageStarted = UARTs[uartName].rx_packageStarted;
	if (newByte == PACKAGE_START_BYTE &&
	    !packageStarted) {
		ulRingBuffer_CreatePackage(&UARTs[uartName].buffer_RX);
		UARTs[uartName].rx_packageStarted = 1;
	} else if (newByte == PACKAGE_FINISH_BYTE && packageStarted) {
		ulRingBuffer_FinalizePackage(&UARTs[uartName].buffer_RX);
		UARTs[uartName].rx_packageStarted = 0;
	} else {
		ulRingBuffer_PushByte(&UARTs[uartName].buffer_RX, newByte);
	}
}

void addIdleEvent(ExistUARTs_t uartName) {
	ulRingBuffer_FinalizePackage(&UARTs[uartName].rx_RingBuffer);
	UARTs[uartName].rx_packageStarted = 0;
	UARTs[uartName].countPackages = UARTs[uartName].countPackages + 1;
}

void addErrorEvent(ExistUARTs_t uartName) {
	ulRingBuffer_RemovePackage(&UARTs[uartName].rx_RingBuffer);
}
