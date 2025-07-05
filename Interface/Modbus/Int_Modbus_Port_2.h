#ifndef __INT_MODBUS_PORT_H__
#define __INT_MODBUS_PORT_H__

#include "Int_Modbus.h"
#include "Com_log.h"
#include "main.h"
#include "usart.h"
#include <string.h>

#include "APP_Tasks.h"

void Int_Modbus_Init(TaskHandle_t modbus_task_handle);
void Int_Modbus_Send(uint8_t *data, uint16_t len);

#endif /* __INT_MODBUS_PORT_H__ */