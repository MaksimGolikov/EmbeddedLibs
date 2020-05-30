/*
 * Name        modbus_ASCII.c
 * Author      Maksim Holikov (golikov.mo@gmail.com)
 * Created on: Feb 17, 2020
 * Description Source file with definitions of the Modbus ASCII functions
 */

#include <ASCII/modbus_ASCII.h>
#include "../modbus_config.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MB_ASCII_CHAR_CR         '\n'
#define MB_ASCII_CHAR_LF         '\r'
#define MB_ASCII_CHAR_START      ':'     /*!< Default start character for Modbus ASCII. */

#define MB_ASCII_COMMON_ADDRESS  0x00    /*!< The address which each slave should receive.
                                              This address is using to send message for all slaves on the bus */


#define SIZE_OF_REQUEST_BUF(data_len)            ( (data_len * 2) + 9) //(9) Start(1) + Addr(2) + Func(2) + LRC(2) + END(2)

#define GET_AS_ASCII_CHAR(val)                   ( ((val) < 10) ? ((val) + 0x30) : (((val) % 10) + 0x41) )
#define GET_AS_UINT_NUMBER(val)                  ( ((val) < 0x40) ? ((val) - 0x30) : ( 10 + ((val) - 0x41))  )


#define RECEIVED_P_ADDRESS(data)                 (&data[1])
#define RECEIVED_P_FUNCTION(data)                (&data[3])
#define RECEIVED_P_DATA(data)                    (&data[5])
#define RECEIVED_P_LRC(data, data_size)          (&data[data_size - 4])
#define RECEIVED_P_RESPONSE_NUM_BYTES(data)      (&data[5])
#define RECEIVED_P_RESPONSE_DATA(data, inx)      (&data[5 + (inx * 2)])
#define RECEIVED_P_RESPONSE_ERROR(data)          (RECEIVED_P_RESPONSE_DATA(data, 0))




static inline uint8_t get_byte(uint8_t *hi_part){
    uint8_t chars[3] = {0};

    if(hi_part != NULL){
        chars[0] =  GET_AS_UINT_NUMBER((*hi_part));
        hi_part ++;
        chars[1] =  GET_AS_UINT_NUMBER((*hi_part));
        chars[2] =  (chars[0] << 4) | ( chars[1]);
    }
    return chars[2];
}

static uint8_t culculate_LRC( uint8_t *buf, uint16_t buflen )
{
    uint8_t culc_LRC = 0;
    while( buflen > 0 )
    {
        culc_LRC += get_byte(buf);
        buf    +=2;
        buflen -=2;
    }

    culc_LRC = ((0xFF - culc_LRC) + 1);
    return culc_LRC;
}

static void MBASCII_SendErrorCode(uint8_t err_code, uint8_t *data, uint16_t size){
//  for(uint8_t i = 0; i < size; i++){
//      data[i] |= 0x80;
//  }
//  send(data, size);
}


mb_error_t MBASCII_SendRequest(  bus_function         send,
                                 uint8_t              my_dev_id,
                                 uint8_t              function,
                                 uint8_t              *data,
                                 uint8_t              data_len){


   mb_error_t result      = MB_ERR_STATUS_SUCCESS;
   uint16_t buf_total_len = 0;
   uint8_t  *send_buf     = NULL;

   buf_total_len = SIZE_OF_REQUEST_BUF(data_len);
   send_buf      = (uint8_t*)malloc(buf_total_len);

   if(send_buf == NULL){
       result = MB_ERR_STATUS_FAIL_MEM_ALLOCATED;
   }else{
       uint8_t inx               = 0;
       uint8_t chars_of_value[2] = {0};


       send_buf[inx] = MB_ASCII_CHAR_START; inx ++;

       chars_of_value[0] = (my_dev_id / 16);
       chars_of_value[1] = (my_dev_id % 16);

       send_buf[inx] = GET_AS_ASCII_CHAR(chars_of_value[0]); inx ++;
       send_buf[inx] = GET_AS_ASCII_CHAR(chars_of_value[1]); inx ++;

       memset(chars_of_value, 0, sizeof(chars_of_value));
       chars_of_value[0] = (function / 16);
       chars_of_value[1] = (function % 16);

       send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[0]); inx ++;
       send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[1]); inx ++;

       while(data_len--){

           memset(chars_of_value, 0, sizeof(chars_of_value));

           chars_of_value[0] = ((*data) / 16);
           chars_of_value[1] = ((*data) % 16);

           send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[0]); inx ++;
           send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[1]); inx ++;

           data++;
       }

       uint8_t culculated_lrc = culculate_LRC(&send_buf[1], (buf_total_len - 5));

       memset(chars_of_value, 0, sizeof(chars_of_value));

       chars_of_value[0] = (culculated_lrc / 16);
       chars_of_value[1] = (culculated_lrc % 16);

       send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[0]); inx++;
       send_buf[inx] =  GET_AS_ASCII_CHAR(chars_of_value[1]); inx++;

       send_buf[inx] = MB_ASCII_CHAR_LF; inx++;
       send_buf[inx] = MB_ASCII_CHAR_CR;

       send(send_buf, buf_total_len);
       result = MB_ERR_STATUS_SUCCESS;
       free(send_buf);
   }
    return result;
}





