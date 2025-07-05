#include "Int_Modbus.h"

#define MODBUS_MAX_FRAME_SIZE 256
static void (*callbacks[8])(uint8_t *, uint16_t);

static M_STATUS Int_ModBus_SendHandle(uint8_t slave_addr, MODBUS_FUNCTION_CODE function_code, uint8_t *data, uint8_t data_len);
static uint16_t Int_Modbus_CRC(const uint8_t *data, uint16_t length);

/// @brief 01 读线圈
/// @param slave_addr 从机地址
/// @param start_register 起始寄存器
/// @param read_length 读取寄存器的长度
M_STATUS Int_Modbus_Read_Coils(uint8_t slave_addr, uint16_t start_register, uint16_t read_length)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }
    if (read_length == 0 || read_length > 2000) { // 读取长度范围是1-2000
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(read_length >> 8);
    pdata[index++] = (uint8_t)(read_length & 0xFF);

    Int_ModBus_SendHandle(slave_addr, MODBUS_READ_COILS, pdata, index);
    return M_SUCCESS;
}

/// @brief 02 读离散输入状态
/// @param slave_addr 从机地址
/// @param start_register 起始寄存器
/// @param read_length 读取寄存器的长度
M_STATUS Int_Modbus_Read_Discrete_Inputs(uint8_t slave_addr, uint16_t start_register, uint16_t read_length)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }
    if (read_length == 0 || read_length > 2000) { // 读取长度范围是1-2000
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(read_length >> 8);
    pdata[index++] = (uint8_t)(read_length & 0xFF);

    Int_ModBus_SendHandle(slave_addr, MODBUS_READ_DISCRETE_INPUTS, pdata, index);
    return M_SUCCESS;
}

/// @brief 03 读保持寄存器代码
/// @param slave_addr 从机地址
/// @param start_register 起始寄存器
/// @param read_length 读取寄存器的长度
M_STATUS Int_Modbus_Read_Holding_Registers(uint8_t slave_addr, uint16_t start_register, uint16_t read_length)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }
    if (read_length == 0 || read_length > 2000) { // 读取长度范围是1-2000
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(read_length >> 8);
    pdata[index++] = (uint8_t)(read_length & 0xFF);

    Int_ModBus_SendHandle(slave_addr, MODBUS_READ_HOLDING_REGISTERS, pdata, index);
    return M_SUCCESS;
}

/// @brief 04 读输入寄存器代码
/// @param slave_addr 从机地址
/// @param start_register 起始寄存器
/// @param read_length 读取寄存器的长度
M_STATUS Int_Modbus_Read_Input_Registers(uint8_t slave_addr, uint16_t start_register, uint16_t read_length)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }
    if (read_length == 0 || read_length > 2000) { // 读取长度范围是1-2000
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(read_length >> 8);
    pdata[index++] = (uint8_t)(read_length & 0xFF);
    Int_ModBus_SendHandle(slave_addr, MODBUS_READ_INPUT_REGISTERS, pdata, index);
    return M_SUCCESS;
}

/// @brief 05 写单个线圈
/// @param slave_addr 从机地址
/// @param start_register 寄存器
/// @param value 写入的值
M_STATUS Int_Modbus_Write_Single_Coil(uint8_t slave_addr, uint16_t start_register, uint16_t value)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = value ? 0xFF : 0x00;
    pdata[index++] = 0;

    Int_ModBus_SendHandle(slave_addr, MODBUS_WRITE_SINGLE_COIL, pdata, index);
    return M_SUCCESS;
}

/// @brief 06 写单个寄存器
/// @param slave_addr 从机地址
/// @param start_register 寄存器
/// @param read_length 写入的值
M_STATUS Int_Modbus_Write_Single_Register(uint8_t slave_addr, uint16_t start_register, uint16_t value)
{
    uint8_t pdata[8] = {0};
    uint8_t index    = 2;

    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(value >> 8); // 储存读取的寄存器长度
    pdata[index++] = (uint8_t)(value & 0xFF);

    Int_ModBus_SendHandle(slave_addr, MODBUS_WRITE_SINGLE_REGISTER, pdata, index);
    return M_SUCCESS;
}

