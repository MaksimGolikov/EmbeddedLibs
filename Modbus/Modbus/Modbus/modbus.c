#include "modbus.h"
#include "modbus_config.h"

#include "RTU/modbus_RTU.h"

#include <stdint.h>
#include <stddef.h>


typedef enum{
	MB_MASTER_IDLE,
	MB_MASTER_WAITING_FOR_A_RESPONSE,
}mb_master_mode_t;


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


    (*handler).master_mode          = MB_MASTER_IDLE;
    (*handler).response_timeout     = 500; //1 s in ms


	switch(type){
		case MB_TYPEMODE_RTU:{

			(*handler).functions.parse_request = MBRTU_ParseRequest;
            (*handler).functions.send_response = MBRTU_SendResponse;
            (*handler).functions.send_request  = MBRTU_SendRequest;

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



mb_error_t Modbus_SendResponse(modbus_defenition_t   *handler,
							   mb_command_t          command,
							   uint8_t               *data,
							   uint8_t               data_len,
							   uint16_t              first_reg){
	mb_error_t result = MB_ERR_STATUS_BUS_BUISY;

    return handler->functions.send_response(handler->send_buf_function,
    		                                handler->id,
											first_reg,
											(uint8_t)command,
											data,
											data_len);

	return result;
}

mb_error_t Modbus_ParseQuery(modbus_defenition_t  *handler, uint8_t *data, uint16_t data_length){


	if(handler->workmode == MB_WORKMODE_MASTER){
	    if(handler->master_mode == MB_MASTER_WAITING_FOR_A_RESPONSE){
	        handler->master_mode = MB_MASTER_IDLE;
	        handler->status      = MB_STATUS_FREE;
	    }else{
	        printf("Exit by unknown \n");
	        return MB_ERR_STATUS_UNKHOWN_PACKAGE;
	    }
	}

	return handler->functions.parse_request(data, data_length, handler->id,
			                                handler->workmode, handler->last_funct);
}



/************************
 * Master`s functions
 ***********************/

mb_error_t Modbus_MasterRequest_ReadDisret(modbus_defenition_t  *handler,
		                                   uint16_t             slave_id,
										   uint16_t             reg_name){
	mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

	if(handler->status == MB_STATUS_FREE){

		 uint8_t data[4] = {0};

		 /* First two byte of the frame is address of the register [Hi,Lo]*/
		 data[0] = reg_name >> 8;
		 data[1] = (reg_name & 0xFF);

		 /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
		 data[2] = 0;
		 data[3] = 1;

		 status = handler->functions.send_request(handler->send_buf_function,
		    	   	                              slave_id,
												  MB_COMMAND_READ_DISCRET_INPUT,
												  data,
												  sizeof(data) );
		 if(status == MB_ERR_STATUS_SUCCESS){
			 handler->status            = MB_STATUS_BUISY;

			 handler->last_funct        = MB_COMMAND_READ_DISCRET_INPUT;
             handler->id                = slave_id;

             handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
			 handler->request_send_time = GET_CURRENT_TIME;
		 }
	}

	return status;
}






void Modbus_MasterRun(modbus_defenition_t  *handler){
	if(handler->workmode == MB_WORKMODE_MASTER){
		switch(handler->master_mode){
			case MB_MASTER_IDLE:{

			}break;
			case MB_MASTER_WAITING_FOR_A_RESPONSE:{
				if( IS_TIME_SPENT(handler->request_send_time, handler->response_timeout) ){
					handler->master_mode = MB_MASTER_IDLE;
					handler->status      = MB_STATUS_FREE;
				}
			}break;
			default:
				break;
		}
	}
}
