#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t size;             /*!< Total size of the buffer in bytes*/
    uint8_t fullness;         /*!< Number of bytes which are being used */
    uint8_t* ptrBuffer;       /*!< Pointer to the pice of memory which is going to be used for storing*/
    uint8_t indexWrite;       /*!< Pointer to next cell to write*/
    uint8_t indexRead;        /*!< Pointer to next cell to read*/
    uint8_t tmpReceivedSize;  /*!< Nuber of the received bytes. This field is beig used for culculating size of package*/
    uint8_t tmpStartIndex;    /*!< Pointer to the cell for write size of the package*/
    uint8_t packageAmounts;   /*!< Amount of the sored packages */
}RingBuffer_t;

typedef enum {
    ringbuff_ok,
    ringbuff_bufferIsFull,
    ringbuff_bufferIsEmpty,
    ringbuff_initializationError
}RingBuffer_Messages_t;


/**
 * @brief function to create ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param ptrBuffer     - pointer to buffer what will be use
 * @param size          - size of buffer what will be use
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Create(RingBuffer_t *ptrRingBuffer, uint8_t* ptrBuffer, uint8_t size);




/* Function Push and Pop should use to sate and get one byte.*/
/**
 * @brief function for save one byte to buffer
 * @param ptrRingBuffer - pointer to necessary riing buffer
 * @param newData       - pointer to necessary data
 * @param size          - size of the pushing data
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Push(RingBuffer_t *ptrRingBuffer, void* newData, uint8_t size);

/**
 * @brief function for get data from ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param readData      - pointer to byte what will be use for save read data
 * @param sizeData      - pointer to value what will be contain size of read data
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Pop(RingBuffer_t *ptrRingBuffer, void* readData, uint8_t* sizeData);





/*Function Create package, Push byte, Finalize package use for save to ring buffer data if size data not know at the start*/
/**
 * @brief function for create package in ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_CreatePackage(RingBuffer_t *ptrRingBuffer);
/**
 *
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @param newByte       - what should to save in ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_PushByte(RingBuffer_t *ptrRingBuffer, uint8_t newByte);
/**
 * @brief function for finish package in ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_FinalizePackage(RingBuffer_t *ptrRingBuffer);
/**
 * @brief function for remove last package from ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_RemovePackage(RingBuffer_t *ptrRingBuffer);

/**
 * @brief function for clear ring buffer
 * @param ptrRingBuffer - pointer to necessary ring buffer 
 * @return              - operation state
 */
RingBuffer_Messages_t ul_RingBuffer_Clear(RingBuffer_t *ptrRingBuffer);
