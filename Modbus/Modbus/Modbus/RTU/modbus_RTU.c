#include "modbus_RTU.h"
#include "../modbus_CRC16.h"
#include "../modbus_config.h"

#if MODBUS_COMMAND_WRITE_SINGLE_REGISTER == 1

mb_error_t MBRTU_WriteSingleRegister(uint8_t             slave_addr,
	                                 uint8_t             register_name,
	                                 uint16_t            data){

	//Here should be realization of preparing buffer and send prepared data to the usrt function
return MB_ERR_STATUS_SUCCESS;
}

#endif

#if MODBUS_COMMAND_READ_SINGLE_REGISTER == 1

mb_error_t MBRTU_ReadSingleRegister(uint8_t             slave_addr,
                                    uint8_t             register_name,
                                    uint16_t            data){
	return MB_ERR_STATUS_SUCCESS;
}

#endif
