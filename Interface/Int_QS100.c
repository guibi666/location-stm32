#include "Int_QS100.h"

#define IOT_Buff_Max_Len 128
uint8_t IOT_Buff[IOT_Buff_Max_Len];
uint16_t IOT_Buff_Len;

#define IOT_Buff_Full_Max_Len 128
uint8_t IOT_Buff_Full[IOT_Buff_Full_Max_Len];
uint16_t IOT_Buff_Full_Len;

//获取ip重试次数
#define IOT_RETRY_CNT 5

#define SEQUENCE 1


void Int_QS100_WKup(void)
{
    HAL_GPIO_WritePin(NB_WK_GPIO_Port,NB_WK_Pin,GPIO_PIN_SET);
    Com_Delay_ms(5);
    HAL_GPIO_WritePin(NB_WK_GPIO_Port,NB_WK_Pin,GPIO_PIN_RESET);
    Com_Delay_ms(5);
}

void Int_QS100_Reset(void)
{
    uint8_t *cmd = "AT+RB\r\n";
    HAL_UART_Transmit(&huart3,cmd,strlen((char *)cmd),1000);

    HAL_UART_Receive(&huart3,IOT_Buff,IOT_Buff_Max_Len,3000);
    IOT_Buff_Len = strlen((char *)IOT_Buff);
    if (IOT_Buff_Len > 0)
    {
        debug_printfln("%s",IOT_Buff);
        debug_printfln("QS100_Init was finished!");
        IOT_Buff_Len = 0;
        memset(IOT_Buff,0,IOT_Buff_Max_Len);
    }
    

}

void Int_QS100_SendCmd(uint8_t *cmd)
{
    HAL_UART_Transmit(&huart3,cmd,strlen((char *)cmd),1000);

    memset(IOT_Buff_Full,0,IOT_Buff_Full_Len);
    IOT_Buff_Full_Len = 0;
    
    
    while (1)
    {
        if ((strstr((char *)IOT_Buff_Full,"OK") != NULL) || (strstr((char *)IOT_Buff_Full,"ERROR") != NULL))
        {
            break;
        }
        HAL_UARTEx_ReceiveToIdle(&huart3,IOT_Buff,IOT_Buff_Max_Len,&IOT_Buff_Len,1000);
        memcpy(&IOT_Buff_Full[IOT_Buff_Full_Len],IOT_Buff,IOT_Buff_Len);
        IOT_Buff_Full_Len += IOT_Buff_Len;
        memset(IOT_Buff,0,IOT_Buff_Len);
        IOT_Buff_Len = 0;
        
    }
    debug_printfln("%s",IOT_Buff_Full);
    debug_printfln("==============");
    
}

void Int_QS100_Init(void)
{
    //初始化
    MX_USART3_UART_Init();

    //唤醒
    //Int_QS100_WKup();

    //软重启
    Int_QS100_Reset();
    

    //开启回显
    Int_QS100_SendCmd("ATE1\r\n");
}

CommonStatus Int_QS100_GetIP(void)
{
    //1.查询芯片是否处于附着状态
    /*
        +CGATT:1

        OK

    */
    Int_QS100_SendCmd("AT+CGATT?\r\n");

    if (strstr((char *)IOT_Buff_Full,"+CGATT:1") == NULL)
    {
        return Common_ERROR;
    }
    

    return Common_OK;
}

CommonStatus Int_QS100_Creat_Client(uint8_t *socket)
{
   
    Int_QS100_SendCmd("AT+NSOCR=STREAM,6,0,0\r\n");

    if (strstr((char *)IOT_Buff_Full,"OK") == NULL)
    {
        return Common_ERROR;
    }

    char *tmp = strstr((char *)IOT_Buff_Full,"+NSOCR:");
    *socket = tmp[7] - 48;
    return Common_OK;
}


CommonStatus Int_QS100_Connect_Server(uint8_t socket,uint8_t *ip,uint16_t port)
{
    uint8_t cmd[64] = {0};
    sprintf((char *)cmd,"AT+NSOCO=%d,%s,%d\r\n",socket,ip,port);

    Int_QS100_SendCmd(cmd);

    if (strstr((char *)IOT_Buff_Full,"OK") == NULL)
    {
        return Common_ERROR;
    }
    

    return Common_OK;
}

uint8_t cmd[512] = {0};

