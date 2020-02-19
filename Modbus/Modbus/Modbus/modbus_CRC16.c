/*
 * modbus_CRC16.c
 *
 *  Created on: 19 лют. 2020 р.
 *      Author: MaGol
 */

#include "modbus_CRC16.h"
#include <stdint.h>


uint16_t GetCRC16(uint8_t* buf, uint16_t buflen)
{
	uint8_t         ucCRCHi = 0xFF;
	uint8_t         ucCRCLo = 0xFF;
	int32_t         iIndex  = 0;

	while( buflen-- )
	{
		iIndex = ucCRCLo ^ *( buf++ );
		ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
		ucCRCHi = aucCRCLo[iIndex];
	}
	return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}
