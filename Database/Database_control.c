/*
 * Name         DataBase_control.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  utility for control Data base  (source file)
 */

#include "Common/Database/Database_control.h"
#include "Common/Database/Database_configuration.h"
#include "Common/Database/Database_describe.h"
#include "DRV/w25q32bv.h"
#include <string.h>



#define INDEX_NOT_FOUND      0xFF

uint8_t GetIdInDatabase(DB_Field_t parameter);







void Database_Init(void) {

	for (uint8_t parameter = 0; parameter < DB_parameters_Amount; parameter++) {
		uint8_t indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			if (parameter > FINISH_FLASH) {
				Database_SetDefaultIn(parameter);
			} else {
#ifdef DRV_FLASH_H
					if (Database[indexDB].Type <= UINT32) {
						convertType_t read;
						sFLASH_ReadBuffer(read.mass, Database[indexDB].AddressInFlash, (uint16_t) Database[indexDB].Type);
						Database[indexDB].Value = 0;
						for (uint8_t i = 0; i < Database[indexDB].Type; i++) {
							Database[indexDB].Value |= read.mass[i]	<< ((Database[indexDB].Type * 8) - (i + 1) * 8);
						}
						if ((Database[indexDB].Value < Database[indexDB].MinValue)
								|| (Database[indexDB].Value	> Database[indexDB].MaxValue)) {
							Database_SetDefaultIn(indexDB);
						}
					}
#endif
			}
		}
	}

}


DB_Message_t Database_ChangeFeild(DB_Field_t parameter, void *value, uint16_t sizeData) {
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;
    uint8_t dataChanged = 0;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			if(Database[indexDB].Type <= UINT32){
				convertType_t newData;
				convertType_t currentData;
                newData.data = 0;

				Database_ReadField(indexDB, currentData.mass);
				memcpy(newData.mass, value, sizeData);

				if ((newData.data >= Database[indexDB].MinValue)
									&& (newData.data <= Database[indexDB].MaxValue)
									&& (newData.data != currentData.data)){
					memcpy(&Database[indexDB].Value, value, sizeData);
					result = db_OperationDone;
#ifdef DRV_FLASH_H
					if(parameter < FINISH_FLASH){
							memcpy(&Database[indexDB].AddressInFlash, newData.mass, (uint16_t)Database[indexDB].Type);
							result = db_SavedInFlash;
							dataChanged = 1;
					}
#endif
				}
			}else{
				if (Database[indexDB].AddressInFlash != 0) {
					uint8_t write[Database[indexDB].Type];
					if(parameter < FINISH_FLASH){
#ifdef DRV_FLASH_H
						sFLASH_ReadBuffer(write, Database[indexDB].AddressInFlash, (uint16_t)Database[indexDB].Type);
#endif
					}else{
						memcpy(&Database[indexDB].AddressInFlash, write, (uint16_t)Database[indexDB].Type);
					}
					result = db_OperationDone;
					dataChanged = 1;
				}
			}
			if (dataChanged && Database[indexDB].Callback != NULL) {
				Database[indexDB].Callback(parameter);
			}
		}
	}

	return result;
}


DB_Message_t Database_ReadField(DB_Field_t parameter, void *value) {
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		 indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			if(Database[indexDB].Type <= UINT32){
				memcpy(value, &Database[indexDB].Value, (uint16_t) Database[indexDB].Type);
				result = db_OperationDone;
			} else {
					uint8_t read[Database[indexDB].Type];
					if(parameter < FINISH_FLASH){
#ifdef DRV_FLASH_H
						sFLASH_ReadBuffer(read, Database[indexDB].AddressInFlash, (uint16_t)Database[indexDB].Type);
#endif
					}else{
						memcpy(read, &Database[indexDB].AddressInFlash, (uint16_t)Database[indexDB].Type);
					}
					memcpy(value, read, Database[indexDB].Type);
					result = db_OperationDone;
			}

		}
	}
	return result;
}


DB_Message_t Database_ReadMin(DB_Field_t parameter, void *value) {
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		indexDB = GetIdInDatabase(parameter);
		if(indexDB != INDEX_NOT_FOUND){
			memcpy(value, &Database[indexDB].MinValue, sizeof(Database[indexDB].MinValue));
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_ReadMax(DB_Field_t parameter, void *value){
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		indexDB = GetIdInDatabase(parameter);
		if(indexDB != INDEX_NOT_FOUND){
			memcpy(value, &Database[indexDB].MaxValue, sizeof(Database[indexDB].MaxValue));
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_ReadAddress(DB_Field_t parameter, void *value) {
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			memcpy(value, &Database[indexDB].AddressInFlash, sizeof(Database[indexDB].AddressInFlash));
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_ReadType(DB_Field_t parameter, void *value){
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if ((parameter < DB_parameters_Amount) && (value != NULL)) {
		indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			memcpy(value, &Database[indexDB].Type, sizeof(Database[indexDB].Type));
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_SetDefault(void) {
	DB_Message_t result = db_OperationError;
	for (uint8_t i = 0; i < DB_parameters_Amount; i++) {
		Database_SetDefaultIn(i);
		result = db_OperationDone;
	}
	return result;
}


DB_Message_t Database_SetDefaultIn(DB_Field_t parameter) {
	DB_Message_t result = db_OperationError;
	uint8_t indexDB;

	if (parameter < DB_parameters_Amount) {
		indexDB = GetIdInDatabase(parameter);
		if (indexDB != INDEX_NOT_FOUND) {
			if(Database[indexDB].Type <= UINT32){
				Database[indexDB].Value = Database[indexDB].DefaultValue;
				result = db_OperationDone;
			}else{
				memset(&Database[indexDB].AddressInFlash, 0, Database[indexDB].Type);
			}
		}
	}
	return result;
}





uint8_t GetIdInDatabase(DB_Field_t parameter) {
	uint8_t result = INDEX_NOT_FOUND;

	if (parameter != FINISH_FLASH) {
		for (uint8_t i = 0; i < DB_parameters_Amount; i++) {
			if (Database[i].FieldName == parameter) {
				result = i;
				break;
			}
		}
	}

	return result;
}

