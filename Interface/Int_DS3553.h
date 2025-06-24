#ifndef INT_DS3553_H
#define INT_DS3553_H

#include "i2c.h"
#include "Com_Delay.h"
#include "Com_Debug.h"

#define DS3553_ADDR 0x27
#define DS3553_ADDR_W 0x4E
#define DS3553_ADDR_R 0x4F

#define DS3553_REG_CHIP_ID 0x01
#define DS3553_REG_USER_SET 0xC3
#define DS3553_REG_STEP_CNT_L 0xC4
#define DS3553_REG_STEP_CNT_M 0xC5
#define DS3553_REG_STEP_CNT_H  0xC6

//片选
#define DS3553_CS_H (HAL_GPIO_WritePin(DS3553_CS_GPIO_Port,DS3553_CS_Pin,GPIO_PIN_SET));
#define DS3553_CS_L (HAL_GPIO_WritePin(DS3553_CS_GPIO_Port,DS3553_CS_Pin,GPIO_PIN_RESET));

void Int_DS3553_Init(void);

void Int_DS3553_ReadStep(uint32_t * step);

#endif
