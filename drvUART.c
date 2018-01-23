/*
 * drvUART.c
 *
 *  Created on: May 18, 2016
 *      Author: sak
 */

#include <string.h>

#include "drvUART.h"
#include "stm32f4xx.h"
#include "UL/ulRingBuffer.h"



#define drvUART_TX_BUFFER_SIZE	100
#define drvUART_RX_BUFFER_SIZE	100

static uint16_t drvUART_TX_Buffer_size;
static uint16_t drvUART_TX_Counter;
static uint8_t drvUART_TX_Buffer[drvUART_TX_BUFFER_SIZE];


static uint8_t drvUART_isPackageCreated = 0;
static uint8_t drvUART_CountPackagesInBuffer = 0;

uint8_t drvUART_RX_Buffer[drvUART_RX_BUFFER_SIZE];
RingBuffer_t drvUART_RX_RingBuffer;

static State_UART_Buffer_t TX_buffer_Empty_Flag;



static void drvUART_TxIrqDisable(void);
static void drvUART_TxIrqEnable(void);

static void drvUART_RxIrqDisable(void);
static void drvUART_RxIrqEnable(void);




void drvUART_Init(){


	// UART2 Clock Enable
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		// UART2 GPIO configuration
		GPIO_InitTypeDef GPIO;

		GPIO.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOA, &GPIO);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);


		// UART2 configuration
		USART_InitTypeDef UART;

		UART.USART_BaudRate = 115200;
		UART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		UART.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		UART.USART_Parity = USART_Parity_No;
		UART.USART_StopBits = USART_StopBits_2;
		UART.USART_WordLength = USART_WordLength_8b;

		USART_Init(USART2, &UART);
		USART_Cmd(USART2, ENABLE);

		// UART IRQ initialization
		NVIC_ClearPendingIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, 7);
		NVIC_EnableIRQ(USART2_IRQn);

		ulRingBuffer_Create(&drvUART_RX_RingBuffer, drvUART_RX_Buffer, drvUART_RX_BUFFER_SIZE);
}


void drvUART_Send(uint8_t *ptrData, uint16_t size){


	if (size < drvUART_TX_BUFFER_SIZE){

		memcpy(drvUART_TX_Buffer, ptrData, size);
		drvUART_TX_Buffer_size = size;
		drvUART_TX_Counter = 0;

		USART_SendData(USART2, drvUART_TX_Buffer[drvUART_TX_Counter]);
		drvUART_TX_Counter++;
		if (size > 1){
			drvUART_TxIrqEnable();
		}
	}
	TX_buffer_Empty_Flag=Buffer_full;

}

State_UART_Buffer_t drvUART_getSTATE_TX_BUFFER(){
	return TX_buffer_Empty_Flag;
}

uint8_t drvUART_getDataToSend(uint8_t *newByte){
	uint8_t returnValue = 0;

	if (drvUART_TX_Counter < drvUART_TX_Buffer_size) {
		*newByte = drvUART_TX_Buffer[drvUART_TX_Counter];
		drvUART_TX_Counter++;
		returnValue = 1;
	}
	else{
		TX_buffer_Empty_Flag=Buffer_empty;
		drvUART_TxIrqDisable();
		returnValue = 0;
	}

	return returnValue;
}



uint8_t drvUART_isDataReady(void){
	uint8_t returnValue = 0;
	if ( drvUART_CountPackagesInBuffer > 0 ) {
		returnValue = 1;
	}

	return returnValue;
}

void drvUART_GetData(uint8_t *ptrData){
	if(drvUART_CountPackagesInBuffer > 0) {
		uint8_t SizeData;
		ulRingBuffer_Pop(&drvUART_RX_RingBuffer, ptrData,SizeData);
		drvUART_CountPackagesInBuffer = drvUART_CountPackagesInBuffer - 1;
	}
}


void addData(uint8_t newByte) {

	if(drvUART_isPackageCreated == 0) {
		ulRingBuffer_CreatePackage(&drvUART_RX_RingBuffer);
		drvUART_isPackageCreated = 1;
	}

	ulRingBuffer_PushByte(&drvUART_RX_RingBuffer, newByte);
}

void addIdleEvent(void) {
	ulRingBuffer_FinalizePackage(&drvUART_RX_RingBuffer);
	drvUART_isPackageCreated = 0;
	drvUART_CountPackagesInBuffer = drvUART_CountPackagesInBuffer + 1;
}

void addErrorEvent(void) {
	ulRingBuffer_RemovePackage(&drvUART_RX_RingBuffer);
}



void drvUART_TxIrqEnable(void){
	USART2->CR1 |= USART_CR1_TXEIE;
}
void drvUART_TxIrqDisable(void){
	USART2->CR1 &= ~USART_CR1_TXEIE;
}

void drvUART_RxIrqEnable(void){
	USART2->CR1 |= USART_CR1_IDLEIE ;
}
void drvUART_RxIrqDisable(void){
	USART2->CR1 &= ~USART_CR1_IDLEIE ;
}






void USART2_IRQHandler(void)
{
	volatile uint32_t USART_SR_Register;
	uint8_t shouldWeSend = 0;
	uint8_t newByte;

	USART_SR_Register = USART2->SR;
	if (USART_SR_Register & USART_SR_TXE) {
		shouldWeSend = drvUART_getDataToSend(&newByte);
		if (shouldWeSend == 1){
			USART_SendData(USART2, newByte);
		}
		else
		    TX_buffer_Empty_Flag=Buffer_empty;
	}

	    if (USART_SR_Register & USART_SR_RXNE) {

			newByte = USART_ReceiveData(USART2);
			addData(newByte);
			drvUART_RxIrqDisable();
		}
	    if (USART_SR_Register & USART_SR_IDLE) {
	    	//drvUART_RxIrqDisable();
			addIdleEvent();
			drvUART_RxIrqEnable();
		}
	    /*if ((USART_SR_Register & USART_SR_NE)||(USART_SR_Register & USART_SR_PE)||(USART_SR_Register & USART_SR_FE)||(USART_SR_Register & USART_SR_ORE)) {
	    	//drvUART_RxIrqDisable();
	    	addErrorEvent();
	    	drvUART_RxIrqEnable();
		}*/

}
