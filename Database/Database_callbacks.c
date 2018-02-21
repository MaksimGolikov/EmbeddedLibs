/*
 * Name         DataBase_callbacks.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe Database callback`s (source file)
 */


#include "Common/DataBase/Database_callbacks.h"
#include "DRV/w25q32bv.h"
#include <string.h>


void Callback_SaveToFlash(uint32_t address, void *value, uint16_t sizeData) {

	if (value != NULL) {
		uint8_t read[sizeData];
		memcpy(read, value, sizeData);
		sFLASH_RewriteData(read, address, sizeData);
	}
}



