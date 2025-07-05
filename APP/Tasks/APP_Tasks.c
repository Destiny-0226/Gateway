#include "APP_Tasks.h"

// MQTT任务
#define MQTT_TASK_NAME       "MQTT_TASK"
#define MQTT_TASK_STACK_SIZE 1024
#define MQTT_TASK_PRIORITY   3
#define MQTT_TASK_CYCLES     (pdMS_TO_TICKS(200))
TaskHandle_t MQTT_Task_Handle;
void Mqtt_Task(void *pvParameters);

// Modbus任务
#define MODBUS_TASK_NAME       "MODBUS_TASK"
#define MODBUS_TASK_STACK_SIZE 1024
#define MODBUS_TASK_PRIORITY   4
TaskHandle_t Modbus_Task_Handle;
void Modbus_Task(void *pvParameters);

void APP_Tasks_Start(void)
{
    log_info(">>>>>>>>>>>>>>>>>>>>>>>>>> Tasks Start <<<<<<<<<<<<<<<<<<<<<<<<<<<");

    APP_Modbus_Init();

    // 创建任务
    xTaskCreate(Mqtt_Task, MQTT_TASK_NAME, MQTT_TASK_STACK_SIZE, NULL, MQTT_TASK_PRIORITY, &MQTT_Task_Handle);
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


// Modbus 任务
void Modbus_Task(void *pvParameters){
    // Modbus 初始化
    Int_Modbus_Init(Modbus_Task_Handle);
}