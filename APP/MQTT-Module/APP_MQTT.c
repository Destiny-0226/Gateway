#include "APP_MQTT.h"

uint8_t read_index = 0;

// MQTT 接收消息回调函数
static void App_MessageCallback(void *data, size_t datalen);

void APP_Mqtt_Init(void)
{
    // 注册回调
    Int_Mqtt_RegisterRecvCallback(App_MessageCallback);
    // 初始化MQTT
    Int_Mqtt_Init();
}

void APP_Mqtt_Receive(void)
{
    Int_Mqtt_Yield();
}
void APP_Mqtt_Send(uint8_t *data)
{
    Int_Mqtt_SendData(data);
}

static void App_MessageCallback(void *data, size_t datalen)
{
    // 解析JSON数据
    cJSON *root = cJSON_ParseWithLength((char *)data, datalen);
    if (root == NULL) {
        log_error("Error parsing JSON string.");
        cJSON_Delete(root);
        return;
    }

    // 提取必填字段
    cJSON *connection_type = cJSON_GetObjectItem(root, "connection_type");
    cJSON *device_id       = cJSON_GetObjectItem(root, "device_id");

    // 检查必填字段是否存在
    if (!cJSON_IsString(connection_type) || !cJSON_IsNumber(device_id)) {
        log_error("Invalid JSON format: missing required fields.");
        cJSON_Delete(root);
        return;
    }

    // 检查连接类型是否为RS485
    if (strcmp(connection_type->valuestring, "rs485") != 0) {
        log_error("Unsupported connection type: %s", connection_type->valuestring);
        cJSON_Delete(root);
        return;
    }
    log_info("Connection type: %s", connection_type->valuestring);

    // 获取从设备地址
    uint8_t slave_addr = (uint8_t)device_id->valueint;

    if (slave_addr == SLAVE_ADDR) {
        log_info("Device ID:%d", slave_addr);
    } else {
        log_error("Device ID:%d Error!", slave_addr);
        cJSON_Delete(root);
        return;
    }

    // 检查是否为查询请求
    cJSON *motor_status     = cJSON_GetObjectItem(root, "motor_status");
    cJSON *target_direction = cJSON_GetObjectItem(root, "target_direction");
    cJSON *target_speed     = cJSON_GetObjectItem(root, "target_speed");

    M_STATUS status;

    // 写入线圈状态（电机启停）
    if (cJSON_IsBool(motor_status)) {
        uint16_t value = motor_status->valueint ? 1 : 0; // 0: 停止，1: 启动
        status         = Int_Modbus_Write_Single_Coil(slave_addr, 2, value);
        if (status != M_SUCCESS) {
            log_error("Failed to write coil status.");
        }
    }

    // 写入线圈状态（目标方向）
    if (cJSON_IsBool(target_direction)) {
        uint16_t value = target_direction->valueint ? 1 : 0; // 1: 正转 0: 反转
        status         = Int_Modbus_Write_Single_Coil(slave_addr, 3, value);
        if (status != M_SUCCESS) {
            log_error("Failed to write coil status for target direction.");
        }
    }

    // 写入保持寄存器（目标转速）
    if (cJSON_IsNumber(target_speed)) {
        uint16_t value = (uint16_t)target_speed->valueint; // 速度
        status         = Int_Modbus_Write_Single_Register(slave_addr, 2, value);
        if (status != M_SUCCESS) {
            log_error("Failed to write holding register for target speed.");
        }
    }

    // 设置完成后，查询所有寄存器状态 ，总共5个
    // 查询电机启停
    read_index = 1;
    status     = Int_Modbus_Read_Coils(slave_addr, 2, 1);
    if (status != M_SUCCESS) {
        log_error("Failed to read coil status.");
    }

    // 查询目标方向
    read_index = 2;
    status     = Int_Modbus_Read_Coils(slave_addr, 3, 1);
    if (status != M_SUCCESS) {
        log_error("Failed to read coil status.");
    }

    // 查询当前方向
    read_index = 3;
    status     = Int_Modbus_Read_Discrete_Inputs(slave_addr, 3, 1);
    if (status != M_SUCCESS) {
        log_error("Failed to read discrete input status.");
    }

    // 查询当前转速
    read_index = 4;
    status     = Int_Modbus_Read_Input_Registers(slave_addr, 2, 1);
    if (status != M_SUCCESS) {
        log_error("Failed to read input register.");
    }

    // 查询目标转速
    read_index = 5;
    status     = Int_Modbus_Read_Holding_Registers(slave_addr, 2, 1);
    if (status != M_SUCCESS) {
        log_error("Failed to read holding register.");
    }

    read_index = 0;
    // 释放空间 删除结构体
    cJSON_Delete(root);
}