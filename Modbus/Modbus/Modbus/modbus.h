/*
 * Name        modbus.h
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description Header file with definitions of the Modbus functions in general without bind to realization (RTU, ASCII, TCP/IP)
 */


#ifndef MODBUS_COMMON_FILE_H
#define MODBUS_COMMON_FILE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief List of possible error code of modbus utility
 */
typedef enum{
    MB_ERR_STATUS_SUCCESS,           /**< Ok */

	/**
	 * Modbus protocol error
	 */
	MB_ERR_FUNCTION_NOT_SUPPORTED = 0x01,
	MB_ERR_UNCORRECT_OFFSET       = 0x02,
	MB_ERR_UNCORRECT_VALUE        = 0x03,
	MB_ERR_UNKNOWN_ERROR          = 0x04,
	MB_ERR_LONG_OPERATION         = 0x05,
	MB_ERR_SLAVE_BUSY             = 0x06,
	MB_ERR_SLAVE_OR_ANSWER_ABSENT = 0x11,



    MB_ERR_STATUS_UNCORRECT_PARAM,   /**< Input parameters not as expected*/
    MB_ERR_STATUS_BUS_BUISY,

    MB_ERR_STATUS_CRC_FAIL,          /**< This error code means that CRC of received package
                                          and calculated CRC for received package are not equal*/
    MB_ERR_STATUS_FAIL_MEM_ALLOCATED,/**< This code means that memory for the frame was not allocated*/

    MB_ERR_STATUS_UNKHOWN_PACKAGE,    /**< This error code means that received package was not recognized*/

    MB_ERR_STATUS_NOT_SUPPORTED       /**< Not realized or disabled functionality*/
}mb_error_t;

/**
 * @brief List of possible type of the Modbus mode
 */
typedef enum{
    MB_TYPEMODE_RTU,
    MB_TYPEMODE_ASCII,
	MB_TYPEMODE_TCP

}mb_type_mode_t;

/**
 * @brief List of possible work mode of the Modbus utility
 */
typedef enum{
    MB_WORKMODE_SLAVE,
    MB_WORKMODE_MASTER,

    MB_WORKMDODE_AMOUNT
}mb_work_mode_t;

/**
 * List with supported functions
 */
typedef enum{
    MB_COMMAND_READ_COIL            = 1,
    MB_COMMAND_READ_DISCRETE_INPUT  = 2,
    MB_COMMAND_READ_HOLD_INPUT      = 3,
    MB_COMMAND_READ_ANALOG_INPUT    = 4,

    MB_COMMAND_WRITE_SINGLE_COIL    = 5,
    MB_COMMAND_WRITE_SINGLE_ANALOG  = 6,
    MB_COMMAND_WRITE_MULTI_DISCRETE = 15,
    MB_COMMAND_WRITE_MULTI_ANALOG   = 16,

    MB_COMMAND_AMOUNT
}mb_command_t;


/**
 * @brief Possible status of the Modbus device
 */
typedef enum{
    MB_STATUS_FREE,  /**< This status means that bus can be use*/
    MB_STATUS_BUISY  /**< This status means that bus busy at the moment. For example master is waiting to the response*/
}modbus_status;


/**
 * @brief Definition of the API of a function to sent array  with data
 * @param buf         - The pointer to the data
 * @param buff_length - The length of the data
 * @return The operation status. The function should return 0 at success and 1 or other value at fail
 */
typedef int8_t (*bus_function)(uint8_t *buf, uint16_t buff_length);


/**
 * @brief The definition of functions which are using during Modbus utility working
 */
typedef struct {
    /**
     * @brief The description of the function which will call by library to parse received frame
     *
     * @param data         - Pointer to the data which depends on  the function
     * @param data_length  - The length of the data
     * @param my_dev_id    - The id of devise which received frame (Slave mode);
     *                       Id of the slave from what master is waiting for an answer (Master mode)
     * @param is_it_master - The variable declare is master received frame or slave
     * @param last_funct   - The code of function which was sent the last (Only in Master mode)
     * @return The operation status. If operation success return MB_ERR_STATUS_SUCCESS
     */
    mb_error_t (*parse_frame)(uint8_t *data,
                                uint16_t data_length,
                                uint8_t  my_dev_id,
                                bool     is_it_master,
                                uint8_t  last_funct);

    /**
     * @brief The description of the function which will call by library to send a response
     *
     * @param send            - The pointer to the function to send data (platform realization)
     * @param my_dev_id       - The id of the device which send response
     * @param reg             - This value depends on the function which we answer for
     * @param answer_function - The code of function which was apply
     * @param data            - Pointer to the data which depends on  the function
     * @param data_len        - The length of the data
     * @return  The operation status. If operation success return MB_ERR_STATUS_SUCCESS
     */
    mb_error_t (*send_response)(bus_function send,
                                uint8_t      my_dev_id,
                                uint16_t     reg,
                                uint8_t      answer_function,
                                uint8_t      *data,
                                uint8_t      data_len);

    /**
     * @brief The description of the function which will call by library to send a request
     *
     * @param send      - The pointer to the function to send data (platform realization)
     * @param my_dev_id - The id of the device which send response.
     * @param function  - The code of function which should be apply
     * @param data      - Pointer to the data which depends on  the function
     * @param data_len  - The length of the data
     * @return The operation status. If operation success return MB_ERR_STATUS_SUCCESS
     */
    mb_error_t (*send_request)(  bus_function         send,
                                 uint8_t              my_dev_id,
                                 uint8_t              function,
                                 uint8_t              *data,
                                 uint8_t              data_len);


}mb_functions_t;



