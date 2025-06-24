#include "App_Low_Power.h"


void App_Enter_Low_Power(void)
{
    Int_AT6558R_Enter_Low_Power();
    Int_QS100_Enter_Low_Power();

    //stm32
    RTC_TimeTypeDef rtc_time;
    RTC_AlarmTypeDef rtc_alarm;

    //获取当前时间
    HAL_RTC_GetTime(&hrtc,&rtc_time,RTC_FORMAT_BIN);

    //设置闹钟=>在原来基础上加
    rtc_alarm.AlarmTime.Hours = rtc_time.Hours;
    rtc_alarm.AlarmTime.Minutes = rtc_time.Minutes;
    rtc_alarm.AlarmTime.Seconds = rtc_time.Seconds + 20;
    HAL_RTC_SetAlarm(&hrtc,&rtc_alarm,RTC_FORMAT_BIN);

    //清除标志位
    __HAL_RTC_CLEAR_FLAG(PWR_FLAG_WU);
    __HAL_RTC_CLEAR_FLAG(PWR_FLAG_SB);
    
    Com_Delay_ms(1000);
    HAL_PWR_EnterSTANDBYMode();
}

void App_Exist_Low_Power(void)
{
    Int_QS100_Exist_Low_Power();
    Int_AT6558R_Exist_Low_Power();
}
