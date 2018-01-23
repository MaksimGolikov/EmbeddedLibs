/*
 * Name         ul_ParameterGenerator.c
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  utility for change game parameter (source file)
 */

#include "UL/ul_ParameterGenerator.h"

#define  COUNT_OF_POSSIBLE_DAMAGE    16

#define  SHIFT_FOR_PLAYER_ID         31
#define  SHIFT_FOR_COMMAND_COLOR     24
#define  SHIFT_FOR_DAMAGE            22




typedef struct{ /* WARNING  the order of placement is important*/
	uint32_t  freeBit      :8;
	uint32_t  crc          :8;
	uint32_t  zeroToByte   :2;
	uint32_t  damageCode   :4;
	uint32_t  commandColor :2;
	uint32_t  idPlayer     :7;
	uint32_t  firstZero    :1;
}ShotStruct_t;

typedef union{
	uint32_t     shotCommand;
	ShotStruct_t data;
}ShotCommand_t;




typedef struct{
	ShotCommand_t   shotConfiguration;
	uint32_t        shotCommand;
}GameConfiguration_t;


static uint8_t PossibleDamage []={
		1, 2, 4, 5, 7, 10, 15, 17, 20, 25, 30, 35, 40, 50, 75, 100
};




static GameConfiguration_t gameConfiguration;



void ul_ParameterGenerator_Init(void){
	gameConfiguration.shotConfiguration.data.freeBit      = 0;
	gameConfiguration.shotConfiguration.data.crc          = 0;
	gameConfiguration.shotConfiguration.data.zeroToByte   = 0;
	gameConfiguration.shotConfiguration.data.damageCode   = 0;
	gameConfiguration.shotConfiguration.data.commandColor = RED_COMMAND;
	gameConfiguration.shotConfiguration.data.idPlayer     = 1;
	gameConfiguration.shotConfiguration.data.firstZero    = 0;


	gameConfiguration.shotCommand = gameConfiguration.shotConfiguration.shotCommand;
}



void ul_ParameterGenerato_SetNewDamage(uint8_t newDamage){

	gameConfiguration.shotConfiguration.data.damageCode = 0;

	for(uint8_t i = 0; i < COUNT_OF_POSSIBLE_DAMAGE; i++){
		if(PossibleDamage[i] == newDamage){
			gameConfiguration.shotConfiguration.data.damageCode = i;
			break;
		}
	}

	gameConfiguration.shotCommand = gameConfiguration.shotConfiguration.shotCommand;
}


void ul_ParameterGenerato_SetNewCommand(Command_t newColor){
	gameConfiguration.shotConfiguration.data.commandColor = newColor;
	gameConfiguration.shotCommand = gameConfiguration.shotConfiguration.shotCommand;
}


void ul_ParameterGenerato_SetNewPlayerID(uint8_t newPlayerID) {

	if(newPlayerID < 1){
		newPlayerID = 1;
	}
	if(newPlayerID > 127){
		newPlayerID = 127;
	}

	gameConfiguration.shotConfiguration.data.idPlayer = newPlayerID;
	gameConfiguration.shotCommand = gameConfiguration.shotConfiguration.shotCommand;
}


uint32_t ul_ParameterGenerato_GetShotCommand(void){
	return gameConfiguration.shotCommand;
}
