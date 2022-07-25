#ifndef _APP_BATTERY_H
#define _APP_BATTERY_H

#include "drv_battery.h"

#define BATT_VOL_100                4200
#define BATT_VOL_75                 3950
#define BATT_VOL_50                 3820
#define BATT_VOL_25                 3700
#define BATT_VOL_10                 3600
#define BATT_VOL_5                  3475

typedef enum 
{
    BATT_LEVEL_100 = 0,
    BATT_LEVEL_75,
    BATT_LEVEL_50,
    BATT_LEVEL_25,
    BATT_LEVEL_10,
    BATT_LEVEL_5
}batt_level_t;

typedef enum _func_state_t
{
    FUNC_ENTRY = 0,
    FUNC_HANDLER, 
    FUNC_EXIT
}func_state_t;

typedef enum _batt_state_t
{
    BATT_NORMAL = 0,
    BATT_OVER_TEMPER,
    BATT_OVER_CUR
}batt_state_t;

typedef enum _earbud_state_t
{
    EARBUD_CHARGING_PROCESS = 0,
    EARBUD_CHARGING_DONE
}earbud_state_t;

typedef struct _batt_para_t
{
    func_state_t dischargingState;
    func_state_t chagingState;

    batt_state_t battState;

    batt_level_t battLevel;
    
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

