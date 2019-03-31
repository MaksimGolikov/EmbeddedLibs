/*
 * Name         FirwareInfo.c
 * Author       Maksim Golikov (SW developer)
 * Created      18 MARCH 2019
 * Description  file for get version of software (source file)
 */





#include "FirmwareInfo.h"
#include <stdlib.h>
#include <string.h>


#define NUMBER_OF_MONTH 12

const char *months[NUMBER_OF_MONTH] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug","Sep", "Oct", "Nov", "Dec"};



static uint8_t majorNumber;
static uint8_t minorNumber;


uint8_t GetInx(uint8_t _startInx, char *_ptrMass){
  uint8_t inx = _startInx;
	if(_ptrMass[inx] == '0'){
	  inx++;
	}
 return inx;
}




void SetVersion(uint8_t _major, uint8_t _minor){
    majorNumber = _major;
	minorNumber = _minor;
}




void GetBuildInfo(VersionInfo *_info){
	char tmp[3]      = {0};	
	
	_info->detail.major = majorNumber;
	_info->detail.minor = minorNumber;
	
	char time_string[] =(__TIME__);
	
 
	_info->detail.hours = atoi(&time_string[GetInx(0, time_string)]);
	_info->detail.min   = atoi(&time_string[GetInx(3, time_string)]);
	_info->detail.sec   = atoi(&time_string[GetInx(6, time_string)]);
		
	char data_string[] =(__DATE__);		
	memcpy(tmp, data_string, 3);
	for(uint8_t month = 0; month < NUMBER_OF_MONTH; month ++){
	   if(!memcmp(tmp, months[month], 3)){
		   _info->detail.month = month + 1;
			  break;
		 }
	}
	
	_info->detail.day  = atoi(&data_string[GetInx(4, data_string)]);
	_info->detail.year = atoi(&data_string[GetInx(9, data_string)]);
}

void DecodeBuildInfo(uint8_t *_ptrVersionMass, VersionInfo* _BuildInfoStruct){  		
	memcpy(_BuildInfoStruct->massive, _ptrVersionMass, TOTAL_DATE_SIZE);
}
