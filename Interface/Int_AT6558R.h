#ifndef __INT_AT6558R_
#define __INT_AT6558R_

#include "usart.h"
#include "Com_Debug.h"
#include "Com_Delay.h"

#define GPS_Buff_Full_Max_Len 512

extern uint8_t GPS_Buff_Full[GPS_Buff_Full_Max_Len];
extern uint16_t GPS_Buff_Full_Len;

void Int_AT6558R_Init(void);

void Int_AT6558R_Read_GPS(void);

void Int_AT6558R_Enter_Low_Power(void);

void Int_AT6558R_Exist_Low_Power(void);


#endif
