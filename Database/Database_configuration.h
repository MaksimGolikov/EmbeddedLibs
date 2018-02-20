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
#include <Common/DataBase/Database_callbacks.h>

#include "Common/FlashMap.h"

void Callback_SaveToFlash(uint32_t address, void *value, DB_DataTypes_t type);



Field_struct_t Database[DB_field_Amount]={
    //  Name                            Type,      Value     Min     Max     Default    Address                                Callback
	    {ID,                            UINT8,      0,       1,      127,       1,      ADDR_ID,                            Callback_SaveToFlash},
		{COLOR,                         UINT8,      0,       0,      4,         1,      ADDR_TEAM_COLOR,                    Callback_SaveToFlash},
		{INVULNERABILITY_TIME,          UINT32,     0,       0,      100,       10,     ADDR_DEF_INVULNERAB_TIME,           Callback_SaveToFlash},
		{HEALTH,                        UINT16,     0,       0,      999,       30,     ADDR_DEF_HEALTH,                    Callback_SaveToFlash},
		{HEADBAND_VIBRO_TIME,           UINT32,     0,       0,      5000,      500,    ADDR_HEADBAND_VIBRO_TIME,           Callback_SaveToFlash},
		{SHOCK_TIME,                    UINT32,     0,       0,      5000,      500,    ADDR_SHOCK_TIME,                    Callback_SaveToFlash},
		{SOUND_VOLUME,                  UINT8,      0,       0,      100,       10,     ADDR_SOUND_VOLUME,                  Callback_SaveToFlash},
		{FRIENDLY_FIRE,                 BOOL,       0,       0,      1,         0,      ADDR_FRIENDLY_FIRY,                 Callback_SaveToFlash},
		{RESPOWNS_NUMBER,               UINT8,      0,       0,      100,       0,      ADDR_STAT_RESPAWNS,                 Callback_SaveToFlash},
		{KILLS_NUMBER,                  UINT8,      0,       0,      100,       0,      ADDR_STAT_KILLS,                    Callback_SaveToFlash},
		{MEDICINE_NUMBER,               UINT8,      0,       0,      100,       0,      ADDR_STAT_MED_NUMBER,               Callback_SaveToFlash},
		{FEADBACK_SHOTS_NUMBER,         UINT16,     0,       0,      100,       0,      ADDR_STAT_ADD_FEEDBACK_SHOTS,       Callback_SaveToFlash},
		{FEADBACK_KILLS_NUMBER,         UINT16,     0,       0,      100,       0,      ADDR_STAT_ADD_FEEDBACK_KILLS,       Callback_SaveToFlash},
		{SHOTS_NUMBER,                  UINT16,     0,       0,      100,       0,      ADDRESS_SHOTS_COUNT,                Callback_SaveToFlash},
		{SHOTS_FROM_COUNT,              UINT16,     0,       0,      100,       0,      ADDR_STATISTICS_SHOTS_FROM,         Callback_SaveToFlash},
		{DAMAGE_FROM_COUNT,             UINT16,     0,       0,      100,       0,      ADDR_STATISTICS_DAMAGE_FROM,        Callback_SaveToFlash},
		{KILLS_WHO_COUNT,               UINT16,     0,       0,      100,       0,      ADDR_STAT_KILLS,                    Callback_SaveToFlash},
		{DAMAGE_TOMMY_GUN,              UINT8,      0,       1,      100,       1,      ADDR_DAMAGE ,                       Callback_SaveToFlash},
		{SHOTS_IN_AUTOMOD_TOMMY_GUN,    UINT8,      0,       1,      5,         3,      ADDR_SHOTS_IN_AUTOMODE,             Callback_SaveToFlash},
		{IR_STRENGTH_TOMMY_GUN,         UINT8,      0,       0,      100,       50,     ADDRESS_IR_STRANGHT,                Callback_SaveToFlash},
		{FIRE_RATE_TOMMY_GUN,           UINT16,     0,       0,      999,       545,    ADDR_FIRE_RATE,                     Callback_SaveToFlash},
		{CLIPS_NUMBER_TOMMY_GUN,        UINT16,     0,       0,      100,       0,      ADDR_DEF_CLIPS_NUMB,                Callback_SaveToFlash},
		{AMMO_NUMBER_TOMMY_GUN,         UINT16,     0,       0,      100,       0,      ADDR_DEF_AMMO_NUMB,                 Callback_SaveToFlash},
		{AUTORESPOUN_TIME_TOMMY_GUN,    UINT32,     0,       0,      1000,      100,    ADDR_DEF_AVTO_RESPOUND_TIME,        Callback_SaveToFlash},
		{GAME_MODE,                     UINT8,      0,       0,       3,        0}
};




#endif /* COMMON_DATABASE_DATABASE_CONFIGURATION_H_ */
