#ifndef __APP_MQTT_H__
#define __APP_MQTT_H__

#include "Int_MQTT.h"
#include "Com_cJSON.h"
#include "Com_log.h"
#include "APP_Modbus.h"

#define SLAVE_ADDR 5

extern uint8_t read_index;

void APP_Mqtt_Init(void);

void APP_Mqtt_Receive(void);

void APP_Mqtt_Send(uint8_t *data);

#endif /* __APP_MQTT_H__ */