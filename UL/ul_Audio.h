/*
 * Name        ul_Audio.h
 * Author      Maksim Golikov (SW developer)
 * Created on: 16 FEB, 2018
 * Description utility for work with audio (header file)
 */

#ifndef UL_UL_AUDIO_H_
#define UL_UL_AUDIO_H_


#include <stdint.h>

typedef enum{
	audio_Data_added,
	audio_Buffer_full,
	audio_Buffer_empty,
	audio_Ok,
	audio_Error
}BufferMessage_t;


/**
 * @brief function for initialize default values of parameters
 */
void ul_Audio_Init(void);


/**
 * @brief function for add new address  to prepare buffer
 * @param addresOfSound - address of necessary track in flash
 * @param delayToNext   - time to start play next sound
 * @return state of operation
 */
BufferMessage_t ul_Audio_AddSound(uint32_t addressOfSound, uint16_t delayToNext);



/**
 * @brief function for stop play sound
 * @return state of operation
 */
BufferMessage_t ul_Audio_Stop(void);


/**
 * @brief function for change volume of sound
 * @param volume new value of sound
 * @return state of operation
 */
BufferMessage_t ul_Audio_SetSoundValue(uint8_t volume);


/**
 * @brief function for check audio buffer state
 * @return state of sound buffer
 */
BufferMessage_t ul_Audio_ExistValue(void);


/**
 * @brief function for start play sound
 */
void ul_Audio_Run(void);

#endif /* UL_UL_AUDIO_H_ */
