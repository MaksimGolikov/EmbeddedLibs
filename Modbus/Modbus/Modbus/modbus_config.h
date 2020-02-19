/*
 * Name        modbus_conf.h
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description File for configure functionality of the modbus utility (enable/disable)
 */

#ifndef MODBUS_COMMON_CONFIG_H
#define MODBUS_COMMON_CONFIG_H


/**
 * @brief List of commands which this modbus realization supported.
 *        For enable functionality parameter should be set to the ( 1 )
 *        If functionality should be disabled, definition mast be set to the  ( 0 )
 */


#define MODBUS_COMMAND_READ_DISCRET_INPUT_REGISTER       ( 1 )

#define MODBUS_COMMAND_WRITE_SINGLE_HOLD_REGISTER        ( 1 )



// Callbacks which will be called if function received
#if MODBUS_COMMAND_READ_DISCRET_INPUT_REGISTER
	void ReadDiscretInputClbk(uint16_t first_reg, uint16_t number);
#endif


#if MODBUS_COMMAND_WRITE_SINGLE_HOLD_REGISTER
	void WriteSingleHoldClbk(uint16_t reg, uint16_t value);
#endif




#endif
