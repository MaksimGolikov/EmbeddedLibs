/*
 * Name        modbus_CRC16.c
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description Source file to calculate CRC value
 */

#include "modbus_CRC16.h"
#include <stdint.h>


uint16_t GetCRC16(uint8_t* buf, uint16_t buflen)
{
    uint8_t         culc_crc_hi = 0xFF;
    uint8_t         culc_crc_lo = 0xFF;
    int32_t         inx         = 0;

    while( buflen-- )
    {
        inx = culc_crc_lo ^ *( buf++ );
        culc_crc_lo = ( uint8_t )( culc_crc_hi ^ crc_part_Hi[inx] );
        culc_crc_hi = crc_part_Lo[inx];
    }
    return ( uint16_t )( culc_crc_hi << 8 | culc_crc_lo );
}