/// @brief 0F 写多个线圈
/// @param slave_addr 从机地址
/// @param start_register 起始线圈地址
/// @param number 线圈数量
/// @param data 控制模式
M_STATUS Int_Modbus_Write_Multiple_Coil(uint8_t slave_addr, uint16_t start_register, uint16_t number, uint8_t *data)
{
    uint8_t pdata[256] = {0};
    uint8_t index      = 2;
    uint8_t write_len  = (uint8_t)((number + 7) / 8); // 确保向上取整

    if (number == 0 || number > 1968) { // 线圈数量范围是1-1968
        return M_FAIL;
    }
    if (data == NULL) { // 数据指针不能为空
        return M_FAIL;
    }
    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(number >> 8);
    pdata[index++] = (uint8_t)(number & 0xFF);
    pdata[index++] = write_len;

    for (int8_t i = 0; i < write_len; i++) {
        pdata[index++] = data[i];
    }

    Int_ModBus_SendHandle(slave_addr, MODBUS_WRITE_MULTIPLE_COILS, pdata, index);
    return M_SUCCESS;
}

/// @brief 10 写多个寄存器
/// @param slave_addr 从机地址
/// @param start_register 起始寄存器地址
/// @param number 寄存器数量
/// @param write_len 写入几个
/// @param control_mode 控制模式
M_STATUS Int_Modbus_Write_Multiple_Register(uint8_t slave_addr, uint16_t start_register, uint16_t number, uint16_t *data)
{
    uint8_t pdata[256] = {0};
    uint8_t index      = 2;

    if (number == 0 || number > 123) { // 线圈数量范围是1-1968
        return M_FAIL;
    }
    if (data == NULL) { // 数据指针不能为空
        return M_FAIL;
    }
    if (slave_addr == 0 || slave_addr > 247) { // 从机地址范围是1-247
        return M_FAIL;
    }
    if (start_register > 0xFFFF) { // 寄存器地址范围是0x0000-0xFFFF
        return M_FAIL;
    }

    pdata[index++] = (uint8_t)(start_register >> 8);
    pdata[index++] = (uint8_t)(start_register & 0xFF);
    pdata[index++] = (uint8_t)(number >> 8);
    pdata[index++] = (uint8_t)(number & 0xFF);

    pdata[index++] = number * 2;

    for (uint8_t i = 0; i < number; i++) {
        pdata[index++] = (uint8_t)(data[i] >> 8);
        pdata[index++] = (uint8_t)(data[i] & 0xFF);
    }
    Int_ModBus_SendHandle(slave_addr, MODBUS_WRITE_MULTIPLE_REGISTERS, pdata, index);
    return M_SUCCESS;
}

/// @brief 对要发送的数据进行发送
/// @param slave_addr 从机地址
/// @param function_code 功能码
/// @param data 数据
/// @param data_len 数据的长度
/// @param crc crc计算
/// @return
static M_STATUS Int_ModBus_SendHandle(uint8_t slave_addr, MODBUS_FUNCTION_CODE function_code, uint8_t *data, uint8_t data_len)
{

    data[0]      = slave_addr;             // 赋值从设备地址
    data[1]      = (uint8_t)function_code; // 赋值功能码
    uint16_t crc = Int_Modbus_CRC(data, data_len);

    data[data_len++] = crc & 0xff;
    data[data_len++] = crc >> 8 & 0xff;
    Int_Modbus_Send(data, data_len);
    return M_SUCCESS;
}

void Int_Modbus_RegisterCallback(MODBUS_FUNCTION_CODE code, void (*callback)(uint8_t *data, uint16_t datalen))
{
    switch (code) {
        case MODBUS_READ_COILS:
            /* 处理读取线圈状态 */
            callbacks[0] = callback;
            break;
        case MODBUS_READ_DISCRETE_INPUTS:
            /* 处理读取离散输入状态? */
            callbacks[1] = callback;
            break;

        case MODBUS_READ_HOLDING_REGISTERS:
            /* 处理读取保持寄存器? */
            callbacks[2] = callback;
            break;

        case MODBUS_READ_INPUT_REGISTERS:
            /* 处理读取输入寄存器? */
            callbacks[3] = callback;
            break;

        case MODBUS_WRITE_SINGLE_COIL:
            /* 处理写单个线圈 */
            callbacks[4] = callback;
            break;

        case MODBUS_WRITE_SINGLE_REGISTER:
            /* 处理写单个保持寄存器 */
            callbacks[5] = callback;
            break;

        case MODBUS_WRITE_MULTIPLE_COILS:
            /* 处理写多个线圈 */
            callbacks[6] = callback;
            break;

        case MODBUS_WRITE_MULTIPLE_REGISTERS:
            /* 处理写多个保持寄存器 */
            callbacks[7] = callback;
            break;

        default:
            /* 处理未定义的功能码 */
            break;
    }
}

