#ifndef __INT_MQTT_H__
#define __INT_MQTT_H__

#include "main.h"
#include <stdio.h>

void Int_Mqtt_Init(void);
void Int_Mqtt_Yield(void);

void Int_Mqtt_SendData(uint8_t *data);
void Int_Mqtt_RegisterRecvCallback(void (*callback)(void *data, size_t data_len));

#endif /* __INT_MQTT_H__ */