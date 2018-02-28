/*
 * Name         ul_WorkWithServer.h
 * Author       Maksim Golikov (SW developer)
 * Created      12 FEB 2018
 * Description  utility for working with server (header file)
 */


#ifndef UL_WORK_WITH_SERVER_H_
#define UL_WORK_WITH_SERVER_H_

#include <stdint.h>

#define ARRAY_SIZE_MAX     20
 
 

typedef enum{
	Error,
	Changed,
	Ok
}Messages_t;


typedef enum{
	DEFAULT,
	TEST,
	TEST1,
	MGO,
	TEST2,
	TEST3,
	LASERTAG,
	LASERTAG_NET,
	Servers_Amount
}ServersName_t;

 
 typedef struct{
	 uint8_t  Name[ARRAY_SIZE_MAX];
	 uint8_t  Password[ARRAY_SIZE_MAX];
	 uint8_t  IP[ARRAY_SIZE_MAX];
	 uint16_t Port;
 }ServerDescription_t;

 
 /**
* @brief function which mast call before use this utility
*/
Messages_t ul_WorkWithServer_Init(void);
 
 
/**
* @brief function for change server
* @param newServer name of necessary server
* @param status of operation
*/
Messages_t ul_WorkWithServer_ChangeServerOn(ServersName_t newServer);

/**
*@brief function for get param of selected server
*@param serverDescription - poinnter to variable wich should contain description of connect
*/
void ul_WorkWithServer_GetServerParameter(ServerDescription_t *serverDescription);


#endif
