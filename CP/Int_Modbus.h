#ifndef __INT_MODBUS_H__
#define __INT_MODBUS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "Int_Modbus_Port.h"

// 发送数据宏定义
typedef enum {
    M_FAIL    = -1,
    M_SUCCESS = 0,
} M_STATUS;

typedef enum {
    MODBUS_READ_COILS               = 0x01, /** 读取线圈状态 */
    MODBUS_READ_DISCRETE_INPUTS     = 0x02, /** 读+++++++++取离散输入状态 */
    MODBUS_READ_HOLDING_REGISTERS   = 0x03, /** 读取保持寄存器 */
    MODBUS_READ_INPUT_REGISTERS     = 0x04, /** 读取输入寄存器 */
    MODBUS_WRITE_SINGLE_COIL        = 0x05, /** 写单个线圈 */
    MODBUS_WRITE_SINGLE_REGISTER    = 0x06, /** 写单个保持寄存器 */
    MODBUS_WRITE_MULTIPLE_COILS     = 0x0F, /** 写多个线圈 */
    MODBUS_WRITE_MULTIPLE_REGISTERS = 0x10, /** 写多个保持寄存器 */
} MODBUS_FUNCTION_CODE;

typedef enum {
    MODBUS_ERROR_ILLEGAL_FUNCTION         = 0x01, /** 错误：非法功能 */
    MODBUS_ERROR_ILLEGAL_DATA_ADDRESS     = 0x02, /** 错误：非法数据地址 */
    MODBUS_ERROR_ILLEGAL_DATA_VALUE       = 0x03, /** 错误：非法数据值 */
    MODBUS_ERROR_SLAVE_DEVICE_FAILURE     = 0x04, /** 错误：从机设备故障 */
    MODBUS_ERROR_ACKNOWLEDGE              = 0x05, /** 错误：确认（请求已接收，正在处理） */
    MODBUS_ERROR_SLAVE_DEVICE_BUSY        = 0x06, /** 错误：从机设备忙 */
    MODBUS_ERROR_MEMORY_PARITY_ERROR      = 0x07, /** 错误：存储奇偶校验错误 */
    MODBUS_ERROR_GATEWAY_PATH_UNAVAILABLE = 0x08, /** 错误：不可用网关路径 */
    MODBUS_ERROR_GATEWAY_TARGET_FAILED    = 0x09, /** 错误：网关目标设备响应失败 */
    MODBUS_ERROR_TIMEOUT                  = 0x0A, /** 错误：超时 */
} MODBUS_ERROR_CODE;

M_STATUS Int_Modbus_Read_Coils(uint8_t slave_addr, uint16_t start_register, uint16_t read_length);
M_STATUS Int_Modbus_Read_Discrete_Inputs(uint8_t slave_addr, uint16_t start_register, uint16_t read_length);
M_STATUS Int_Modbus_Read_Holding_Registers(uint8_t slave_addr, uint16_t start_register, uint16_t read_length);
M_STATUS Int_Modbus_Read_Input_Registers(uint8_t slave_addr, uint16_t start_register, uint16_t read_length);
M_STATUS Int_Modbus_Write_Single_Coil(uint8_t slave_addr, uint16_t register, uint16_t value);
M_STATUS Int_Modbus_Write_Single_Register(uint8_t slave_addr, uint16_t register, uint16_t value);
M_STATUS Int_Modbus_Write_Multiple_Coil(uint8_t slave_addr, uint16_t start_register, uint16_t number, uint8_t *data);
M_STATUS Int_Modbus_Write_Multiple_Register(uint8_t slave_addr, uint16_t start_register, uint16_t number, uint16_t *data);

void Int_Modbus_RegisterCallback(MODBUS_FUNCTION_CODE code, void (*callback)(uint8_t *data, uint16_t datalen));
void Int_ModBus_ReadHandle(uint8_t *data, uint8_t dataLen);

#endif /* __INT_MODBUS_H__ */
