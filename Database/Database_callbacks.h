/*
 * Name         DataBase_callbacks.h
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe Database callback`s (header file)
 */

#ifndef COMMON_DATABASE_DATABASE_CALLBACKS_H_
#define COMMON_DATABASE_DATABASE_CALLBACKS_H_


#include <Common/DataBase/Database_describe.h>
#include "Common/DataBase/Database_configuration.h"
#include "stdint.h"


/**
 * @brief function for save value of parameter to DB
 * @param address - address in flash for write
 * @param value   - pointer to necessary value
 * @param type    - type of value
 */
void Callback_SaveToFlash(uint32_t address, void *value, DB_DataTypes_t type);





#endif /* COMMON_DATABASE_DATABASE_CALLBACKS_H_ */
