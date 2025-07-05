#include "Int_Modbus_Port.h"

static TaskHandle_t Modbus_TaskHandle;
// 定义一个接收从机数据的缓存区
#define RX_Buffer_Size 10
uint8_t RX_Buffer[RX_Buffer_Size];
void Int_Modbus_Init(TaskHandle_t modbus_task_handle)
{
    Modbus_TaskHandle = modbus_task_handle;
    // 使用正常模式的DMA完成接收 所以先调用一次DMA接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RX_Buffer, RX_Buffer_Size);
}
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

void Int_Modbus_IRQ_Callback()
{
    // 获取DMA接收到的数据
    // 直接任务通知
    vTaskNotifyGiveFromISR(Modbus_TaskHandle, NULL); // 直接发送任务通知
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RX_Buffer, RX_Buffer_Size);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    if (huart->Instance == USART2) {
        if (huart->RxEventType == HAL_UART_RXEVENT_TC || huart->RxEventType == HAL_UART_RXEVENT_IDLE) {

            Int_Modbus_IRQ_Callback(Size); // rs485 callback
        }
    }
}