mb_error_t MBASCII_ParseFrame (uint8_t *data,
                               uint16_t data_length,
                               uint8_t  my_dev_id,
                               bool     is_it_master,
                               uint8_t  last_function){

    mb_error_t result = MB_ERR_STATUS_UNCORRECT_PARAM;


    if(data != NULL){
        result           = MB_ERR_STATUS_CRC_FAIL;
        uint8_t cul_lrc  = culculate_LRC(&data[1],  (data_length - 5) );
        uint8_t read_lrc = get_byte(RECEIVED_P_LRC(data, data_length));

        if(cul_lrc == read_lrc){
            uint8_t received_address = get_byte(RECEIVED_P_ADDRESS(data));

            if((received_address == my_dev_id) ||
               (received_address == MB_ASCII_COMMON_ADDRESS)){

                if(is_it_master){
                    #if MODBUS_ROLE_MASTER
                    uint8_t received_finction = get_byte(RECEIVED_P_FUNCTION(data));

                    if(received_finction == last_function){
                        uint16_t count_reg  = get_byte(RECEIVED_P_RESPONSE_NUM_BYTES(data));

                        uint16_t read_length = 0;
                        while(count_reg){

                            uint16_t read = get_byte(RECEIVED_P_RESPONSE_DATA(data, read_length));
                            count_reg --;
                            read_length++;

                            modbus_MasterResnonse_cb(read,read_length, count_reg, ((count_reg == 0)?1:0) );
                        }
                    }else{
                      modbus_MasterError_cb(RECEIVED_P_RESPONSE_ERROR(data));
                    }
                    #endif
                }else{

                }
            }
        }
    }




//    if(data != NULL){
//        uint16_t cul_crc  = GetCRC16(data,  (data_length - 2) );
//        uint16_t read_crc = (data[data_length - 2] << 8) | data[data_length - 1];
//        result            = MB_ERR_STATUS_CRC_FAIL;
//
//        if(cul_crc == read_crc){
//            if(my_dev_id == RECEIVER_ID(data) ){
//                if(is_it_master){
//                    #if MODBUS_ROLE_MASTER
//
//                    if(RECEIVER_FUNCTION(data) == last_function){
//                        uint16_t count_reg  = RECEIVER_NUMBER_BYTES(data);
//
//                        uint8_t inx = RECEIVER_FIRST_DATA_BYTE_INX;
//                        uint16_t read_length = 0;
//                        while(count_reg){
//
//                            uint16_t read = 0;
//                            switch(RECEIVER_FUNCTION(data)){
//                                case MB_COMMAND_READ_HOLD_INPUT:
//                                case MB_COMMAND_READ_ANALOG_INPUT:
//                                case MB_COMMAND_WRITE_SINGLE_ANALOG:
//                                case MB_COMMAND_WRITE_MULTI_ANALOG:
//                                   read = (data[inx] << 8) | data[inx++];
//                                break;
//                                case MB_COMMAND_READ_COIL:
//                                case MB_COMMAND_READ_DISCRETE_INPUT:
//                                case MB_COMMAND_WRITE_SINGLE_COIL:
//                                case MB_COMMAND_WRITE_MULTI_DISCRETE:
//                                    read = data[inx];
//                                break;
//                                default:
//                                    break;
//                            }
//                            count_reg --;
//                            read_length++;
//
//                            modbus_MasterResnonse_cb(read,read_length, count_reg, ((count_reg == 0)?1:0) );
//                            inx ++;
//                        }
//                    }else{
//                      modbus_MasterError_cb(RECEIVER_ERROR_CODE(data));
//                    }
//
//                    #endif
//                }else{
//                    uint16_t offset_reg = (data[2] << 8) | data[3];
//                    uint16_t numb_reg  = (data[4] << 8) | data[5];
//
//                    switch(RECEIVER_FUNCTION(data)){
//
//                      #if MODBUS_COMMAND_READ_DISCRETE_INPUT_REGISTER
//                        case MB_COMMAND_READ_DISCRETE_INPUT:{
//                            modbus_ReadDiscretInput_cb(offset_reg, numb_reg);
//                            result = MB_ERR_STATUS_SUCCESS;
//                        }break;
//                      #endif
//                      #if MODBUS_COMMAND_READ_HOLD_INPUT_REGISTER
//                        case MB_COMMAND_READ_HOLD_INPUT:{
//                            modbus_ReadHold_cb(offset_reg, numb_reg);
//                            result = MB_ERR_STATUS_SUCCESS;
//                        }break;
//                      #endif
//                      #if MODBUS_COMMAND_READ_ANALOG_INPUT_REGISTER
//                        case MB_COMMAND_READ_ANALOG_INPUT:{
//                            modbus_ReadAnalog_cb(offset_reg, numb_reg);
//                            result = MB_ERR_STATUS_SUCCESS;
//                        }break;
//                      #endif
//
//                      #if MODBUS_COMMAND_WRITE_SINGLE_ANALOG_REGISTER
//                        case MB_COMMAND_WRITE_SINGLE_ANALOG:{
//                            modbus_WriteSingleAnalog_cb(offset_reg, numb_reg);
//                            result = MB_ERR_STATUS_SUCCESS;
//                        }break;
//                      #endif
//
//                      #if MODBUS_COMMAND_WRITE_MULTI_ANALOG_REGISTER
//                        case MB_COMMAND_WRITE_MULTI_ANALOG:{
//                            uint8_t count_values_to_write = RECEIVER_NUMBER_DATA_BYTE_MULTI(data) >> 1; // devived on 2
//
//                            result                     = MB_ERR_STATUS_FAIL_MEM_ALLOCATED;
//                            uint8_t inx                = RECEIVER_FIRST_VAL_TO_WRITE_MULTI;
//
//                            uint16_t *values_for_write = (uint16_t*)malloc(count_values_to_write);
//
//                            if(values_for_write != NULL){
//                                result = MB_ERR_STATUS_SUCCESS;
//
//                                for(uint8_t i = 0; i < count_values_to_write; i++){
//                                    values_for_write[i]  = (data[inx] << 8); inx++;
//                                    values_for_write[i] |= data[inx];        inx++;
//                                }
//                                modbus_WriteMultiAnalog_cb(offset_reg, numb_reg, values_for_write);
//                            }
//
//                            free(values_for_write);
//                        }break;
//                      #endif
//
//                        default:
//                            break;
//                    }
//                }
//            }
//        }
//    }

    return result;
}













