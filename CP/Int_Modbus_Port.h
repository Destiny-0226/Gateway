#ifndef __INT_MODBUS_PORT_H__
#define __INT_MODBUS_PORT_H__

#include "Int_Modbus.h"
#include "Com_log.h"
#include "main.h"
#include "usart.h"
#include <string.h>

#include "APP_Tasks.h"

#define RECV_BUFFER_SIZE  256
#define RECV_BUFFER_COUNT 4

extern uint8_t recv_buf[RECV_BUFFER_SIZE * RECV_BUFFER_COUNT];
extern uint16_t recv_buf_size[RECV_BUFFER_COUNT];
extern volatile uint8_t current_read_index;
extern volatile uint8_t current_write_index;

void Int_Modbus_Init(void);
void Int_Modbus_Send(uint8_t *data, uint16_t len);

#endif /* __INT_MODBUS_PORT_H__ */