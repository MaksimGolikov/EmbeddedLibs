/*
 * Name        ul_Audio.h
 * Author      Maksim Golikov (SW developer)
 * Created on: 16 FEB, 2018
 * Description utility for work with audio (source file)
 */


#include "UL/ul_Audio.h"
#include "UL/ul_RingBuffer.h"

#include "DRV/audio.h"
#include "DRV/drv_SysClock.h"

#include <string.h>




#define SIZE_SOUND_BUFFER              120  // buffer can contain at one time 20 sounds
#define SIZE_ADDRES_AND_DELAYTIME      6


#define MIN_SOUND_VOLUME       5
#define MAX_SOUND_VOLUME       100


typedef union{
 uint8_t  mass[4];
 uint32_t data;
}dataSaveType_t;

typedef union{
 uint8_t  mass[2];
 uint16_t data;
}timeSaveType_t;


static uint32_t startDelayTime;
static uint16_t delayTimeToStartNextSound;
static uint8_t  countOfCreatedPackage;

static  uint8_t buffer[SIZE_SOUND_BUFFER];
static  RingBuffer_t soundBuffer;





void ul_Audio_Init(void){

	delayTimeToStartNextSound = 0;
	countOfCreatedPackage = 0;
	startDelayTime = drv_SysClock_GetCurrentTime();


	memset(buffer, 0 , SIZE_SOUND_BUFFER);
	ul_RingBuffer_Create(&soundBuffer, buffer, SIZE_SOUND_BUFFER);
}


BufferMessage_t ul_Audio_AddSound(uint32_t addressOfSound, uint16_t delayToNext){
	BufferMessage_t result = audio_Error;

	if( soundBuffer.size > 0
		&& ((soundBuffer.fullness + SIZE_ADDRES_AND_DELAYTIME) < soundBuffer.size)){
		dataSaveType_t save;
		timeSaveType_t delayTime;

		save.data = addressOfSound;
        delayTime.data = delayToNext;

		ul_RingBuffer_CreatePackage(&soundBuffer);
		for(uint8_t i = 0; i < sizeof(addressOfSound); i ++){
			ul_RingBuffer_PushByte(&soundBuffer, save.mass[i]);
		}
		ul_RingBuffer_FinalizePackage(&soundBuffer);

		ul_RingBuffer_CreatePackage(&soundBuffer);
		for (uint8_t i = 0; i < sizeof(delayToNext); i++) {
			ul_RingBuffer_PushByte(&soundBuffer, delayTime.mass[i]);
		}
		ul_RingBuffer_FinalizePackage(&soundBuffer);

		result = audio_Data_added;
		countOfCreatedPackage += 1;
	}

	return result;
}


BufferMessage_t ul_Audio_Stop(void){
	BufferMessage_t result = audio_Error;

	audio_stop_all();

	result = audio_Ok;
	return  result;
}


BufferMessage_t ul_Audio_SetSoundValue(uint8_t volume){

	if(volume < MIN_SOUND_VOLUME){
		volume = MIN_SOUND_VOLUME;
	}
	if(volume > MAX_SOUND_VOLUME){
		volume = MAX_SOUND_VOLUME;
	}

	audio_set_volume(volume);
	return audio_Ok;
}


BufferMessage_t ul_Audio_ExistValue(void){
	BufferMessage_t result = audio_Buffer_empty;

	if(countOfCreatedPackage > 0){
		result = audio_Buffer_full;
	}

	return result;
}

void ul_Audio_Run(void){

	if(countOfCreatedPackage > 0
			&& drv_SysClock_IsTimeSpent(startDelayTime, (uint32_t)delayTimeToStartNextSound) ){
	   dataSaveType_t send_data;
	   timeSaveType_t delayTime;
       uint16_t       size_data;
       uint16_t       size_delay;

       ul_RingBuffer_Pop(&soundBuffer, send_data.mass, &size_data);
       ul_RingBuffer_Pop(&soundBuffer, delayTime.mass, &size_delay);

       audio_start(send_data.data);
       delayTimeToStartNextSound = delayTime.data;
       startDelayTime = drv_SysClock_GetCurrentTime();
       countOfCreatedPackage -= 1;
	}
	audio_tim_handler();
}