typedef struct modbus_definition_t{
    mb_functions_t functions;           /*!< Pointer to the structure with pointers the functions which depend on modbus type*/

    mb_type_mode_t typemode;            /*!< This parameter define what type of Modbus library should use*/
    mb_work_mode_t workmode;            /*!< This parameter define role of the device in communication*/

    modbus_status  status;              /*!< This parameter store current bus status. It need only for master mode*/

    bus_function   send_buf_function;   /*!< The pointer to the function  which will send the frame*/

    uint8_t        master_mode;         /*!<This parameter describes the current mode of Modbus if the device is a master on a bus */
    uint16_t       response_timeout;    /*!< Period of time which master will wait for a response from a slave*/
    uint32_t       request_send_time;   /*!< The time when the frame was sent*/

    uint8_t        id;                  /*!< This parameter describe id of the device if it slave*/
    uint8_t        last_funct;          /*!< This parameter store the number of function which was sent the last.
                                              It needs for checking the answer on the master side*/




}modbus_definition_t;

/**
 * @brief API of the function to initialize Modbus object
 *
 * This function using to configured Modbus object by users parameters
 *
 * @param type    - This parameter declare type of Modbus protocol which should use.
 *                  This parameter should be from the  mb_type_mode_t list.
 * @param mode    - This parameter define role of the device (Master or Slave).
 *                  Value of this parameter should be in the range of the mb_work_mode_t list.
 * @param id      - Identification number of the device if it slave
 *
 * @param handler - Pointer to the Modbus configuration which will filled by this initialization function
 * @return Result of the operation. If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_Init(mb_type_mode_t      type,
                       mb_work_mode_t      mode,
                       uint8_t             id,
                       bus_function        send_buf_func,
                       modbus_definition_t *handler);


/**
 * @brief API of function to send response to the request
 *
 * This function will call specialize function to create a frame with data and send it to the master
 *
 * @param handler   - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param command   - The number of function which should be sent
 * @param data      - The data of the frame
 * @param data_len  - The size of the data
 * @param first_reg - The offset which was used at command
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_SendResponse(modbus_definition_t   *handler,
                               mb_command_t          command,
                               uint8_t               *data,
                               uint8_t               data_len,
                               uint16_t              first_reg);


/**
 * @brief API of the function to parse query
 *
 * This function being use by master and slave to parse received frame.
 * After parsing, if the operation was successful, this function will call the necessary callback
 *
 * @param handler     - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param data        - Pointer to the received frame
 * @param data_length - The size of the received frame
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_ParseFrame(modbus_definition_t  *handler,
                             uint8_t *data,
                             uint16_t data_length);



/**
 * @brief API of function to managing response timeout from a slave
 *
 * @param handler    - The pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param frame      - The pointer to the received frame which should be parsed
 * @param frame_size - The size of the received frame
 *
 * return True if the frame was parsed and count of the received bytes must be dropped, else False
 */
bool modbus_Run(modbus_definition_t  *handler,
                uint8_t              *frame,
                uint16_t             frame_size,
                uint32_t             last_symb_received_time);






/**
 * @brief API of the function to send request to a slave with command 0x02
 *
 * This function can be used only at master mode
 *
 * @param handler       - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param slave_id      - The address of the slave which should receive this message
 * @param reg_offset    - The offset of register which should be use
 * @param number_of_reg - The number of registers which should be read
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_MasterRequest_ReadDisret(modbus_definition_t  *handler,
                                           uint16_t             slave_id,
                                           uint16_t             reg_offset,
                                           uint16_t             number_of_reg);

/**
 * @brief API of the function to send request to a slave with command 0x03
 *
 * This function can be used only at master mode
 *
 * @param handler       - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param slave_id      - The address of the slave which should receive this message
 * @param reg_offset    - The offset of register which should be use
 * @param number_of_reg - The number of registers which should be read
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_MasterRequest_ReadHold(modbus_definition_t  *handler,
                                         uint16_t             slave_id,
                                         uint16_t             reg_offset,
                                         uint16_t             number_of_reg);


/**
 * @brief API of the function to send request to a slave with command 0x04
 *
 * This function can be used only at master mode
 *
 * @param handler       - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param slave_id      - The address of the slave which should receive this message
 * @param reg_offset    - The offset of register which should be use
 * @param number_of_reg - The number of registers which should be read
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_MasterRequest_ReadAnalog(modbus_definition_t  *handler,
                                           uint16_t             slave_id,
                                           uint16_t             reg_offset,
                                           uint16_t             number_of_reg);


/**
 * @brief API of the function to send request to a slave with command 0x06
 *
 * This function can be used only at master mode
 *
 * @param handler       - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param slave_id      - The address of the slave which should receive this message
 * @param reg_offset    - The offset of register which should be use
 * @param value         - The value to have write
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_MasterRequest_WriteSingleRegister(modbus_definition_t  *handler,
                                                    uint16_t             slave_id,
                                                    uint16_t             reg_offset,
                                                    uint16_t             value);


/**
 * @brief API of the function to send request to a slave with command 0x10
 *
 * This function can be used only at master mode
 *
 * @param handler         - Pointer to the Modbus configuration struct to have described the role of the device in communication
 * @param slave_id        - The address of the slave which should receive this message
 * @param reg_offset      - The offset of register which should be use
 * @param quantity        - The number of registers to have write
 * @param values          - The pointer to the array with values which should be written
 * @param length_of_value - The size of the array with data
 *
 * @return If operation was finished with success will return MB_ERR_STATUS_SUCCESS,
 *         else will be return error code from the  mb_error_t list
 */
mb_error_t modbus_MasterRequest_WriteMultiAnalogRegister(modbus_definition_t  *handler,
                                                         uint16_t             slave_id,
                                                         uint16_t             reg_offset,
                                                         uint16_t             quantity,
                                                         uint16_t             *values,
                                                         uint16_t             length_of_value);





#endif
