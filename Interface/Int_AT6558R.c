#include "Int_AT6558R.h"

#define GPS_Buff_Max_Len 512
uint8_t GPS_Buff[GPS_Buff_Max_Len];
uint16_t GPS_Buff_Len;


uint8_t GPS_Buff_Full[GPS_Buff_Full_Max_Len];
uint16_t GPS_Buff_Full_Len;


#define AT6558R_FREQ "PCAS02,1000"
#define AT6558R_MODE "PCAS04,3"

uint8_t send_Cmd_Buff[64];

void Int_AT6558R_Send_Cmd(uint8_t *cmd)
{
    //1.计算校验和
    uint8_t tmp = 0;
    for (uint8_t  i = 1; i != '\0';i ++)
    {
        tmp ^= cmd[i];
    }
    sprintf((char *)send_Cmd_Buff,"$%s*%02x\r\n",cmd,tmp);

    debug_printfln("\n%s",send_Cmd_Buff);

    HAL_UART_Transmit(&huart2,send_Cmd_Buff,strlen((char *)send_Cmd_Buff),1000);
    
}



void Int_AT6558R_Init(void)
{
    //1.初始化底层驱动
    MX_USART2_UART_Init();

    //2.GPS_EN
    //HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_SET);

    //3.修改参数
    //3.1设置刷新率 $PCAS02,1000*2E\r\n
    Int_AT6558R_Send_Cmd(AT6558R_FREQ);
    //3.2设置模式
    Int_AT6558R_Send_Cmd(AT6558R_MODE);

}

void Int_AT6558R_Read_GPS(void)
{
    //读取GPS信息
    /*
        $GNGGA,,,,,,0,00,25.5,,,,,,*64
        $GNGLL,,,,,,V,N*7A
        $GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,1*01
        $GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,4*04
        $GPGSV,1,1,00,0*65
        $BDGSV,1,1,00,0*74
        $GNRMC,,V,,,,,,,,,,N,V*37
        $GNVTG,,,,,,,,,N*2E
        $GNZDA,,,,,,*56
        $GPTXT,01,01,01,ANTENNA OPEN*25
    */
    //HAL_UARTEx_ReceiveToIdle(&huart2,GPS_Buff,GPS_Buff_Max_Len,GPS_Buff_Len,1000);
    //debug_printfln("\n%s",GPS_Buff);
    //memset(GPS_Buff,0,GPS_Buff_Max_Len);

    //初始化full_buff
    memset(GPS_Buff_Full,0,GPS_Buff_Full_Max_Len);
    GPS_Buff_Full_Len = 0;

    while (1)
    {
        if ((strstr((char *)GPS_Buff_Full,"GGA") != NULL) && (strstr((char*)GPS_Buff_Full,"TXT") != NULL))
        {
            break;
        }
        HAL_UARTEx_ReceiveToIdle(&huart2,GPS_Buff,GPS_Buff_Max_Len,&GPS_Buff_Len,1000);
        if (GPS_Buff_Len > 0)
        {
            memcpy(&GPS_Buff_Full[GPS_Buff_Full_Len],GPS_Buff,GPS_Buff_Len);
            //记录长度
            GPS_Buff_Full_Len += GPS_Buff_Len;
            //初始化缓存区
            memset(GPS_Buff,0,GPS_Buff_Max_Len);
            GPS_Buff_Len = 0;
        }
        
        
    }
    debug_printfln("\n%s",GPS_Buff_Full);
    
}

void Int_AT6558R_Enter_Low_Power(void)
{
    HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_RESET);
}

void Int_AT6558R_Exist_Low_Power(void)
{
    HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_SET);
    Com_Delay_ms(1000);
}
