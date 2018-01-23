/*
 * Name         drv_IRresiver.h
 * Author       Maksim Golikov (SW developer)
 * Created      15 DEC 2017
 * Description  driver for control IR resiver (header file)
 */

#ifndef __DRV_IRRESIVER_H
#define __DRV_IRRESIVER_H
  
#include "stdint.h"
#include "stm32f1xx_hal.h"


typedef enum{
	MAIN_IR
}IRSensors_t;



/**
   @brief function for initialize internal parameters base value 
*/
void drv_IRresiver_Init(GPIO_TypeDef *port, uint8_t  pinStruct);

/**
  @brief  function for read pin
  @param  nameIR name of necessary IR sensor
  @return resived command
*/
void drv_IRresiver_Read(IRSensors_t nameIR);


/**
  @brief  function for take value of selected resiver
  @param  numIR - name of necessary IR resiver
  @return last read value
*/
uint32_t drv_IRresiver_GetLastState(IRSensors_t nameIR);








/*
   @brief function for initialize internal parameters base value

void drv_IRresiver_Init(GPIO_TypeDef *port, uint8_t  pinStruct);*/

/*
  @brief  function for read pin
	@return resived command
*/
//uint32_t drv_IRresiver_Read(void);



#endif /* __DRV_IRRESIVER_H */

