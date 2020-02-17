/*
 * Name        ul_RingBuffer.c
 * Author      Maksim Golikov (SW developer)
 * Created on: Apr 12, 2016
 * Description utility for store data (source file)
 */


#include <stdlib.h>
#include <string.h>

#include "ul_RingBuffer.h"



/**
 * @brief structure to describe all parameters of the ring buffer
 */
typedef struct RingBuffer_t{
	uint8_t size;             /*!< Total size of the buffer in a bytes*/
	uint8_t fullness;         /*!< Size of buffer which are using at the moment in a bytes*/
	uint8_t* ptrBuffer;       /*!< Pointer to the buffer which should use as ring buffer.
	                               Type of the buffer should be uint8_t.
	                               Buffer should be static */
	uint8_t indexWrite;      /*!< Index to the cell of the buffer which should use to start writing*/
	uint8_t indexRead;       /*!< Index to the cell of the buffer which should use to start reading*/

	                         /* Next cells use to work with package*/
	uint8_t tmpReceivedSize; /*!< Index of the cell which should be use for store the size of the package*/
	uint8_t tmpStartIndex;   /*!< Index of the cell which should be use as start of the package`s data*/
}RingBuffer_t;



static uint8_t indexIncrement(uint8_t currentIndex,uint8_t sizeData, uint8_t maxValue);


RingBuffer_Messages_t ul_RingBuffer_Create(RingBuffer_t *ptrRingBuffer, uint8_t* ptrBuffer, uint8_t size) {
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

	if ((size != 0)&&(ptrBuffer != NULL)) {
		ptrRingBuffer->size = size;
		ptrRingBuffer->ptrBuffer = ptrBuffer;
		ptrRingBuffer->fullness = 0;
		ptrRingBuffer->indexRead = 0;
		ptrRingBuffer->indexWrite = 0;
		returnValue = RB_ERR_STATUS_OK;
	} else {
		returnValue = RB_ERR_STATUS_ERROR_INIT;
	}

	return returnValue;
}



RingBuffer_Messages_t ul_RingBuffer_Push(RingBuffer_t *ptrRingBuffer,uint8_t* newData){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;
    uint8_t dataSize = strlen( (char*)newData);

	if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else if ( ptrRingBuffer->fullness + dataSize >= ptrRingBuffer->size) {
			returnValue = RB_ERR_STATUS_BUFF_IS_FULL;
		} else {

			ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite] = dataSize;
			ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite,dataSize,ptrRingBuffer->size);

			if( (dataSize+ptrRingBuffer->indexWrite) < ptrRingBuffer->size){
				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]),newData,dataSize);
				ptrRingBuffer->indexWrite=indexIncrement(ptrRingBuffer->indexWrite,dataSize,ptrRingBuffer->size);
			}
			else{
				uint8_t FirstPart;
				uint8_t SecondPart;

				FirstPart=ptrRingBuffer->size-ptrRingBuffer->indexWrite;
				SecondPart=dataSize-FirstPart;

				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]),newData,FirstPart);
				ptrRingBuffer->indexWrite=indexIncrement(ptrRingBuffer->indexWrite,FirstPart,ptrRingBuffer->size);
				memcpy(&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite]),&(newData[FirstPart]),SecondPart);
				ptrRingBuffer->indexWrite=indexIncrement(ptrRingBuffer->indexWrite,SecondPart,ptrRingBuffer->size);
			}
			ptrRingBuffer->fullness  += (dataSize + 1);
			returnValue = RB_ERR_STATUS_OK;

		}
 return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_Pop(RingBuffer_t *ptrRingBuffer, uint8_t* readData, uint16_t* sizeData){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else if ( ptrRingBuffer->fullness == 0) {
			returnValue = RB_ERR_STATUS_BUFF_IS_EMPTY;
		} else {

			uint8_t DataSize = ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead];
			ptrRingBuffer->indexRead=indexIncrement(ptrRingBuffer->indexRead,1,ptrRingBuffer->size);

		   if( (ptrRingBuffer->indexRead+DataSize)<=ptrRingBuffer->size){

			   memcpy(readData, &(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead]), DataSize);
			   ptrRingBuffer->indexRead=indexIncrement(ptrRingBuffer->indexRead,DataSize,ptrRingBuffer->size);

		   }else{

			uint8_t FirstPart;
			uint8_t SecondPart;

			FirstPart=ptrRingBuffer->size-ptrRingBuffer->indexRead;
			SecondPart=DataSize-FirstPart;

			memcpy(readData,&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead]),FirstPart);
			ptrRingBuffer->indexRead=indexIncrement(ptrRingBuffer->indexRead,FirstPart,ptrRingBuffer->size);
			memcpy(&(readData[FirstPart]),&(ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexRead]),SecondPart);
			ptrRingBuffer->indexRead=indexIncrement(ptrRingBuffer->indexRead,SecondPart,ptrRingBuffer->size);

		   }

			ptrRingBuffer->fullness -= ( DataSize +1) ;
			(*sizeData)=DataSize;

			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}



