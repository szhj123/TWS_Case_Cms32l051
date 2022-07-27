#ifndef _APP_BATTERY_H
#define _APP_BATTERY_H

#include "drv_battery.h"

#define BATT_VOL_100                4200
#define BATT_VOL_75                 3950
#define BATT_VOL_50                 3820
#define BATT_VOL_25                 3700
#define BATT_VOL_10                 3600
#define BATT_VOL_5                  3475

#define NTC_RES_45                  4356
#define NTC_RES_42                  4905

#define EARBUD_CUR_MAX_VALUE        300
#define EARBUD_CUR_MIN_VALUE        10

typedef enum 
{
    BATT_LEVEL_= 0,
    BATT_LEVEL_5, 
    BATT_LEVEL_10,
    BATT_LEVEL_25,
    BATT_LEVEL_50,
    BATT_LEVEL_75,
    BATT_LEVEL_100
}batt_level_t;

typedef enum _func_state_t
{
    FUNC_ENTRY = 0,
    FUNC_HANDLER, 
    FUNC_EXIT
}func_state_t;

typedef enum _batt_ntc_state_t
{
    BATT_NTC_NORMAL = 0,
    BATT_NTC_OVER_TEMPER
}batt_ntc_state_t;

typedef enum _batt_cur_state_t
{
    BATT_CUR_NORMAL = 0,
    BATT_CUR_OVER
}batt_cur_state_t;

typedef enum _earbud_state_t
{
    EARBUD_CHARGING_PROCESS = 0,
    EARBUD_CHARGING_DONE
}earbud_state_t;

typedef struct _batt_para_t
{
    func_state_t dischargingState;
    func_state_t chagingState;

    batt_ntc_state_t ntcState;
    batt_cur_state_t curState;
    earbud_state_t   earbudState;
    
    batt_level_t battLevel;
    
    uint16_t battVol;
    uint16_t battErrVol;
    uint16_t ntcVol;
    uint16_t earbudCur;
    
    uint8_t  battChgFlag;    
    uint8_t  battSampleEndFlag;
    
    uint8_t  usbPluginState;
    
}batt_para_t;

void App_Batt_Init(void );
void App_Batt_Set_BatVol(uint16_t batVol );
void App_Batt_Set_NtcVol(uint16_t ntcVol );
uint16_t App_Batt_Get_BatVol(void );
void App_Batt_Set_EarbudCur(uint16_t earbudCur );
uint8_t App_Batt_Get_Level(void );
batt_ntc_state_t App_Batt_Get_Ntc_State(void );
batt_cur_state_t App_Batt_Get_Cur_State(void );
earbud_state_t App_Earbud_Get_State(void );
uint8_t App_Batt_Get_Usb_State(void );
void App_Batt_Send_Para(void );

#endif 

