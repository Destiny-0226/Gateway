#include "APP_Modbus.h"

static void APP_Modbus_Callback(uint8_t *data, uint16_t datalen);
void APP_Modbus_Init(void)
{
    // 注册回调函数
    Int_Modbus_RegisterCallback(MODBUS_READ_COILS, APP_Modbus_Callback);             // 读取线圈
    Int_Modbus_RegisterCallback(MODBUS_READ_DISCRETE_INPUTS, APP_Modbus_Callback);   // 读取离散输入
    Int_Modbus_RegisterCallback(MODBUS_READ_HOLDING_REGISTERS, APP_Modbus_Callback); // 读取保持寄存器
    Int_Modbus_RegisterCallback(MODBUS_READ_INPUT_REGISTERS, APP_Modbus_Callback);   // 读取输入寄存器

    // 初始化Modbus模块
    Int_Modbus_Init();
}

// 定义常用 JSON 键名常量
#define JSON_KEY_CONNECTION_TYPE   "connection_type"
#define JSON_KEY_DEVICE_ID         "device_id"
#define JSON_KEY_MOTOR_STATUS      "motor_status"
#define JSON_KEY_TARGET_DIRECTION  "target_direction"
#define JSON_KEY_CURRENT_DIRECTION "current_direction"
#define JSON_KEY_CURRENT_SPEED     "current_speed"
#define JSON_KEY_TARGET_SPEED      "target_speed"

uint8_t num = 0;
static void APP_Modbus_Callback(uint8_t *data, uint16_t datalen)
{
    static cJSON *obj = NULL;

    if (data == NULL || datalen < 2) {
        log_error("Invalid data or length too small.");
        return;
    }

    if (obj == NULL) {
        obj = cJSON_CreateObject();
        if (obj == NULL) {
            log_error("Failed to create JSON object.");
            return;
        }
        log_info("receive device_id: %d", data[0]);
        // 添加固定字段
        cJSON_AddStringToObject(obj, JSON_KEY_CONNECTION_TYPE, "rs485");
        cJSON_AddNumberToObject(obj, JSON_KEY_DEVICE_ID, data[0]); // 设备地址为MODBUS响应帧的第1字节
    }

    // 解析MODBUS响应帧的功能码
    uint8_t function_code = data[1];

    // 根据功能码处理数据
    switch (function_code) {
        case MODBUS_READ_COILS: {
            if (datalen >= 4) {
                uint8_t input_data = data[3];
                if (num == 0) {
                    num++;
                    log_info("get motor_status");
                    uint8_t motor_status = (input_data & 0x01) ? 1 : 0;
                    log_info("%d", motor_status);
                    cJSON_AddBoolToObject(obj, JSON_KEY_MOTOR_STATUS, motor_status);
                    // cJSON_AddNumberToObject(obj, JSON_KEY_MOTOR_STATUS, motor_status);

                } else if (num == 1) {
                    num = 0;
                    log_info("get target_direction");
                    uint8_t target_direction = (input_data & 0x01) ? 1 : 0;
                    log_info("%d", target_direction);
                    cJSON_AddBoolToObject(obj, JSON_KEY_TARGET_DIRECTION, target_direction);
                    // cJSON_AddNumberToObject(obj, JSON_KEY_TARGET_DIRECTION, target_direction);
                }
            }
            break;
        }

        case MODBUS_READ_DISCRETE_INPUTS: {
            if (datalen >= 4) {
                log_info("get current_direction");
                uint8_t input_data        = data[3];
                uint8_t current_direction = (input_data & 0x01) != 0;
                log_info("%d", current_direction);
                cJSON_AddBoolToObject(obj, JSON_KEY_CURRENT_DIRECTION, current_direction);
                // cJSON_AddNumberToObject(obj, JSON_KEY_CURRENT_DIRECTION, current_direction);
            }
            break;
        }

        case MODBUS_READ_INPUT_REGISTERS: {
            if (datalen >= 5) {
                log_info("get current_speed");
                uint16_t current_speed = (data[3] << 8) | data[4];
                log_info("%d", current_speed);
                cJSON_AddNumberToObject(obj, JSON_KEY_CURRENT_SPEED, current_speed);
            }
            break;
        }

        case MODBUS_READ_HOLDING_REGISTERS: {
            if (datalen >= 5) {
                log_info("get target_speed");
                uint16_t target_speed = (data[3] << 8) | data[4];
                log_info("%d", target_speed);
                cJSON_AddNumberToObject(obj, JSON_KEY_TARGET_SPEED, target_speed);
            }
            break;
        }

        default:
            log_warn("Unsupported function code: %02X", function_code);
            break;
    }

    // 检查是否所有字段都已填充
    if (cJSON_HasObjectItem(obj, JSON_KEY_MOTOR_STATUS) &&
        cJSON_HasObjectItem(obj, JSON_KEY_TARGET_DIRECTION) &&
        cJSON_HasObjectItem(obj, JSON_KEY_CURRENT_DIRECTION) &&
        cJSON_HasObjectItem(obj, JSON_KEY_CURRENT_SPEED) &&
        cJSON_HasObjectItem(obj, JSON_KEY_TARGET_SPEED)) {
        log_info("All fields filled, sending data.");
        // 将JSON对象转换为字符串
        char *json_str = cJSON_Print(obj);
        cJSON_DeleteItemFromObject(obj, JSON_KEY_MOTOR_STATUS);
        cJSON_DeleteItemFromObject(obj, JSON_KEY_TARGET_DIRECTION);
        cJSON_DeleteItemFromObject(obj, JSON_KEY_CURRENT_DIRECTION);
        cJSON_DeleteItemFromObject(obj, JSON_KEY_CURRENT_SPEED);
        cJSON_DeleteItemFromObject(obj, JSON_KEY_TARGET_SPEED);

        if (json_str != NULL) {
            log_info("json_str: %s", json_str);
            APP_Mqtt_Send((uint8_t *)json_str);
            cJSON_free(json_str);
        } else {
            log_error("Failed to convert JSON to string.");
        }
    }
}