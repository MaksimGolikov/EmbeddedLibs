#include "modbus.h"
#include "modbus_config.h"

#include "RTU/modbus_RTU.h"

#include <stdint.h>
#include <stddef.h>





mb_error_t Modbus_Init(mb_type_mode_t      type,
		               mb_work_mode_t      mode,
					   uint8_t             id,
					   bus_function       send_buf_func,
					   bus_function       receive_buf_func,
					   modbus_defenition_t *handler){

	mb_error_t result = MB_ERR_STATUS_UNCORRECT_PARAM;
	if(mode > MB_WORKMDODE_AMOUNT || send_buf_func == NULL || receive_buf_func == NULL){
		return result;
	}

	(*handler).status               = MB_STATUS_FREE;
	(*handler).typemode             = type;
	(*handler).workmode             = mode;

	(*handler).send_buf_function    = send_buf_func;
	(*handler).receive_buf_function = receive_buf_func;
    (*handler).id                   = id;


	switch(type){
		case MB_TYPEMODE_RTU:{

			(*handler).functions.parse_request = MBRTU_ParseRequest;
            (*handler).functions.send_response = MBRTU_SendResponse;

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

mb_error_t Modbus_SendResponse(modbus_defenition_t   *handler,
							   mb_command_t          command,
							   uint8_t               *data,
							   uint8_t               data_len){
	mb_error_t result = MB_ERR_STATUS_BUS_BUISY;

    return handler->functions.send_response(handler->send_buf_function,
    		                                handler->id,
											(uint8_t)command,
											data,
											data_len);

	return result;
}

mb_error_t Modbus_ReadQuery(modbus_defenition_t  *handler, uint8_t *data, uint16_t data_length){

	return handler->functions.parse_request(data, data_length, handler->id);
}
