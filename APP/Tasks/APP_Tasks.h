#ifndef __APP_TASKS_H__
#define __APP_TASKS_H__

#include "Com_log.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "APP_MQTT.h"
#include "APP_Modbus.h"
#include "Int_CAN.h"

void APP_Tasks_Start(void);

extern SemaphoreHandle_t modbus_msg_semaphore;

#endif /* __APP_TASKS_H__ */