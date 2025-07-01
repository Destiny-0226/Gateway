#include "Com_log.h"

int fputc(int _Ch, FILE *_File)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&_Ch, 1, 1000);
    return _Ch;
}