#ifndef __INT_QS100_H
#define __INT_QS100_H

#include "usart.h"
#include "Com_Delay.h"
#include "Com_Debug.h"
#include "Com_Config.h"

void Int_QS100_Init(void);

CommonStatus Int_QS100_SendData(uint8_t * data,uint16_t len);

void Int_QS100_Enter_Low_Power(void);

void Int_QS100_Exist_Low_Power(void);


#endif
