/*
 * Name         ul_DisplayConterol.c
 * Author       Maksim Golikov (SW developer)
 * Created      13 FEB 2018
 * Description  utility for work with display (source file)
 */
 
 
 #include "UL/ul_DisplayControl.h"
 #include "UL/ul_RingBuffer.h"
 #include "string.h"
 
 
 #define SIZE_BUFFER       100
 #define SIZE_PACKAGE      30
 
 
 
static UART_HandleTypeDef *uart;
static uint8_t countPackage;



uint8_t buffer_display[SIZE_BUFFER];
RingBuffer_t displayBuffer;
 
 
 
 
 
void ul_DisplayControl_Init(UART_HandleTypeDef *ptrUART){
	
	if(ptrUART != NULL ){
		uart = ptrUART;
		countPackage = 0;
		memset(buffer_display, 0, SIZE_BUFFER);
		ul_RingBuffer_Create(&displayBuffer, buffer_display, SIZE_BUFFER);		
	}	
}




void ul_DisplayControl_SendData(uint8_t *buffer, uint16_t size){
	if(buffer != NULL && size){
		 if( (displayBuffer.fullness + size) < displayBuffer.size){
			 ul_RingBuffer_CreatePackage(&displayBuffer);
			 for(uint8_t byte = 0; byte < size; byte ++){
				 ul_RingBuffer_PushByte(&displayBuffer, buffer[byte]);
			 }
			 ul_RingBuffer_FinalizePackage(&displayBuffer);
			 countPackage += 1;
		 }
	}
}


void ul_DisplayControl_Run(void){
	if(countPackage > 0){
		uint8_t send[SIZE_PACKAGE] = {0};
		uint16_t size;
		
		ul_RingBuffer_Pop(&displayBuffer, send, &size);
		countPackage -= 1;
		
		HAL_UART_Transmit(uart, send, size, 10);		
	}
}
