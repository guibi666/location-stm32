#ifndef __APP_LOCATION_H
#define __APP_LOCATION_H

#include "Int_AT6558R.h"
#include "Int_QS100.h"
#include "Int_DS3553.h"
#include "Int_LoRa.h"
#include "Com_Debug.h"
#include "Com_Delay.h"
#include "cJSON.h"

void APP_Location_Get_GPS_Data(void);

void APP_Location_Get_Step_Count(void);

void APP_Location_Data_To_Json(void);

void APP_Location_Send_Data(void);

#endif
