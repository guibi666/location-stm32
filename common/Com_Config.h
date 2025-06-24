#ifndef __COM_Config_H
#define __COM_Config_H

#define HTTP_SERVER_IP "112.125.89.8"
#define HTTP_SERVER_PORT 47162 

//函数运行判断运行结果是否成功
typedef enum
{
    Common_OK ,
    Common_ERROR
}CommonStatus;

typedef struct
{
    uint8_t uuid[33];

    //GPS
    double lon;//经度
    uint8_t lonDir[2];
    double lat;//纬度
    uint8_t latDir[2];
    //定位时间
    uint8_t dataTime[21];
    
    //步数信息
    uint32_t stepCount;

    uint8_t data[512];
    uint16_t dataLen;
}UploadData;

#endif
