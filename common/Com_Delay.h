#ifndef __COM_DELAY_H
#define __COM_DELAY_H


#include "main.h"
#include "time.h"
#include "stdio.h"
#include "Com_Debug.h"
void Com_Delay_us(uint16_t us);

void Com_Delay_ms(uint16_t ms);

void Com_Delay_s(uint16_t s);

void Com_UTC_to_Bj_Time(uint8_t * UTC_Time, uint8_t * Bj_Time);

#endif
