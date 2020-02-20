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

	MB_ERR_STATUS_CRC_FAIL,
    MB_ERR_STATUS_FAIL_MEM_ALLOCATED,

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


typedef enum{
	MB_COMMAND_READ_COIL          = 1,
	MB_COMMAND_READ_DISCRET_INPUT = 2,
	MB_COMMAND_READ_HOLD_INPUT    = 3,
	MB_COMMAND_READ_INPUT         = 4,

	MB_COMMAND_WRITE_SINGLE_COIL  = 5,
	MB_COMMAND_WRITE_SINGLE_HILD  = 6,
	MB_COMMAND_WRITE_MUSTI_COILS  = 15,
	MB_COMMAND_WRITE_MUSTI_HOLDS  = 16,

	MB_COMMAND_AMOUNT
}mb_command_t;



typedef enum{
	MB_STATUS_FREE,
	MB_STATUS_BUISY
}modbus_status;



typedef int8_t (*bus_function)(uint8_t *buf, uint16_t buff_length);



typedef struct {
	mb_error_t (*parse_request)(uint8_t *data,
			                    uint16_t data_length,
								uint8_t my_dev_id);

	mb_error_t (*send_response)(bus_function send,
			                    uint8_t      my_dev_id,
								uint16_t     reg,
								uint8_t      answer_function,
								uint8_t      *data,
			                    uint8_t      data_len);


	mb_error_t (*send_request)(  bus_function         send,
			                     uint8_t              my_dev_id,
							     uint8_t              function,
								 uint8_t              *data,
								 uint8_t              data_len);


}mb_functions_t;



typedef struct modbus_defenition_t{
	mb_functions_t functions;

	mb_type_mode_t typemode;
	mb_work_mode_t workmode;

	modbus_status  status;

	bus_function   send_buf_function;
	bus_function   receive_buf_function;

    uint8_t        master_mode;         /*!<This parameter describes the current mode of Modbus if the device is a master on a bus */
    uint16_t       response_timeout;    /*!< Period of time which master will wait for a response from a slave*/
    uint32_t       request_send_time;   /*!< The time when the frame was sent*/

    uint8_t        id;  /*!< This parameter describe id of the device if it slave*/

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
 * @param id      - Identification number of the device if it slave
 *
 * @param handler - Pointer to the modbus configuration which will filled by this initialization function
 * @return Result of the operation. If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t Modbus_Init(mb_type_mode_t      type,
		               mb_work_mode_t      mode,
					   uint8_t             id,
					   bus_function       send_buf_func,
					   bus_function       reseive_buf_func,
					   modbus_defenition_t *handler);


void       Modbus_OperationDoneClbk(modbus_defenition_t *handler);


mb_error_t Modbus_SendResponse(modbus_defenition_t   *handler,
							   mb_command_t          command,
							   uint8_t               *data,
							   uint8_t               data_len,
							   uint16_t              first_reg);


mb_error_t Modbus_ReadQuery(modbus_defenition_t  *handler,
		                    uint8_t *data,
							uint16_t data_length);

#endif
