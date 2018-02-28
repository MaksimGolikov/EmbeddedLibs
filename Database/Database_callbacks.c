/*
 * Name         DataBase_callbacks.c
 * Author       Maksim Golikov (SW developer)
 * Created      19 FEB 2018
 * Description  file for describe Database callback`s (source file)
 */


#include "Common/DataBase/Database_callbacks.h"
#include "Common/Database/Database_control.h"

#include "DRV/drv_Button.h"
#include <string.h>






void Callback_ChangeButtonEvent(uint8_t button, uint8_t event){
	if(button < Buttons_Amount){
		if(event < ButtonsEvent_Amounnt){
			uint8_t nameParameter;
			switch(button){
			case TRIGGER_BUTTON:
				nameParameter = prm_ButtonFireEvent;
				break;
			case POWER_BUTTON:
				nameParameter = prm_ButtonPowerEvent;
				break;
			case RECHARGE_BUTTON:
				nameParameter = prm_ButtonPowerEvent;
				break;
			default:
				break;
			}
			Database_ChangeFeild(nameParameter, &event, sizeof(event));
		}
	}

}