mb_error_t MBASCII_SendResponse(  bus_function         send,
                                  uint8_t              my_dev_id,
                                  uint16_t             first_reg,
                                  uint8_t              answer_function,
                                  uint8_t              *data,
                                  uint8_t              data_len){
    mb_error_t result      = MB_ERR_STATUS_SUCCESS;
    uint16_t buf_total_len = 0;
    uint8_t *send_buff     = NULL;




//    uint16_t buf_total_len = SIZE_OF_RESPONSE_BUF(data_len);
//
//    uint8_t *send_buff = (uint8_t*)malloc(buf_total_len);
//
//    uint8_t inx = 0;
//    if(send_buff != NULL){
//        send_buff[inx] = my_dev_id;         inx ++;
//        send_buff[inx] = answer_function;   inx ++;
//
//        switch(answer_function){
//          default:
//          case MB_COMMAND_READ_COIL:
//          case MB_COMMAND_READ_DISCRETE_INPUT:
//          case MB_COMMAND_READ_HOLD_INPUT:
//          case MB_COMMAND_READ_ANALOG_INPUT:
//              send_buff[inx] = data_len;
//            break;
//          case MB_COMMAND_WRITE_SINGLE_COIL:
//          case MB_COMMAND_WRITE_SINGLE_ANALOG:
//          case MB_COMMAND_WRITE_MULTI_DISCRETE:
//          case MB_COMMAND_WRITE_MULTI_ANALOG:
//              send_buff[inx] = first_reg >> 8;
//              inx++;
//              send_buff[inx] = first_reg & 0xFF;
//              break;
//
//        }
//        inx ++;
//
//        while(data_len--){
//            send_buff[inx] = *data;
//            data++;
//            inx ++;
//        }
//
//        uint8_t crc = GetLRC(send_buff,  buf_total_len - 2 );
//
//        send_buff[inx] = (crc >> 8); inx++;
//        send_buff[inx] = (crc & 0x00FF);
//
//        send(send_buff, buf_total_len);
//        result = MB_ERR_STATUS_SUCCESS;
//        free(send_buff);
//    }
    return result;
}










//============

//==========







//
//#define RECEIVER_ID(data)                          (data[0])
//#define RECEIVER_FUNCTION(data)                    (data[1])
//#define RECEIVER_ERROR_CODE(data)                  (data[2])
//#define RECEIVER_NUMBER_BYTES(data)                (data[2])
//#define RECEIVER_NUMBER_DATA_BYTE_MULTI(data)      (data[6])
//
//
//#define RECEIVER_FIRST_DATA_BYTE_INX               3
//#define RECEIVER_FIRST_VAL_TO_WRITE_MULTI          7
//
//
//
//
//
//





