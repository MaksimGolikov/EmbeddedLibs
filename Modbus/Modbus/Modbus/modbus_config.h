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


#define MODBUS_COMMAND_WRITE_SINGLE_REGISTER     ( 1 )
#define MODBUS_COMMAND_READ_SINGLE_REGISTER      ( 0 )
#define MODBUS_COMMAND_WRITE_MULTY_REGISTER      ( 0 )
#define MODBUS_COMMAND_READ_MULTY_REGISTER       ( 0 )


#endif
