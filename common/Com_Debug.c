#include "Com_Debug.h"

void Com_Debug_Init(void)
{
    MX_USART1_UART_Init();
}

int fputc(int ch,FILE *file)
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,1000);
    return ch;
}
