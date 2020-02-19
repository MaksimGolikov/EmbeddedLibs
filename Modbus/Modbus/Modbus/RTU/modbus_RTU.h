#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "../modbus.h"
#include <stdint.h>


mb_error_t MBRTU_SendResponse(  bus_function          send,
		                        uint8_t               my_dev_id,
								uint16_t              first_reg,
								uint8_t               answer_function,
								uint8_t               *data,
								uint8_t               data_len);

mb_error_t MBRTU_ParseRequest(uint8_t *data,
		                      uint16_t data_length,
							  uint8_t my_dev_id);

#endif
