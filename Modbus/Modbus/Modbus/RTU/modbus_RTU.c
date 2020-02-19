#include "modbus_RTU.h"
#include "../modbus_CRC16.h"
#include "../modbus_config.h"

#include <stddef.h>
#include <stdlib.h>

#define RECIEVER_ID(data)       (data[0])
#define RECIEVER_FUNCTION(data) (data[1])

#define SIZE_OF_RESPONSE_BUF(data_len) (data_len + 5) // Addr(1) + func(1)+ Byte num(1) + CRC(2)

mb_error_t MBRTU_SendResponse(  bus_function         send,
		                        uint8_t              my_dev_id,
								uint8_t              answer_function,
								uint8_t              *data,
								uint8_t              data_len){
	mb_error_t result = MB_ERR_STATUS_FAIL_MEM_ALLOCATED;

	uint16_t buf_total_len = SIZE_OF_RESPONSE_BUF(data_len);

	uint8_t *send_buff = (uint8_t*)malloc(buf_total_len);

	uint8_t inx = 0;
	if(send_buff != NULL){
		send_buff[inx] = my_dev_id;         inx ++;
		send_buff[inx] = answer_function;   inx ++;
		send_buff[inx] = data_len;          inx ++;

		while(data_len--){
			send_buff[inx] = *data;
			data++;
			inx ++;
		}

		uint16_t crc = GetCRC16(send_buff,  buf_total_len - 2 );

		send_buff[inx] = (crc >> 8); inx++;
		send_buff[inx] = (crc & 0x00FF);

        send(send_buff, buf_total_len);
        result = MB_ERR_STATUS_SUCCESS;
        free(send_buff);
	}
    return result;
}


mb_error_t MBRTU_ParseRequest(uint8_t *data, uint16_t data_length, uint8_t my_dev_id){

	mb_error_t result = MB_ERR_STATUS_UNCORRECT_PARAM;

	if(data != NULL){
		uint16_t cul_crc  = GetCRC16(data,  (data_length - 2) );
        uint16_t read_crc = (data[data_length - 2] << 8) | data[data_length - 1];
        result            = MB_ERR_STATUS_CRC_FAIL;

        if(cul_crc == read_crc){

        	if(my_dev_id == RECIEVER_ID(data) ){
				switch(RECIEVER_FUNCTION(data)){

				  #if MODBUS_COMMAND_READ_DISCRET_NPUT_REGISTER
					case MB_COMMAND_READ_DISCRET_INPUT:{
						uint16_t first_reg = (data[2] << 8) | data[3];
						uint16_t numb_reg  = (data[4] << 8) | data[5];

						ReadDiscretInput(first_reg, numb_reg);
					}break;
				  #endif


					default:
						break;
				}
        	}
        }
	}

	return result;
}