CommonStatus Int_QS100_Send_Data_to_Server(uint8_t socket, uint8_t *pData, uint16_t len)
{
    // 需要将数据转换为16进制字符串
    uint16_t hex_len = len * 2 + 1;
    uint8_t hex_data[hex_len];
    memset(hex_data, 0, hex_len);
    for (uint16_t i = 0; i < len; i++)
    {
        sprintf((char *)hex_data + i * 2, "%02X", pData[i]);
    }

    sprintf((char *)cmd, "AT+NSOSD=%d,%d,%s,0x200,%d\r\n", socket, len, hex_data, SEQUENCE);
    debug_printfln("%s", pData);
    // 发送命令
    Int_QS100_SendCmd(cmd);
    debug_printfln("%s", pData);
    // 判断发送数据成功和失败 => 不等于对方收到数据了 => ACK机制 => 回复ACK
    memset(cmd, 0, sizeof(cmd));

    // 发送确认收到ACK的命令
    /**
     *  AT+SEQUENCE=0,1

        2

        OK
     */
    sprintf((char *)cmd, "AT+SEQUENCE=%d,%d\r\n", socket, SEQUENCE);

    // 需要循环不断发送请求 => 一直等待显示发送成功或者失败
    char tmp = 0xff;

    while (tmp != '0' && tmp != '1')
    {
        // 手动在这里加延迟
        Com_Delay_ms(5);
        Int_QS100_SendCmd(cmd);
        tmp = IOT_Buff_Full[IOT_Buff_Full_Len - 9];
    }
    if (tmp == '0')
    {
        return Common_ERROR;
    }

    return Common_OK;
}


CommonStatus Int_QS100_Close_Client(uint8_t socket)
{
    uint8_t cmd[32] = {0};
    sprintf((char *)cmd,"AT+NSOCL=%d\r\n",socket);
    Int_QS100_SendCmd(cmd);

    if (strstr((char *)IOT_Buff_Full,"OK") == NULL)
    {
        return Common_ERROR;
    }
    

    return Common_OK;
}

CommonStatus Int_QS100_SendData(uint8_t * data,uint16_t len)
{
    //1.判断是否能连接外网,并有重试机会
    CommonStatus status = Common_ERROR;
    uint8_t count = IOT_RETRY_CNT;
    while (status == Common_ERROR && count > 0)
    {
        status = Int_QS100_GetIP();
        count --;
        //给重连设置一个间隔时间
        Com_Delay_ms(100);
    }
    if (status == Common_ERROR)
    {
        debug_printfln("cannt get IP");
        return Common_ERROR;
    }
    else
    {
        debug_printfln("get ip is ok");
    }
    
    

    //2.创建客户端
    status = Common_ERROR;
    count = IOT_RETRY_CNT;
    uint8_t socket = 0xff;
    while (status == Common_ERROR && count > 0)
    {
        status = Int_QS100_Creat_Client(&socket);
        count --;
        Com_Delay_ms(100);
    }
    if (status == Common_ERROR)
    {
        debug_printfln("creat client is fail");
        return Common_ERROR;
    }
    else
    {
        debug_printfln("creat client is success,socket:%d",socket);
    }
    
    
    //3.连接客户端
    status = Common_ERROR;
    count = IOT_RETRY_CNT;
    
    while (status == Common_ERROR && count > 0)
    {
        
        status =  Int_QS100_Connect_Server(socket,HTTP_SERVER_IP,HTTP_SERVER_PORT);
        count --;
        Com_Delay_ms(100);
    }
    if (status == Common_ERROR)
    {
        debug_printfln("connect is fail");
        Int_QS100_Close_Client(socket);
        return Common_ERROR;
    }
    else
    {
        debug_printfln("connect is success");
    }
    
    
    //4.进入ES状态
    //5.发送数据
    status = Common_ERROR;
    count = IOT_RETRY_CNT;
    while (status == Common_ERROR && count > 0)
    {
        status = Int_QS100_Send_Data_to_Server(socket, data, len);
        count--;
    }
    if (status == Common_ERROR)
    {
        debug_printfln("send data is fail");
        // 关闭客户端
        Int_QS100_Close_Client(socket);
        return Common_ERROR;
    }
    //6.关闭客户端
    status = Common_ERROR;
    count = IOT_RETRY_CNT;
    
    while (status == Common_ERROR && count > 0)
    {
        
        status = Int_QS100_Close_Client(socket);
        count --;
        Com_Delay_ms(100);
    }
    if (status == Common_ERROR)
    {
        debug_printfln("close is fail");
        return Common_ERROR;
    }
    else
    {
        debug_printfln("close is success");
    }
    
    Com_Delay_ms(1000);
    return Common_OK;
}

void Int_QS100_Enter_Low_Power(void)
{
    Int_QS100_SendCmd("AT+FASTOFF=0\r\n");
}

void Int_QS100_Exist_Low_Power(void)
{
    Int_QS100_WKup();
}
