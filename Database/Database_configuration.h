/*
 * Name         DataBase_configuration.h
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe fields in database  (header file)
 */

#ifndef COMMON_DATABASE_DATABASE_CONFIGURATION_H_
#define COMMON_DATABASE_DATABASE_CONFIGURATION_H_



#include <Common/DataBase/Database_control.h>
#include <Common/DataBase/Database_describe.h>
//#include <Common/DataBase/Database_callbacks.h>

#include "Common/FlashMap.h"

void Callback_SaveToFlash(uint32_t address, void *value, DB_DataTypes_t type);



Field_struct_t Database[]={
    //  Name                               Type,      Value     Min     Max     Default    Address                                Callback
	  //  {ID,                            UINT8,      0,       1,      127,       1,      ADDR_ID,                            Callback_SaveToFlash},
		{prm_GameMode,                     UINT8,      0,       0,       3,        0},
		{prm_PowerButtonEvent,             UINT8,      0,       0,       7,        0},
	    {prm_TriggerButtonEvent,           UINT8,      0,       0,       7,        0}
};




#endif /* COMMON_DATABASE_DATABASE_CONFIGURATION_H_ */
