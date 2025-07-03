#include "Int_modbus_Port.h"
#include "FreeRTOSConfig.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <string.h>

#define RECV_BUFFER_SIZE     256
#define RECV_BUFFER_COUNT    4

#define MODBUS_TASK_NAME     "Modbus_Task"
#define MODBUS_TASK_STACK    1024
#define MODBUS_TASK_PRIORITY 4

static uint8_t recv_buf[RECV_BUFFER_SIZE * RECV_BUFFER_COUNT] = {0};
static uint16_t recv_buf_size[RECV_BUFFER_COUNT]              = {0};
static volatile uint8_t current_read_index                    = 0;
static volatile uint8_t current_write_index                   = 0;

static SemaphoreHandle_t modbus_msg_semaphore = {0};

void Int_Modbus_Task(void *arg)
{

    while (1) {
        if (xSemaphoreTake(modbus_msg_semaphore, portMAX_DELAY) == pdTRUE) {
            // 调用回调函数处理数据
            Int_ModBus_ReadHandle(recv_buf + current_read_index * RECV_BUFFER_SIZE, recv_buf_size[current_read_index]);
            // 清空缓冲区
            memset(recv_buf + current_read_index * RECV_BUFFER_SIZE, 0, RECV_BUFFER_SIZE);
            current_read_index++;
            if (current_read_index >= RECV_BUFFER_COUNT) {
                current_read_index = 0;
            }
        }
    }
}
void Int_Modbus_IRQ_Callback(uint16_t size)
{
    recv_buf_size[current_write_index++] = size;
    if (current_write_index >= RECV_BUFFER_COUNT) {
        current_write_index = 0;
    }
    xSemaphoreGiveFromISR(modbus_msg_semaphore, NULL);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE);
}
void Int_Modbus_Init()
{

    modbus_msg_semaphore = xSemaphoreCreateCounting(RECV_BUFFER_COUNT, 0);

    // 对串口的初始化
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE);

    xTaskCreate(Int_Modbus_Task,
                MODBUS_TASK_NAME,
                MODBUS_TASK_STACK,
                NULL,
                MODBUS_TASK_PRIORITY,
                NULL);
}

// modbus发送的代码
void Int_Modbus_Send(uint8_t *data, uint16_t len)
{
    log_info("\nsend:");
    for (uint8_t i = 0; i < len; i++) {
        printf("%#x ", data[i]);
    }
    printf("\n\n");

    HAL_UART_Transmit_DMA(&huart2, data, len);
    HAL_Delay(1000); // 为了防止发送太快,导致电机板接收不到数据/发送不过来数据
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    if (huart->Instance == USART2) {
        if (huart->RxEventType == HAL_UART_RXEVENT_TC || huart->RxEventType == HAL_UART_RXEVENT_IDLE) {

            Int_Modbus_IRQ_Callback(Size); // rs485 callback
        }
    }
}
