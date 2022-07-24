#ifndef _APP_BATTERY_H
#define _APP_BATTERY_H

#include "drv_battery.h"

typedef struct _batt_para_t
{
    uint16_t battVol;
    uint16_t battNtc;
    uint16_t earBudCur;
    
    uint8_t  battChgFlag;    
    uint8_t  battErrFlag;
    uint8_t  battOverTemperFlag;
    uint8_t  battSampleEndFlag;
    
    uint8_t  usbPluginState;
    
}batt_para_t;

void App_Batt_Init(void );

#endif 

