/*
 * Name        ul_RingBuffer.c
 * Author      Maksim Golikov (SW developer)
 * Created on: Apr 12, 2016
 * Description utility for store data (source file)
 */


#include <stdlib.h>
#include <string.h>

#include "ring_buff/ul_RingBuffer.h"

static uint8_t indexIncrement(uint8_t currentIndex,uint8_t sizeData, uint8_t maxValue);


RingBuffer_Messages_t ul_RingBuffer_Create(RingBuffer_t *ptrRingBuffer, uint8_t* ptrBuffer, uint8_t size) {
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

	if ((size != 0)&&(ptrBuffer != NULL)) {
		ptrRingBuffer->size           = size;
		ptrRingBuffer->ptrBuffer      = ptrBuffer;
		ptrRingBuffer->fullness       = 0;
		ptrRingBuffer->indexRead      = 0;
		ptrRingBuffer->indexWrite     = 0;
		ptrRingBuffer->packageAmounts = 0;
		returnValue = ringbuff_ok;
	} else {
		returnValue = ringbuff_initializationError;
	}

	return returnValue;
}



RingBuffer_Messages_t ul_RingBuffer_Push(RingBuffer_t *ptrRingBuffer,void* newData, uint8_t size){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;
    uint8_t dataSize = size;

	if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else if ( ptrRingBuffer->fullness + dataSize >= ptrRingBuffer->size) {
			returnValue = ringbuff_bufferIsFull;
		} else {

			ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite] = dataSize;
			ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite, 1, ptrRingBuffer->size);
			ptrRingBuffer->packageAmounts++;
			if( (dataSize + ptrRingBuffer->indexWrite) < ptrRingBuffer->size){
				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]), newData, dataSize);
				ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite, dataSize, ptrRingBuffer->size);
			}
			else{
				uint8_t FirstPart;
				uint8_t SecondPart;

				FirstPart  = ptrRingBuffer->size - ptrRingBuffer->indexWrite;
				SecondPart = dataSize - FirstPart;

				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]), newData, FirstPart);
				ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite, FirstPart, ptrRingBuffer->size);

				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]), &newData[FirstPart], SecondPart);
				ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite, SecondPart, ptrRingBuffer->size);
			}
			ptrRingBuffer->fullness  += (dataSize + 1);
			returnValue = ringbuff_ok;

		}
 return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_Pop(RingBuffer_t *ptrRingBuffer, void* readData, uint8_t* sizeData){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else if ( ptrRingBuffer->fullness == 0) {
			returnValue = ringbuff_bufferIsEmpty;
		} else {

			ptrRingBuffer->packageAmounts--;
			uint8_t DataSize = ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead];
			ptrRingBuffer->indexRead = indexIncrement(ptrRingBuffer->indexRead, 1, ptrRingBuffer->size);

		   if( (ptrRingBuffer->indexRead+DataSize) <= ptrRingBuffer->size){

			   memcpy(readData, &ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead], DataSize);
			   ptrRingBuffer->indexRead = indexIncrement(ptrRingBuffer->indexRead, DataSize, ptrRingBuffer->size);

		   }else{

			uint8_t FirstPart  = ptrRingBuffer->size-ptrRingBuffer->indexRead;
			uint8_t SecondPart = DataSize - FirstPart;

			memcpy(readData, &ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead], FirstPart);
			ptrRingBuffer->indexRead = indexIncrement(ptrRingBuffer->indexRead, FirstPart, ptrRingBuffer->size);

			memcpy(&readData[FirstPart], &ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead], SecondPart);
			ptrRingBuffer->indexRead = indexIncrement(ptrRingBuffer->indexRead, SecondPart, ptrRingBuffer->size);

		   }

			ptrRingBuffer->fullness -= ( DataSize + 1) ;
			(*sizeData) = DataSize;

			returnValue = ringbuff_ok;
		}

		return returnValue;
}



RingBuffer_Messages_t ul_RingBuffer_CreatePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else if ( ptrRingBuffer->fullness >= ptrRingBuffer->size) {
			returnValue = ringbuff_bufferIsFull;
		} else {
			ptrRingBuffer->tmpReceivedSize = 0;
			ptrRingBuffer->tmpStartIndex   = ptrRingBuffer->indexWrite;
			ptrRingBuffer->indexWrite      = indexIncrement(ptrRingBuffer->indexWrite, 1, ptrRingBuffer->size);
			ptrRingBuffer->fullness        = ptrRingBuffer->fullness + 1;
			returnValue = ringbuff_ok;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_PushByte(RingBuffer_t *ptrRingBuffer, uint8_t newByte){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else if ( ptrRingBuffer->fullness >= ptrRingBuffer->size) {
			returnValue = ringbuff_bufferIsFull;
		} else {
			ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite] = newByte;
			ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite, 1, ptrRingBuffer->size);
			ptrRingBuffer->fullness   = ptrRingBuffer->fullness + 1;
			ptrRingBuffer->tmpReceivedSize = ptrRingBuffer->tmpReceivedSize + 1;
			returnValue = ringbuff_ok;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_FinalizePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else {
			ptrRingBuffer->packageAmounts++;
			ptrRingBuffer->ptrBuffer[ptrRingBuffer->tmpStartIndex] = ptrRingBuffer->tmpReceivedSize;
			returnValue = ringbuff_ok;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_RemovePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else {
			ptrRingBuffer->indexWrite = ptrRingBuffer->tmpStartIndex;
			ptrRingBuffer->fullness = ptrRingBuffer->fullness - ptrRingBuffer->tmpReceivedSize - 1;
			ptrRingBuffer->packageAmounts--;
			returnValue = ringbuff_ok;
		}

		return returnValue;
}


RingBuffer_Messages_t ul_RingBuffer_Clear(RingBuffer_t *ptrRingBuffer){
    RingBuffer_Messages_t returnValue = ringbuff_initializationError;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = ringbuff_initializationError;
		} else {
			ptrRingBuffer->indexRead  = 0;
		    ptrRingBuffer->indexWrite = 0;
			ptrRingBuffer->fullness   = 0;
			ptrRingBuffer->packageAmounts = 0;
			returnValue = ringbuff_ok;
		}

		return returnValue;
}


static uint8_t indexIncrement(uint8_t currentIndex,uint8_t sizeData, uint8_t maxValue) {
	uint8_t returnValue = currentIndex;

	if(currentIndex + sizeData <= (maxValue - 1))
		returnValue += sizeData;
	else
		returnValue = 0;

	return returnValue;
}

