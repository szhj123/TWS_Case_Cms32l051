/********************************************************
* @file       drv_battery.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_battery.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define INTER_REF_VOL                1450u//mv
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

static uint16_t refVolBuf[8];

void Drv_Batt_Init(void )
{
    uint8_t i;
    
    Hal_Batt_Init();

    for(i=0;i<8;i++)
    {
        refVolBuf[i] = Drv_Batt_Get_BatVol();
    }
}

uint16_t Drv_Batt_Get_BatVol(void )
{
    uint16_t batVol;
    
    batVol = (INTER_REF_VOL * 4096) / Hal_Batt_Get_AdcVal(ADC_INTERREFVOLT);

    return batVol;
}

uint16_t Drv_Batt_Get_NtcVol(void )
{
    
}

