/*
 * Name         FirwareInfo.h
 * Author       Maksim Golikov (SW developer)
 * Created      18 MARCH 2019
 * Description  file for get version of software (header file)
 */
 
#ifndef FIRMWARE_INFO_H
#define FIRMWARE_INFO_H


#include <stdint.h>

#define  TOTAL_DATE_SIZE   6


typedef union{
	#pragma pack(push,1)
	struct{
		uint8_t major;
		uint8_t minor;
		
		uint32_t year  :6;
		uint32_t month :4;
		uint32_t day   :5;
		
		uint32_t hours :5;
		uint32_t min   :6;
		uint32_t sec   :6;
	}detail;
	#pragma pack(pop)
	
	uint8_t massive[TOTAL_DATE_SIZE];
	}VersionInfo;

/**
*@brief function for define major and minor number of version
*@param major - major number f version
*@param minor - minor number f version
*/
void SetVersion(uint8_t _major, uint8_t _minor);

/**
*@brief  function for get version of software
*@return information structure which contain major, minor numbers data and time of build 
*/
void GetBuildInfo(VersionInfo *_buildInfoStruct);


/**
*@brief  function for decode version from massive
*@return information structure which contain major, minor numbers data and time of build 
*/
void DecodeBuildInfo(uint8_t *_ptrVersionMass, VersionInfo* _BuildInfoStruct);


#endif //FIRMWARE_INFO_H
