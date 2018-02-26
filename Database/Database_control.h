/*
 * Name         DataBase_control.h
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  utility for control Data base  (header file)
 */

#ifndef COMMON_DATABASE_DATABASE_CONTROL_H_
#define COMMON_DATABASE_DATABASE_CONTROL_H_

#include <Common/DataBase/Database_describe.h>
#include "stdint.h"



typedef enum{
	BOOL   = 1,
	UINT8  = 1,
	INT8   = 1,
	UINT16 = 2,
	INT16  = 2,
	UINT32 = 4,
	INT32  = 4,
	MASS_UINT16_254 = 254,
	Type_Amount
}DB_DataTypes_t;


typedef struct{
	DB_Field_t      FieldName;
	DB_DataTypes_t  Type;
	int32_t         MinValue;
	uint32_t        MaxValue;
	int32_t         DefaultValue;
	uint32_t        AddressInFlash;
	void            (*Callback)();
	int32_t         Value;
}Field_struct_t;



typedef enum{
	db_OperationDone,
	db_OperationError,
	db_SavedInFlash,
}DB_Message_t;


typedef union{
	uint8_t  mass[4];
	uint32_t data;
}convertType_t;


/**
 * @brief function for initialize DB. Should call one time before other
 */
void Database_Init(void);


/**
 * @brief function for rewrite value in database field
 * @param field    - name of necessary database field
 * @param value    - pointer to variable which contain necessary value
 * @param sizeData - size of value
 * @return - state of operation
 */
DB_Message_t Database_ChangeFeild(DB_Field_t field, void *value, uint16_t sizeData);


/**
 * @brief function for read value from  database field
 * @param field - name of necessary database field
 * @param value - pointer to variable which should contain return value
 * @return - state of operation
 */
DB_Message_t Database_ReadField(DB_Field_t field, void *value);


/**
 * @brief function for read minimal value from  database field
 * @param field - name of necessary database field
 * @param value - pointer to variable which should contain return value
 * @return - state of operation
 */
DB_Message_t Database_ReadMin(DB_Field_t field, void *value);


/**
 * @brief function for read value from  database field
 * @param field - name of necessary database field
 * @param value - pointer to variable which should contain return value
 * @return - state of operation
 */
DB_Message_t Database_ReadMax(DB_Field_t field, void *value);


/**
 * @brief function for read address in flash
 * @param field - name of necessary database field
 * @param value - pointer to variable which should contain return value
 * @return - state of operation
 */
DB_Message_t Database_ReadAddress(DB_Field_t field, void *value);


/**
 * @brief function for read type of field
 * @param field - name of necessary database field
 * @param value - pointer to variable which should contain return value
 * @return - state of operation
 */
DB_Message_t Database_ReadType(DB_Field_t field, void *value);


/**
 * @brief function for set default values for all fields in database
 * @return - state of operation
 */
DB_Message_t Database_SetDefault(void);

/**
 * @brief function for set default value for  field in database
 * @param field - name of necessary database field
 * @return - state of operation
 */
DB_Message_t Database_SetDefaultIn(DB_Field_t field);


#endif /* COMMON_DATABASE_DATABASE_CONTROL_H_ */