RingBuffer_Messages_t ul_RingBuffer_CreatePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else if ( ptrRingBuffer->fullness >= ptrRingBuffer->size) {
			returnValue = RB_ERR_STATUS_BUFF_IS_FULL;
		} else {
			ptrRingBuffer->tmpReceivedSize = 0;
			ptrRingBuffer->tmpStartIndex = ptrRingBuffer->indexWrite;
			ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite,1,ptrRingBuffer->size);
			ptrRingBuffer->fullness = ptrRingBuffer->fullness + 1;
			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_PushByte(RingBuffer_t *ptrRingBuffer, uint8_t newByte){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else if ( ptrRingBuffer->fullness >= ptrRingBuffer->size) {
			returnValue = RB_ERR_STATUS_BUFF_IS_FULL;
		} else {
			ptrRingBuffer->ptrBuffer[ptrRingBuffer->indexWrite] = newByte;
			ptrRingBuffer->indexWrite = indexIncrement(ptrRingBuffer->indexWrite,1,ptrRingBuffer->size);
			ptrRingBuffer->fullness = ptrRingBuffer->fullness + 1;
			ptrRingBuffer->tmpReceivedSize = ptrRingBuffer->tmpReceivedSize + 1;
			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_FinalizePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else {
			ptrRingBuffer->ptrBuffer[ptrRingBuffer->tmpStartIndex] = ptrRingBuffer->tmpReceivedSize;
			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}

RingBuffer_Messages_t ul_RingBuffer_RemovePackage(RingBuffer_t *ptrRingBuffer){
	RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else {
			ptrRingBuffer->indexWrite = ptrRingBuffer->tmpStartIndex;
			ptrRingBuffer->fullness = ptrRingBuffer->fullness - ptrRingBuffer->tmpReceivedSize - 1;
			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}


RingBuffer_Messages_t ul_RingBuffer_Clear(RingBuffer_t *ptrRingBuffer){
    RingBuffer_Messages_t returnValue = RB_ERR_STATUS_ERROR_INIT;

		if ((ptrRingBuffer->size == 0) && (ptrRingBuffer->ptrBuffer == NULL)) {
			returnValue = RB_ERR_STATUS_ERROR_INIT;
		} else {
			ptrRingBuffer->indexRead  = 0;
		  ptrRingBuffer->indexWrite = 0;
			ptrRingBuffer->fullness   = 0;
			returnValue = RB_ERR_STATUS_OK;
		}

		return returnValue;
}


static uint8_t indexIncrement(uint8_t currentIndex,uint8_t sizeData, uint8_t maxValue) {
	uint8_t returnValue = currentIndex;

	if(currentIndex+sizeData<=(maxValue-1))
		returnValue+=sizeData ;
	else
		returnValue=0;

	return returnValue;
}


