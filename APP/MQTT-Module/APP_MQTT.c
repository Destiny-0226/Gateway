#include "APP_MQTT.h"

// MQTT 接收消息回调函数
static void App_MessageCallback(void *data, size_t datalen);

void APP_Mqtt_Init(void)
{
    // 注册回调
    Int_Mqtt_RegisterRecvCallback(App_MessageCallback);
    // 初始化MQTT
    Int_Mqtt_Init();
}

void APP_Mqtt_Start(void)
{
    // 轮询MQTT
    Int_Mqtt_Yield();
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

    if (slave_addr == 5) {
        log_info("Device ID:%d", slave_addr);
    } else {
        log_error("Device ID:%d Error!", slave_addr);
        cJSON_Delete(root);
        return;
    }

    // 释放空间 删除结构体
    cJSON_Delete(root);
}