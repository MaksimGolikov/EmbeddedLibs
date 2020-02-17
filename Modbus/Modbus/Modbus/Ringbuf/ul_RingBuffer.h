/**
 * @Name        ul_RingBuffer.h
 * @Author      Maksim Golikov (SW developer)
 * @Created on: Apr 12, 2016
 * @Description utility for store data (header file)
 */

#include <stdint.h>
#include <stddef.h>

typedef struct RingBuffer_t *ringbuf;

/**
 * @brief List of status of the ring buffer
 */
typedef enum {
	RB_ERR_STATUS_OK,
	RB_ERR_STATUS_BUFF_IS_FULL,
	RB_ERR_STATUS_BUFF_IS_EMPTY,
	RB_ERR_STATUS_ERROR_INIT
}RingBuffer_Messages_t;


/**
 * @brief function for create ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param ptrBuffer     - pointer to buffer what will be use
 * @param size          - size of buffer what will be use
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Create(ringbuf ptrRingBuffer, uint8_t* ptrBuffer, uint8_t size);




/* Function Push and Pop should use to sate and get one byte.*/
/**
 * @brief function for save one byte to buffer
 * @param ptrRingBuffer - pointer to necessary riing buffer
 * @param newData       - pointer to necessary data
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Push(ringbuf ptrRingBuffer,uint8_t* newData);
/**
 * @brief function for get data from ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param readData      - pointer to byte what will be use for save read data
 * @param sizeData      - pointer to value what will be contain size of read data
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Pop(ringbuf ptrRingBuffer,uint8_t* readData, uint16_t* sizeData);





/*Function Create package, Push byte, Finalize package use for save to ring buffer data if size data not know at the start*/
/**
 * @brief function for create package in ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_CreatePackage(ringbuf ptrRingBuffer);
/**
 *
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param newByte       - what should to save in ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_PushByte(ringbuf ptrRingBuffer, uint8_t newByte);
/**
 * @brief function for finish package in ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_FinalizePackage(ringbuf ptrRingBuffer);
/**
 * @brief function for remove last package from ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_RemovePackage(ringbuf ptrRingBuffer);

/**
 * @brief function for clear ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer 
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Clear(ringbuf ptrRingBuffer);
