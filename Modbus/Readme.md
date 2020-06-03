# MODBUS library

## Overview

This is a library to work with Modbus protocol.
The current version supports next modes:

* RTU   
* ASCII
    
The current version of the Modbus library supports the next commands:

| Command | Description |
|--|--|
| *0x02* | read discrete input |
| *0x03* | read hold input |
| *0x04* | read analog input |
| *0x06* | write hold register |
| *0x10* | write few analog registers |

You can configure the library to work it both variant *master/slave*.  
Also, this realization supports disabling compiling of the functions for saving space in the device memory.  

All configurations can be done in the [**modbus_config.h**](./modbus_config.h) file. It contains the common settings, so before using the library update configurations in this file.

    
## RTC 

TBD

## ASCII

The current version doesn't support parity for the ASCII frame.  
This version is using **CRLF** as the end of the frame.


## Example


### Master

```c
static uint8_t received_frame [MAX_FRANE_SIZE] = {0};
static uint8_t receive_cntr         = 0;
static uint32_t time_last_receive   = 0;

int8_t send_function (uint8_t *buf, uint16_t buff_length){
	int8_t st = Send(buf, buff_length); // type of the send function deends on bus which going to use
	return ( st == HAL_OK)?0:1;
}


void RX_Callback(uint8_t symb){

	received_frame[receive_cntr] = symb;
	receive_cntr++;
	time_last_receive = GetTick();
}


void modbus_MasterError_cb(uint8_t err_code){
	// communication error handler
}


void modbus_MasterResnonse_cb(uint16_t data, uint16_t bite_inx, uint16_t total_length, bool the_end){
	// parsing responce fro the slave
}


modbus_definition_t modbus;


int main(void){

	modbus_Init(MB_TYPEMODE_RTC,      //MB_TYPEMODE_ASCII  MB_TYPEMODE_RTC
				MB_WORKMODE_MASTER,
				0,
				send_function,
				&modbus);

	while(1){
		if( modbus_Run(&modbus,   received_frame, receive_cntr,  time_last_receive)){
			receive_cntr = 0;
		}
	}
}
```

### Slave

```c
static uint8_t received_frame [MAX_FRANE_SIZE] = {0};
static uint8_t receive_cntr         = 0;
static uint32_t time_last_receive   = 0;

modbus_definition_t modbus;


int8_t send_function (uint8_t *buf, uint16_t buff_length){
	int8_t st = Send(buf, buff_length); // type of the send function deends on bus which going to use
	return ( st == HAL_OK)?0:1;
}


void RX_Callback(uint8_t symb){

	received_frame[receive_cntr] = symb;
	receive_cntr++;
	time_last_receive = GetTick();
}


void modbus_ReadDiscretInput_cb(uint16_t first_reg, uint16_t number){

	uint8_t data[] = {15, 10};

	modbus_SendResponse(&modbus,
						MB_COMMAND_READ_DISCRETE_INPUT,
						data,
						sizeof(data),
						first_reg);
}


int main(void){

	modbus_Init(MB_TYPEMODE_RTC,    //MB_TYPEMODE_ASCII MB_TYPEMODE_RTC
				MB_WORKMODE_SLAVE,
				DEV_ID,             // salve id on the bus     
				send_function,
				&modbus);

	while(1){
		if( modbus_Run(&modbus,   received_frame, receive_cntr,  time_last_receive)){
			receive_cntr = 0;
		}
	}
}

```

## Plans

* Add functions to support other Modbus commands
* Add support for Modbus TCP

