#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "../modbus.h"

mb_error_t MBRTU_WriteSingleRegister(uint8_t             slave_addr,
	                                 uint8_t             register_name,
	                                 uint16_t            data);

mb_error_t MBRTU_ReadSingleRegister(uint8_t             slave_addr,
                                    uint8_t             register_name,
                                    uint16_t            data);

#endif