/// @brief 对串口收到的数据进行处理
/// @param data 串口收到的完整数据
/// @param data_len 数据的长度
void Int_ModBus_ReadHandle(uint8_t *data, uint8_t data_len)
{
    log_info("\nreceive data:");
    for (uint8_t i = 0; i < data_len; i++) {
        printf("%#x ", data[i]);
    }
    printf("\n\n");

    // 计算crc,判断数据是否正确
    uint16_t crc = Int_Modbus_CRC(data, data_len - 2);
    if (memcmp(&crc, data + data_len - 2, 2) != 0) {
        log_error("modbus crc error");
        return;
    }
    // 错误响应帧
    if (data[1] & 0x80) {
        uint8_t error_code = data[2]; // 错误码位于数据帧的第3字节

        switch (error_code) {
            case MODBUS_ERROR_ILLEGAL_FUNCTION:
                log_error("modbus error: illegal function (0x01)");
                break;
            case MODBUS_ERROR_ILLEGAL_DATA_ADDRESS:
                log_error("modbus error: illegal data address (0x02)");
                break;
            case MODBUS_ERROR_ILLEGAL_DATA_VALUE:
                log_error("modbus error: illegal data value (0x03)");
                break;
            case MODBUS_ERROR_SLAVE_DEVICE_FAILURE:
                log_error("modbus error: slave device failure (0x04)");
                break;
            case MODBUS_ERROR_ACKNOWLEDGE:
                log_error("modbus error: acknowledge (0x05)");
                break;
            case MODBUS_ERROR_SLAVE_DEVICE_BUSY:
                log_error("modbus error: slave device busy (0x06)");
                break;
            case MODBUS_ERROR_MEMORY_PARITY_ERROR:
                log_error("modbus error: memory parity error (0x07)");
                break;
            case MODBUS_ERROR_GATEWAY_PATH_UNAVAILABLE:
                log_error("modbus error: gateway path unavailable (0x08)");
                break;
            case MODBUS_ERROR_GATEWAY_TARGET_FAILED:
                log_error("modbus error: gateway target failed (0x09)");
                break;
            case MODBUS_ERROR_TIMEOUT:
                log_error("modbus error: timeout (0x0A)");
                break;
            default:
                log_error("modbus error: unknown error code (0x%02X)", error_code);
                break;
        }
        return;
    }

    // 数据正确
    MODBUS_FUNCTION_CODE function_code = (MODBUS_FUNCTION_CODE)data[1];
    switch (function_code) // 根据不同的功能码来调用不同的函数
    {
        case MODBUS_READ_COILS:
            /* 处理读取线圈状态 */
            if (callbacks[0]) {
                callbacks[0](data, data_len);
            }
            break;
        case MODBUS_READ_DISCRETE_INPUTS:
            /* 处理读取离散输入状态 */
            if (callbacks[1]) {
                callbacks[1](data, data_len);
            }
            break;

        case MODBUS_READ_HOLDING_REGISTERS:
            /* 处理读取保持寄存器 */
            if (callbacks[2]) {
                callbacks[2](data, data_len);
            }
            break;

        case MODBUS_READ_INPUT_REGISTERS:
            /* 处理读取输入寄存器 */
            if (callbacks[3]) {
                callbacks[3](data, data_len);
            }
            break;

        case MODBUS_WRITE_SINGLE_COIL:
            /* 处理写单个线圈 */
            if (callbacks[4]) {
                callbacks[4](data, data_len);
            }
            break;

        case MODBUS_WRITE_SINGLE_REGISTER:
            /* 处理写单个保持寄存器 */
            if (callbacks[5]) {
                callbacks[5](data, data_len);
            }

            break;

        case MODBUS_WRITE_MULTIPLE_COILS:
            /* 处理写多个线圈 */
            if (callbacks[6]) {
                callbacks[6](data, data_len);
            }
            break;

        case MODBUS_WRITE_MULTIPLE_REGISTERS:
            /* 处理写多个保持寄存器 */
            if (callbacks[7]) {
                callbacks[7](data, data_len);
            }
            break;

        default:
            /* 处理未定义的功能码 */
            break;
    }
}

/// @brief 485计算crc函数
/// @param data 数据
/// @param length 数据长度
/// @return 0xDDFF, DD为crc的最后一个，FF为crc的前面一个
// temp[7] = Int_Modbus_CRC(temp, 6) >> 8 & 0xff;
// temp[6] = Int_Modbus_CRC(temp, 6) & 0xff;

static uint16_t Int_Modbus_CRC(const uint8_t *data, uint16_t length)
{
    static const uint16_t crc_table[256] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < length; pos++) {
        uint8_t index = (crc ^ data[pos]) & 0xFF;
        crc           = (crc >> 8) ^ crc_table[index];
    }
    return crc;
}

void Int_Modbus_Receive(uint8_t *data, uint16_t len)
{
    // Int_ModBus_ReadHandle(data, len);
}