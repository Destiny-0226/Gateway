#include "APP_Tasks.h"

// MQTT任务
#define MQTT_TASK_NAME       "MQTT_TASK"
#define MQTT_TASK_STACK_SIZE 1024
#define MQTT_TASK_PRIORITY   3
#define MQTT_TASK_CYCLES     (pdMS_TO_TICKS(200))
TaskHandle_t MQTT_Task_Handle;
void Mqtt_Task(void *pvParameters);

// // Modbus任务
// #define MODBUS_TASK_NAME       "MODBUS_TASK"
// #define MODBUS_TASK_STACK_SIZE 1024
// #define MODBUS_TASK_PRIORITY   4
// TaskHandle_t Modbus_Task_Handle;
// void Modbus_Task(void *pvParameters);

// CAN 任务
#define CAN_TASK_NAME       "CAN_TASK"
#define CAN_TASK_STACK_SIZE 128
#define CAN_TASK_PRIORITY   4
#define CAN_TASK_CYCLES     (pdMS_TO_TICKS(1000))
TaskHandle_t Can_Task_Handle;
void CAN_Task(void *pvParameters);
void APP_Tasks_Start(void)
{
    log_info(">>>>>>>>>>>>>>>>>>>>>>>>>> Tasks Start <<<<<<<<<<<<<<<<<<<<<<<<<<<");

    // MODBUS 通讯
    // APP_Modbus_Init();
    // MQTT 任务
    // xTaskCreate(Mqtt_Task, MQTT_TASK_NAME, MQTT_TASK_STACK_SIZE, NULL, MQTT_TASK_PRIORITY, &MQTT_Task_Handle);

    // CAN 通讯
    Int_CAN_Init();
    // 创建 CAN 任务
    xTaskCreate(CAN_Task, CAN_TASK_NAME, CAN_TASK_STACK_SIZE, NULL, CAN_TASK_PRIORITY, &Can_Task_Handle);

    // 启用调度器
    vTaskStartScheduler();
}

// MQTT 任务
void Mqtt_Task(void *pvParameters)
{
    // MQTT 初始化
    APP_Mqtt_Init();
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    while (1) {
        // MQTT 启动
        APP_Mqtt_Receive();
        vTaskDelayUntil(&pxPreviousWakeTime, MQTT_TASK_CYCLES);
    }
}
// CAN 任务
void CAN_Task(void *pvParameters)
{
    uint8_t *ID                   = "Destiny";
    uint8_t cmd[8]                = {0};
    cmd[0]                        = 1;
    cmd[1]                        = 1;
    cmd[2]                        = 2002 & 0xff;
    cmd[3]                        = (2002 >> 8) & 0xff;
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    log_info("CAN Task Start\r\n");
    while (1) {

        // Int_CAN_Send(100, ID, 8);
        //  vTaskDelay(1000);
        Int_CAN_Send(20020226, ID, 8);
        vTaskDelayUntil(&pxPreviousWakeTime, CAN_TASK_CYCLES);
    }
}

// // Modbus 任务
// void Modbus_Task(void *pvParameters){
//     // Modbus 初始化
//     Int_Modbus_Init(Modbus_Task_Handle);
// }