#ifndef MODBUS_COMMON_FILE_H
#define MODBUS_COMMON_FILE_H

#include <stdint.h>

/**
 * @brief List of possible error code of modbus utility
 */
typedef enum{
	MB_ERR_STATUS_SUCCESS,          /**< Ok */
	MB_ERR_STATUS_UNCORRECT_PARAM,  /**< Input parameters not as expected*/
	MB_ERR_STATUS_BUS_BUISY,

	MB_ERR_STATUS_RB_FULL,          /**< Buffer of message is full*/

	MB_ERR_STATUS_NOT_SUPPORTED     /**< Not realized or disabled functionality*/
}mb_error_t;

/**
 * @brief List of possible type of the modbus mode
 */
typedef enum{
	MB_TYPEMODE_RTU,
	MB_TYPEMODE_TCP,

}mb_type_mode_t;

/**
 * @brief List of possible work mode of the modbus utility
 */
typedef enum{
	MB_WORKMODE_MASTER,
	MB_WORKMODE_SLAVE,

	MB_WORKMDODE_AMOUNT
}mb_work_mode_t;


///**
// * @brief Pointer to the modbus object
// */
//typedef struct modbus_defenition_t* modbuf;
typedef enum{
	MB_STATUS_FREE,
	MB_STATUS_BUISY
}modbus_status;

typedef struct modbus_defenition_t{
	mb_type_mode_t typemode;
	mb_work_mode_t workmode;

	modbus_status  status;

    int8_t (*send_buf_function)(uint8_t *send_buf, uint16_t buff_length);        // uart
    int8_t (*receive_buf_function)(uint8_t *receive_buf, uint16_t *buff_length); // uart

	//pointers to the functions
    mb_error_t (*wsr_function)( uint8_t   slave_addr,
                                uint8_t   register_name,
                                uint16_t  data);
    mb_error_t (*rsr_function)( uint8_t   slave_addr,
                                uint8_t   register_name,
                                uint16_t  data);
}modbus_defenition_t;

/**
 * @brief API of the function to initialize modbus object
 *
 * This function using to configured modbus object by users parameters
 *
 * @param type    - This parameter declare type of modbus protocol which should use.
 *                  This parameter should be from the  mb_type_mode_t list.
 * @param mode    - This parameter define role of the device (Master or Slave).
 *                  Value of this parameter should be in the range of the mb_work_mode_t list.
 * @param handler - Pointer to the modbus configuration which will filled by this initialization function
 * @return Result of the operation. If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t Modbus_Init(mb_type_mode_t type,
		               mb_work_mode_t mode,
					   modbus_defenition_t *handler);

void       Modbus_OperationDoneClbk(modbus_defenition_t *handler);


mb_error_t Modbus_WriteSingleRegister(modbus_defenition_t   *handler,
		                              uint8_t  slave_addr,
								      uint8_t  register_name,
								      uint16_t data);

mb_error_t Modbus_ReadSingleRegister(modbus_defenition_t  *handler,
		                             uint8_t  slave_addr,
								     uint8_t  register_name,
								     uint16_t *data);

#endif
