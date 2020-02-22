#include "modbus.h"
#include "modbus_config.h"

#include "RTU/modbus_RTU.h"

#include <stdint.h>
#include <stddef.h>


typedef enum{
    MB_MASTER_IDLE,
    MB_MASTER_WAITING_FOR_A_RESPONSE,
}mb_master_mode_t;

#define TOTAL_FRAME_SIZE_AT_MULTI_ANALOG_WRITE(data_size) (data_size + 5) //reg offset(2) + reg quantity(2) + data size(1)




mb_error_t modbus_Init(mb_type_mode_t      type,
                       mb_work_mode_t      mode,
                       uint8_t             id,
                       bus_function       send_buf_func,
                       modbus_definition_t *handler){

    mb_error_t result = MB_ERR_STATUS_UNCORRECT_PARAM;
    if(mode > MB_WORKMDODE_AMOUNT || send_buf_func == NULL ){
        return result;
    }

    (*handler).status               = MB_STATUS_FREE;
    (*handler).typemode             = type;
    (*handler).workmode             = mode;

    (*handler).send_buf_function    = send_buf_func;
    (*handler).id                   = id;


    (*handler).master_mode          = MB_MASTER_IDLE;
    (*handler).response_timeout     = 500; //0.5 s in ms


    switch(type){
        case MB_TYPEMODE_RTU:{

            (*handler).functions.parse_frame   = MBRTU_ParseFrame;
            (*handler).functions.send_response = MBRTU_SendResponse;
            (*handler).functions.send_request  = MBRTU_SendRequest;

            result = MB_ERR_STATUS_SUCCESS;
        }break;
        case MB_TYPEMODE_TCP:{
            result = MB_ERR_STATUS_NOT_SUPPORTED;
        }break;
        case MB_TYPEMODE_ASCII:{
            result = MB_ERR_STATUS_NOT_SUPPORTED;
        }break;
        default:
        break;
    }

    return result;
}



mb_error_t modbus_SendResponse(modbus_definition_t   *handler,
                               mb_command_t          command,
                               uint8_t               *data,
                               uint8_t               data_len,
                               uint16_t              first_reg){
    mb_error_t result = MB_ERR_STATUS_BUS_BUISY;

    return handler->functions.send_response(handler->send_buf_function,
                                            handler->id,
                                            first_reg,
                                            (uint8_t)command,
                                            data,
                                            data_len);

    return result;
}

mb_error_t modbus_ParseFrame(modbus_definition_t  *handler, uint8_t *data, uint16_t data_length){


    if(handler->workmode == MB_WORKMODE_MASTER){
        if(handler->master_mode == MB_MASTER_WAITING_FOR_A_RESPONSE){
            handler->master_mode = MB_MASTER_IDLE;
            handler->status      = MB_STATUS_FREE;
        }else{
            printf("Exit by unknown \n");
            return MB_ERR_STATUS_UNKHOWN_PACKAGE;
        }
    }

    return handler->functions.parse_frame(data, data_length, handler->id,
                                            handler->workmode, handler->last_funct);
}

bool modbus_Run(modbus_definition_t  *handler, uint8_t *frame, uint16_t frame_size, uint32_t last_symb_received_time){
    bool frame_size_must_be_dropped = false;

    if(handler->workmode == MB_WORKMODE_MASTER){
        switch(handler->master_mode){
            case MB_MASTER_IDLE:{

            }break;
            case MB_MASTER_WAITING_FOR_A_RESPONSE:{
                if( IS_TIME_SPENT(handler->request_send_time, handler->response_timeout) ){
                    handler->master_mode = MB_MASTER_IDLE;
                    handler->status      = MB_STATUS_FREE;
                }
            }break;
            default:
                break;
        }
    }

    if(frame_size > 0 && IS_TIME_SPENT(last_symb_received_time, FRAME_TIMEOUT)  ){
        modbus_ParseFrame(handler, frame, frame_size);
        frame_size_must_be_dropped = true;
    }

    return frame_size_must_be_dropped;
}


/************************
 * Master`s request functions
 ***********************/

mb_error_t modbus_MasterRequest_ReadDisret(modbus_definition_t  *handler,
                                           uint16_t             slave_id,
                                           uint16_t             reg_offset,
                                           uint16_t             number_of_reg){
    mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

    if(handler->status == MB_STATUS_FREE){

         uint8_t data[4] = {0};

         /* First two byte of the frame is address of the register [Hi,Lo]*/
         data[0] = reg_offset >> 8;
         data[1] = (reg_offset & 0xFF);

         /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
         data[2] = number_of_reg >> 8;
         data[3] = (number_of_reg & 0xFF);

         status = handler->functions.send_request(handler->send_buf_function,
                                                     slave_id,
                                                  MB_COMMAND_READ_DISCRETE_INPUT,
                                                  data,
                                                  sizeof(data) );
         if(status == MB_ERR_STATUS_SUCCESS){
             handler->status            = MB_STATUS_BUISY;

             handler->last_funct        = MB_COMMAND_READ_DISCRETE_INPUT;
             handler->id                = slave_id;

             handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
             handler->request_send_time = GET_CURRENT_TIME;
         }
    }

    return status;
}

