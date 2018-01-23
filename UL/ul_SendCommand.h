/*
 * Name         ul_SendCommand.h
 * Author       Maksim Golikov (SW developer)
 * Created      21 DEC 2017
 * Description  utility for send command by IR led (header file)
 */


#ifndef UL_SEND_COMMAND_H_
#define UL_SEND_COMMAND_H_

#include "stm32f1xx_hal.h"
#include "stdint.h"


/**
   @brief function for set start internal value	 
*/
void ul_SendCommand_Init(void);

/**
  @brief function for set command for transmitby PWM
	@param numIR - number of nesessary PWM
  @param command - value for transmit 
*/
void ul_SendCommand_Send(uint8_t numIR, uint32_t command);

/** 
   @brief function for activate send process
*/
void ul_SendCommand_Run(void);

#endif /* UL_SEND_COMMAND_ */
