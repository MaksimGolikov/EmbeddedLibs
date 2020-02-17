#include "modbus.h"
#include "modbus_config.h"

#include "RTU/modbus_RTU.h"

#include <stdint.h>






mb_error_t Modbus_Init(mb_type_mode_t type,
		               mb_work_mode_t mode,
					   modbus_defenition_t *handler){

	mb_error_t result = MB_ERR_STATUS_UNCORRECT_PARAM;
	if(mode > MB_WORKMDODE_AMOUNT){
		return result;
	}

	(*handler).status   = MB_STATUS_FREE;
	(*handler).typemode = type;
	(*handler).workmode = mode;

	switch(type){
		case MB_TYPEMODE_RTU:{

            #if MODBUS_COMMAND_WRITE_SINGLE_REGISTER
			 (*handler).wsr_function = MBRTU_WriteSingleRegister;
            #endif

            #if MODBUS_COMMAND_READ_SINGLE_REGISTER
			 (*handler).rsr_function = MBRTU_ReadSingleRegister;
            #endif

			result = MB_ERR_STATUS_SUCCESS;
		}break;
		case MB_TYPEMODE_TCP:{


			result = MB_ERR_STATUS_NOT_SUPPORTED;
		}break;
		default:
		break;
	}

	return result;
}

void       Modbus_OperationDoneClbk(modbus_defenition_t *handler){
	handler->status   = MB_STATUS_FREE;
}

mb_error_t Modbus_WriteSingleRegister(modbus_defenition_t *handler,
		                              uint8_t             slave_addr,
								      uint8_t             register_name,
								      uint16_t            data){

  #if MODBUS_COMMAND_WRITE_SINGLE_REGISTER
	mb_error_t result = MB_ERR_STATUS_BUS_BUISY;

    if( handler->status == MB_STATUS_FREE){
       handler->status = MB_STATUS_BUISY;
	   result          = handler->wsr_function(slave_addr, register_name, data);
    }
    return result;
   #else
	  return MB_ERR_STATUS_NOT_SUPPORTED;
  #endif
}

mb_error_t Modbus_ReadSingleRegister(modbus_defenition_t *handler,
		                             uint8_t             slave_addr,
								     uint8_t             register_name,
								     uint16_t            *data){

    #if MODBUS_COMMAND_READ_SINGLE_REGISTER
	  mb_error_t result = MB_ERR_STATUS_BUS_BUISY;
	  if(handler->status == MB_STATUS_FREE){
		 handler->status  = MB_STATUS_BUISY;
	     result           =  handler->rsr_function(slave_addr, register_name, data);
	  }
	  return result;
    #else
	  return MB_ERR_STATUS_NOT_SUPPORTED;
    #endif

}
