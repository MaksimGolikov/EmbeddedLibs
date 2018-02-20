/*
 * Name         DataBase_callbacks.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe Database callback`s (source file)
 */


#include "Common/DataBase/Database_callbacks.h"
#include "Common/DataBase/Database_control.h"
#include "DRV/w25q32bv.h"

#define MAX_SIZE_OF_FLASH_CELL     4



void Callback_SaveToFlash(uint32_t address, void *value, DB_DataTypes_t type) {

	if (value != NULL) {
		sFLASH_RewriteData((uint8_t*)value, address, (uint16_t*)type);
	}
}



