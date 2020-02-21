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
#include 



/**
 * @brief Definition of the function to get the current time of the platform
 */
#define GET_CURRENT_TIME             some_func()

/**
 * @brief Definition to check is the period of time spent
 */
#define IS_TIME_SPENT(start, pause)  ( ( (start + pause) > GET_CURRENT_TIME )?0:1 )





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
 * @brief This definition declare state of Read discrete input function. Code of this function 0x02
 */
#define MODBUS_COMMAND_READ_DISCRET_INPUT_REGISTER       ( 1 ) // Function 0x02


/**
 * @brief This definition declare state of Write hold register function. Code of this function 0x06
 */
#define MODBUS_COMMAND_WRITE_SINGLE_HOLD_REGISTER        ( 0 ) // Function 0x06






#if MODBUS_ROLE_MASTER
    /**
     * @brief API of the callback function, which will call on the master if an error was returned by slave
     *
     * @param err_code - The error code to have received from a slave
     */
	void MasterErrorClbk(uint8_t err_code);

	/**
	 * @brief  API of the callback function, which will call on the master if read function was
	 * @param data - Value which was sent by slave. If slave has sent a few values, callback will call for each value
	 */
	void MasterResnonseClbk(uint16_t data);
#else
	/**
	 * @brief Callbacks which will be called if function received
	 */
	#if MODBUS_COMMAND_READ_DISCRET_INPUT_REGISTER
	    void ReadDiscretInputClbk(uint16_t first_reg, uint16_t number);
	#endif


	#if MODBUS_COMMAND_WRITE_SINGLE_HOLD_REGISTER
	    void WriteSingleHoldClbk(uint16_t reg, uint16_t value);
	#endif

#endif

#endif
