#include "Int_DS3553.h"

void Int_DS3553_ReadReg(uint8_t reg_addr, uint8_t *reg_data)
{
    // uint32_t reg_data;
    // HAL_I2C_Mem_Read(&hi2c1,DS3553_ADDR_R,0x01,I2C_MEMADD_SIZE_8BIT,&reg_data,1,1000);

    // 拉低片选
    DS3553_CS_L;
    Com_Delay_ms(5);

    // 读取
    HAL_I2C_Mem_Read(&hi2c1, DS3553_ADDR_R, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, 1, 1000);

    // 拉高片选
    DS3553_CS_H;
    Com_Delay_ms(12);
}

void Int_DS3553_WriteReg(uint8_t reg_addr, uint8_t reg_data)
{
    // 拉低片选
    DS3553_CS_L;
    Com_Delay_ms(5);

    // 读取
    HAL_I2C_Mem_Write(&hi2c1, DS3553_ADDR_W, reg_addr, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, 1000);

    // 拉高片选
    DS3553_CS_H;
    Com_Delay_ms(12);
}

void Int_DS3553_Init(void)
{
    // MX_I2C1_Init();

    uint8_t data = 0xff;
    Int_DS3553_ReadReg(0x01, &data);

    debug_printfln("\n0x%x", data);

    // 设置参数
    // 芯片默认值
    uint8_t tmp = 0x18;

    // 关闭计步脉冲中断
    tmp &= ~(0x01 << 4);
    // 选择计步器模式
    tmp |= (0x01 << 1);

    Int_DS3553_WriteReg(DS3553_REG_USER_SET, tmp);
}

void Int_DS3553_ReadStep(uint32_t *step)
{
    uint8_t step_l, step_m, step_h;
    Int_DS3553_ReadReg(DS3553_REG_STEP_CNT_L, &step_l);
    Int_DS3553_ReadReg(DS3553_REG_STEP_CNT_M, &step_m);
    Int_DS3553_ReadReg(DS3553_REG_STEP_CNT_H, &step_h);

    //拼接步数
    *step = (step_h << 16) | (step_m << 8) | step_l;
}
