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



void Database_Init(void){

	for(uint8_t i = 0; i < FINISH_FLASH; i ++){
		sFLASH_ReadBuffer(&Database[i].Value, Database[i].AddressInFlash, (uint16_t*)Database[i].Type);

		if( (Database[i].Value < Database[i].MinValue) ||
			(Database[i].Value > Database[i].MaxValue)){
			 Database[i].Value = Database[i].DefaultValue;
		}
	}
}


DB_Message_t Database_ChangeFeild(DB_Field_t field, void *value, uint16_t sizeData) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		if ((sizeData >= Database[field].MinValue)
				&& (sizeData <= Database[field].MaxValue)) {

			 Database[field].Value = value;

			if (Database[field].Callback != NULL) {

				Database[field].Callback(Database[field].AddressInFlash,
						Database[field].Value, Database[field].Type);
			}
			result = db_OperationDone;
		}
	}

	return result;
}


DB_Message_t Database_ReadField(DB_Field_t field, void *value) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		value = Database[field].Value;
		result = db_OperationDone;
	}
	return result;
}


DB_Message_t Database_ReadMin(DB_Field_t field, void *value) {
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		value =  Database[field].MinValue;
		result = db_OperationDone;
	}
	return result;
}


DB_Message_t Database_ReadMax(DB_Field_t field, void *value){
	DB_Message_t result = db_OperationError;

	if ((field < DB_field_Amount) && (value != NULL)) {
		value = Database[field].MaxValue;
		result = db_OperationDone;
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
		Database[field].Value = Database[field].DefaultValue;
		result = db_OperationDone;
	}
	return result;
}
