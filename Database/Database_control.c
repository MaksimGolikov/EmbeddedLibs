/*
 * Name         DataBase_control.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  utility for control Data base  (source file)
 */

#include "Common/DataBase/Database_control.h"
#include "Common/DataBase/Database_configuration.h"
#include "Common/DataBase/Database_describe.h"
#include "DRV/w25q32bv.h"
#include <string.h>



#define INDEX_NOT_FOUND      0xFF

uint8_t GetIdInDatabase(DB_Field_t field);




void Database_Init(void){

	for(uint8_t i = 0; i < DB_field_Amount; i ++){
		uint8_t index = GetIdInDatabase(i);
		if (index != INDEX_NOT_FOUND) {
			Database_SetDefaultIn(index);
			if(i < FINISH_FLASH ){
				uint32_t addressFlash = Database[index].AddressInFlash;
				if (addressFlash != 0) {
					uint8_t read[Database[index].Type];
					sFLASH_ReadBuffer(read, addressFlash, (uint16_t)Database[index].Type);
					for(uint8_t i = 0; i < 9; i++){
						Database[index].Value |= read[i] << ( (Database[index].Type * 8) - (i + 1)*8 );
					}
					if ((Database[index].Value < Database[index].MinValue)
							|| (Database[index].Value > Database[index].MaxValue)) {
						Database_SetDefaultIn(index);
					}
				}
			}
		}
    }


}


DB_Message_t Database_ChangeFeild(DB_Field_t field, void *value, uint16_t sizeData) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		uint8_t index = GetIdInDatabase(field);
		if (index != INDEX_NOT_FOUND) {
			uint32_t currentData;
			Database_ReadField(index, &currentData);

			if ((sizeData >= Database[index].MinValue)
					&& (sizeData <= Database[index].MaxValue)
					&& (currentData != Database[index].Value)) {

				memcpy(&Database[index].Value, value, sizeData);

				if (Database[index].Callback != NULL) {

					Database[index].Callback(Database[index].AddressInFlash,
							Database[index].Value, Database[index].Type);
				}
				result = db_OperationDone;
			}
		}
	}

	return result;
}


DB_Message_t Database_ReadField(DB_Field_t field, void *value) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		uint8_t index = GetIdInDatabase(field);
		if (index != INDEX_NOT_FOUND) {
			memcpy(value, &Database[index].Value, (uint16_t) Database[index].Type);
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_ReadMin(DB_Field_t field, void *value) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		uint8_t index = GetIdInDatabase(field);
		if(index != INDEX_NOT_FOUND){
			memcpy(value, &Database[index].MinValue, (uint16_t)Database[index].Type);
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_ReadMax(DB_Field_t field, void *value){
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		uint8_t index = GetIdInDatabase(field);
		if(index != INDEX_NOT_FOUND){
			memcpy(value, &Database[index].MaxValue, (uint16_t)Database[index].Type);
			result = db_OperationDone;
		}
	}
	return result;
}


DB_Message_t Database_SetDefault(void) {
	DB_Message_t result = db_OperationError;
	for (uint8_t i = 0; i < DB_field_Amount; i++) {
		Database_SetDefaultIn(i);
		result = db_OperationDone;
	}
	return result;
}


DB_Message_t Database_SetDefaultIn(DB_Field_t field) {
	DB_Message_t result = db_OperationError;

	if (field < DB_field_Amount) {
		uint8_t index = GetIdInDatabase(field);
		if (index != INDEX_NOT_FOUND) {
			Database[index].Value = Database[index].DefaultValue;
			result = db_OperationDone;
		}
	}
	return result;
}





uint8_t GetIdInDatabase(DB_Field_t field){
	uint8_t result = INDEX_NOT_FOUND;

	for(uint8_t i = 0; i < DB_field_Amount; i++){
		if(Database[i].FieldName == field){
			result = i;
			break;
		}
	}

	return result;
}

