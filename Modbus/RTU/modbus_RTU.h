/*
 * Name        modbus_RTU.h
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description Header file with definitions of the Modbus RTU functions
 */


#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "../modbus.h"
#include <stdint.h>


/**
 * @brief API of function to send response to a request
 *
 * This function creates a frame depends on function number on which answers
 *
 * @param send            - Pointer to the function which will send frame of the data
 * @param my_dev_id       - The id of the device which answer on the request
 * @param first_reg       - Number of the register which was started to read from
 * @param answer_function - The number of function which on answered
 * @param data            - Information, which should be sent. This field depends on the function`s number
 * @param data_len        - Length of the data
 * @return  Will return MB_ERR_STATUS_SUCCESS if data was sent else return error code
 */
mb_error_t MBRTU_SendResponse(  bus_function          send,
                                uint8_t               my_dev_id,
                                uint16_t              first_reg,
                                uint8_t               answer_function,
                                uint8_t               *data,
                                uint8_t               data_len);

/**
 * @brief API of function to parse received data
 *
 * This function will call callback functions depends on request in the frame.
 * Parameters of callback will take from the received function
 *
 * @param data         - Pointer to the received frame
 * @param data_length  - The size of the received frame
 * @param my_dev_id    - Id of the device (is it slave)
 * @param is_it_master - Show is master receive this message or slave.
 *                       Callback which will call in success depends on this parameter.
 *@param last_funct    - This parameters need to check is it error core. Use only at master mode (is_it_master == true)
 *
 * @return Will return MB_ERR_STATUS_SUCCESS if data was sent else return error code
 */
mb_error_t MBRTU_ParseFrame(  uint8_t *data,
                              uint16_t data_length,
                              uint8_t  my_dev_id,
                              bool     is_it_master,
                              uint8_t  last_funct);


/**
 * @brief API of function to send request to the slave
 *
 * This function will use by master to send frame to the slave
 *
 * @param send       - Pointer to the function which will send frame of the data
 * @param my_dev_id  - The id of the device which should take this massage
 * @param function   - The number of the function to have sent
 * @param data       - Pointer to information which will send.
 * @param data_len   - The length of the data
 * @return Will return MB_ERR_STATUS_SUCCESS if data was sent else return error code
 */
mb_error_t MBRTU_SendRequest(  bus_function         send,
                               uint8_t              my_dev_id,
                               uint8_t              function,
                               uint8_t              *data,
                               uint8_t              data_len);

#endif
