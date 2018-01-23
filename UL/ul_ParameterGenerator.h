/*
 * Name         ul_ParameterGenerator.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  utility for change game parameter (header file)
 */

#ifndef UL_UL_PARAMETERGENERATOR_H_
#define UL_UL_PARAMETERGENERATOR_H_

#include <stdint.h>


typedef enum{
	RED_COMMAND,
	BLUE_COMMAND,
	YELLOW_COMMAND,
	GREEN_COMMAND
}Command_t;



/**
 * @brief function for initialize parameter with base value
 */
void ul_ParameterGenerator_Init(void);


/**
 * @brief function for change command of shot
 * @param newDamage - value of necessary damage
 */
void ul_ParameterGenerato_SetNewDamage(uint8_t newDamage);


/**
 * @brief function for change color of command
 * @param newCommand - value of new color
 */
void ul_ParameterGenerato_SetNewCommand(Command_t newColor);


/**
 * @brief function for change ID of player
 * @param newPlayerID - new ID of player
 */
void ul_ParameterGenerato_SetNewPlayerID(uint8_t newPlayerID);


/**
 * @brief  function for take command for shot
 * @return command for shot
 */
uint32_t ul_ParameterGenerato_GetShotCommand(void);


#endif /* UL_UL_PARAMETERGENERATOR_H_ */
