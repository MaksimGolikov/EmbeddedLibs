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
 * @brief function for change update button event
 * @param button - name of button
 * @param event - name of event
 */
void Callback_ChangeButtonEvent(uint8_t button, uint8_t event);




#endif /* COMMON_DATABASE_DATABASE_CALLBACKS_H_ */
