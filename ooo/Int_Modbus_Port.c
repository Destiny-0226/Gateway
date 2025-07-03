#include "Int_modbus_Port.h"

static uint8_t recv_buf[RECV_BUFFER_SIZE * RECV_BUFFER_COUNT] = {0};
static uint16_t recv_buf_size[RECV_BUFFER_COUNT]              = {0};
static volatile uint8_t current_read_index                    = 0;
static volatile uint8_t current_write_index                   = 0;

uint16_t len;
void Int_Modbus_Init()
{
    // 对串口的初始化
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE);
    // HAL_UARTEx_ReceiveToIdle(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE, &len, 0xffff);
}

// modbus发送
void Int_Modbus_Send(uint8_t *data, uint16_t len)
{
    log_info("send:");
    for (uint8_t i = 0; i < len; i++) {
        printf("%#x ", data[i]);
    }
    printf("\n\n");

    // HAL_UART_Transmit_DMA(&huart2, data, len);
    HAL_UART_Transmit(&huart2, data, len, 0xffff);
    HAL_Delay(600);
}

void Int_Modbus_Receive()
{
    log_debug("Int_Modbus_Receive");
    Int_ModBus_ReadHandle(recv_buf + current_read_index * RECV_BUFFER_SIZE, recv_buf_size[current_read_index]);
    // 清空缓冲区
    memset(recv_buf + current_read_index * RECV_BUFFER_SIZE, 0, RECV_BUFFER_SIZE);
    current_read_index++;
    if (current_read_index >= RECV_BUFFER_COUNT) {
        current_read_index = 0;
    }
}

/**
 * @brief 中断回调函数，用于处理Modbus协议的串口接收中断
 *
 * 当串口接收完成时，通过此回调函数通知相关的任务进行处理
 * 此外，还负责更新接收缓冲区的大小，并在接收到Idle状态时准备下一次接收
 *
 * @param size 接收到的数据大小
 */
void Int_Modbus_IRQ_Callback(uint16_t size)
{
    // 记录当前接收缓冲区的大小
    recv_buf_size[current_write_index++] = size;
    // 检查是否达到接收缓冲区组的末尾，如果是则重置索引
    if (current_write_index >= RECV_BUFFER_COUNT) {
        current_write_index = 0;
    }
    // 在空闲时启动下一次接收，使用DMA方式接收数据
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE);
    // HAL_UARTEx_ReceiveToIdle(&huart2, recv_buf + current_write_index * RECV_BUFFER_SIZE, RECV_BUFFER_SIZE, &len, 0xffff);
}
