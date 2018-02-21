/*
 * Name         DataBase_callbacks.h
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe Database callback`s (header file)
 */

#ifndef COMMON_DATABASE_DATABASE_CALLBACKS_H_
#define COMMON_DATABASE_DATABASE_CALLBACKS_H_


#include <Common/DataBase/Database_control.h>
#include "stdint.h"


/**
 * @brief function for save value of parameter to DB
 * @param address  - address in flash for write
 * @param value    - pointer to necessary value
 * @param sizeData - size of tent data
 */
void Callback_SaveToFlash(uint32_t address, void *value, uint16_t sizeData);





#endif /* COMMON_DATABASE_DATABASE_CALLBACKS_H_ */
