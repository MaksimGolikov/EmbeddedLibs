/*
 * Name         ul_WorkWithServer.c
 * Author       Maksim Golikov (SW developer)
 * Created      12 FEB 2018
 * Description  utility for working with server (source file)
 */
 
 #include "UL/ul_WorkWithServer.h"
 #include "DRV/w25q32bv.h"          // Flash
 #include "Common/FlashMap.h"

 
 static ServersName_t ServersDescriptionIndex;
 
 static ServerDescription_t possibleService [Servers_Amount] = {
 //      Name              Password           IP             Port
    { "TechSupport",     "2015LT2015",  "192.168.0.101",    55000 },
	{ "Test mode",       "2015LT2015",  "192.168.0.101",    55000 },
	{ "--------",        "0000000000",  "192.168.0.101",    55000 },
	{ "MGO",             "2015LT2015",  "192.168.0.101",    55000 },
	{ "--------",        "0000000000",  "192.168.0.101",    55000 },
	{ "OTK",             "2015LT2015",  "192.168.0.101",    55000 },
	{ "LASERTAG",        "2015LT2015",  "192.168.0.101",    55000 },
	{ "LASERTAG-NET",    "2015LT2015",  "192.168.0.101",    55000 }
 };
 
 
 
 
 Messages_t ul_WorkWithServer_Init(void){
	 Messages_t resalt = Error;	 
	 uint8_t tmp = 0;
	 ServersDescriptionIndex = DEFAULT;
	 
	 sFLASH_ReadBuffer(&tmp, ADDRES_SERVER, 1);
	 if(tmp > 0 && tmp < Servers_Amount){
		 ServersDescriptionIndex = (ServersName_t)tmp;
		 resalt = Ok;
	 }
	 return resalt;	 
 } 
 
 Messages_t ul_WorkWithServer_ChangeServerOn(ServersName_t newServer) {
	Messages_t result = Error;

	if ((newServer > 0 && newServer < Servers_Amount)
			&& newServer != ServersDescriptionIndex) {
		ServersDescriptionIndex = newServer;
		sFLASH_RewriteData(&newServer, ADDRES_SERVER, 1);
		result = Changed;
	}
	return result;
}
 
 
 void ul_WorkWithServer_GetServerParameter(ServerDescription_t *serverDescription){
	 (*serverDescription) = possibleService[ServersDescriptionIndex];
 }
 
