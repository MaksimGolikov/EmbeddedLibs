/*
 * Name        modbus_conf.h
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description File for configure functionality of the Modbus utility (enable/disable)
 */

#ifndef MODBUS_COMMON_CONFIG_H
#define MODBUS_COMMON_CONFIG_H

#include <stdint.h>

//Include of library for with definition of the get time function
#include "stm32f4xx_hal.h"



/**
 * @brief Definition of the function to get the current time of the platform
 */
#define MODBUS_GET_CURRENT_TIME             HAL_GetTick()

/**
 * @brief Definition to check is the period of time spent
 */
#define MODBUS_IS_TIME_SPENT(start, pause)  ( ( (start + pause) > MODBUS_GET_CURRENT_TIME )?0:1 )

/**
 *@brief - Definition of the period of time to understand that frame is received completely (only for RTU)
 *         This value should be in ms
 */
#define MODBUS_FRAME_TIMEOUT      10

/**
 * @brief The definition the sequence of the end of the frame for the ASCII frame mode
 */
#define MODBUS_END_FRAME_ASCII    "\n\r"



/**
 * @brief - Definition of the period of time of the master waiting for a response timeout (in ms);
 *          After this time has elapsed the Modbus error callback will be called with timeout error code
 * @param err_code
 */
#define MODBUS_RESPONSE_TIMEOUT   500







/**
 * @brief List of commands which this modbus realization supported.
 *        Each functionality can be enable or disable. You can disable necessary functionality
 *        to save space on the controller
 *        For enable functionality parameter should be set to the ( 1 )
 *        If functionality should be disabled, definition mast be set to the  ( 0 )
 */


/**
 * @brief This definition allows compiling functions of the master
 */
#define MODBUS_ROLE_MASTER                               ( 1 )



/**
 *@brief List of the definitions to enable/disable commands for a slave
 */
#if MODBUS_ROLE_MASTER == 0
/**
 * @brief This definition declare state of Read discrete input function.
 *        Code of this function 0x02
 */
#define MODBUS_COMMAND_READ_DISCRETE_INPUT_REGISTER       ( 0 )

/**
 * @brief This definition declare state of Read hold input function.
 *        Code of this function 0x03
 */
#define MODBUS_COMMAND_READ_HOLD_INPUT_REGISTER           ( 0 )

/**
 * @brief This definition declare state of Read hold input function.
 *        Code of this function 0x04
 */
#define MODBUS_COMMAND_READ_ANALOG_INPUT_REGISTER         ( 0 )





/**
 * @brief This definition declare state of Write hold register function.
 *        Code of this function 0x06
 */
#define MODBUS_COMMAND_WRITE_SINGLE_ANALOG_REGISTER        ( 0 )


/**
 * @brief This definition declare state of Write few analog registers function.
 *        Code of this function 0x10
 */
#define MODBUS_COMMAND_WRITE_MULTI_ANALOG_REGISTER        ( 0 )

#endif












#if MODBUS_ROLE_MASTER
    /**
     * @brief API of the callback function, which will call on the master if an error was returned by slave
     *
     * @param err_code - The error code to have received from a slave
     */
    void modbus_MasterError_cb(uint8_t err_code);

    /**
     * @brief  API of the callback function, which will call on the master if read function was
     * @param data - Value which was sent by slave. If slave has sent a few values, callback will call for each value
     */
    void modbus_MasterResnonse_cb(uint16_t data, uint16_t bite_inx, uint16_t total_length, bool the_end);
#else
    /**
     * @brief Callbacks which will be called if function received
     */
    #if MODBUS_COMMAND_READ_DISCRETE_INPUT_REGISTER
        /**
         * @brief API of the callback function which will call on slave side
         * @param fisrt_reg  - The offset of the target register
         * @param number     - The  quantity of the register to have read
         */
        void modbus_ReadDiscretInput_cb(uint16_t first_reg, uint16_t number);
    #endif

    #if MODBUS_COMMAND_READ_HOLD_INPUT_REGISTER
        /**
         * @brief API of the callback function which will call on slave side
         * @param fisrt_reg  - The offset of the target register
         * @param number     - The  quantity of the register to have read
         */
       void modbus_ReadHold_cb(uint16_t first_reg, uint16_t number);
    #endif

    #if MODBUS_COMMAND_READ_ANALOG_INPUT_REGISTER
       /**
        * @brief API of the callback function which will call on slave side
        * @param fisrt_reg  - The offset of the target register
        * @param number     - The  quantity of the register to have read
        */
       void modbus_ReadAnalog_cb(uint16_t first_reg, uint16_t number);
    #endif




    #if MODBUS_COMMAND_WRITE_SINGLE_ANALOG_REGISTER
       /**
        * @brief API of the callback function which will call on slave side
        * @param reg    - The offset of the target register
        * @param value  - The value to have write
        */
        void modbus_WriteSingleAnalog_cb(uint16_t reg, uint16_t value);
    #endif

    #if MODBUS_COMMAND_WRITE_MULTI_ANALOG_REGISTER
        /**
        * @brief API of the callback function which will call on slave side
        * @param reg       - The offset of the target register
        * @param quantity  - The number of register to write
        * @param values    - The pointer to the memory which contains the values to write
        */
        void modbus_WriteMultiAnalog_cb(uint16_t reg, uint16_t quantity, uint16_t *values);
    #endif



#endif

#endif