mb_error_t modbus_MasterRequest_ReadHold(modbus_definition_t  *handler,
                                         uint16_t             slave_id,
                                         uint16_t             reg_offset,
                                         uint16_t             number_of_reg){

    mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

    if(handler->status == MB_STATUS_FREE){

             uint8_t data[4] = {0};

             /* First two byte of the frame is address of the register [Hi,Lo]*/
             data[0] = reg_offset >> 8;
             data[1] = (reg_offset & 0xFF);

             /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
             data[2] = number_of_reg >> 8;
             data[3] = (number_of_reg & 0xFF);

             status = handler->functions.send_request(handler->send_buf_function,
                                                      slave_id,
                                                      MB_COMMAND_READ_HOLD_INPUT,
                                                      data,
                                                      sizeof(data) );
             if(status == MB_ERR_STATUS_SUCCESS){
                 handler->status            = MB_STATUS_BUISY;

                 handler->last_funct        = MB_COMMAND_READ_HOLD_INPUT;
                 handler->id                = slave_id;

                 handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
                 handler->request_send_time = GET_CURRENT_TIME;
             }
     }

    return status;
}

mb_error_t modbus_MasterRequest_ReadAnalog(modbus_definition_t  *handler,
                                           uint16_t             slave_id,
                                           uint16_t             reg_offset,
                                           uint16_t             number_of_reg){

    mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

    if(handler->status == MB_STATUS_FREE){

             uint8_t data[4] = {0};

             /* First two byte of the frame is address of the register [Hi,Lo]*/
             data[0] = reg_offset >> 8;
             data[1] = (reg_offset & 0xFF);

             /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
             data[2] = number_of_reg >> 8;
             data[3] = (number_of_reg & 0xFF);

             status = handler->functions.send_request(handler->send_buf_function,
                                                      slave_id,
                                                      MB_COMMAND_READ_ANALOG_INPUT,
                                                      data,
                                                      sizeof(data) );
             if(status == MB_ERR_STATUS_SUCCESS){
                 handler->status            = MB_STATUS_BUISY;

                 handler->last_funct        = MB_COMMAND_READ_ANALOG_INPUT;
                 handler->id                = slave_id;

                 handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
                 handler->request_send_time = GET_CURRENT_TIME;
             }
     }

    return status;
}


mb_error_t modbus_MasterRequest_WriteSingleRegister(modbus_definition_t  *handler,
                                                    uint16_t             slave_id,
                                                    uint16_t             reg_offset,
                                                    uint16_t             value){

    mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

    if(handler->status == MB_STATUS_FREE){

             uint8_t data[4] = {0};

             /* First two byte of the frame is address of the register [Hi,Lo]*/
             data[0] = reg_offset >> 8;
             data[1] = (reg_offset & 0xFF);

             /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
             data[2] = value >> 8;
             data[3] = (value & 0xFF);

             status = handler->functions.send_request(handler->send_buf_function,
                                                      slave_id,
                                                      MB_COMMAND_WRITE_SINGLE_ANALOG,
                                                      data,
                                                      sizeof(data) );
             if(status == MB_ERR_STATUS_SUCCESS){
                 handler->status            = MB_STATUS_BUISY;

                 handler->last_funct        = MB_COMMAND_WRITE_SINGLE_ANALOG;
                 handler->id                = slave_id;

                 handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
                 handler->request_send_time = GET_CURRENT_TIME;
             }
     }

    return status;
}

mb_error_t modbus_MasterRequest_WriteMultiAnalogRegister(modbus_definition_t  *handler,
                                                         uint16_t             slave_id,
                                                         uint16_t             reg_offset,
                                                         uint16_t             quantity,
                                                         uint16_t             *values,
                                                         uint16_t             length_of_value){

    mb_error_t status = MB_ERR_STATUS_BUS_BUISY;

    if(handler->status == MB_STATUS_FREE){

             uint8_t inx       = 0;
             uint8_t data_size = TOTAL_FRAME_SIZE_AT_MULTI_ANALOG_WRITE(length_of_value);
             uint8_t *data     = malloc(data_size);

             if(data == NULL){
                 return MB_ERR_STATUS_FAIL_MEM_ALLOCATED;
             }

             /* First two byte of the frame is address of the register [Hi,Lo]*/
             data[0] = reg_offset >> 8;
             data[1] = (reg_offset & 0xFF);

             /* Second two byte of the frame is number of the register which should be read[Hi,Lo]*/
             data[2] = quantity >> 8;
             data[3] = (quantity & 0xFF);

             /* Count of byte with data*/
             data[4] = length_of_value;

             inx = 5;
             for(uint8_t i = 0; i < length_of_value; i++){
                 data[inx] = values[i] >> 8;      inx++;
                 data[inx] = (values[i] & 0xFF);  inx++;
             }


             status = handler->functions.send_request(handler->send_buf_function,
                                                      slave_id,
                                                      MB_COMMAND_WRITE_MULTI_ANALOG,
                                                      data,
                                                      data_size );
             free(data);

             if(status == MB_ERR_STATUS_SUCCESS){
                 handler->status            = MB_STATUS_BUISY;

                 handler->last_funct        = MB_COMMAND_WRITE_MULTI_ANALOG;
                 handler->id                = slave_id;

                 handler->master_mode       = MB_MASTER_WAITING_FOR_A_RESPONSE;
                 handler->request_send_time = GET_CURRENT_TIME;
             }
     }

    return status;
}
