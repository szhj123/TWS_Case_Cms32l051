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

void Drv_Batt_Init(void )
{
    Hal_Batt_Init();
}

uint8_t Drv_Batt_Get_Usb_State(void )
{
    return Hal_Batt_Get_Usb_State();
}

uint8_t Drv_Batt_Get_Charing_State(void )
{
    return Hal_Batt_Get_Charging_State();
}

uint16_t Drv_Batt_Get_BatVol(void )
{
    uint16_t batVol;
    
    batVol = (INTER_REF_VOL * 4096) / Hal_Batt_Get_AdcVal(ADC_INTERREFVOLT);

    return batVol;
}

uint16_t Drv_Batt_Get_NtcVol(void )
{
    uint16_t ntcVol;
    
    Hal_Batt_Ntc_PwrOn();

    ntcVol = (INTER_REF_VOL * Hal_Batt_Get_AdcVal(ADC_CHANNEL_2)) / Hal_Batt_Get_AdcVal(ADC_INTERREFVOLT);
	
    return ntcVol;
}

uint16_t Drv_Battery_Get_EarbudCur(void )
{
    uint16_t earbudCur;
    
    Hal_Batt_Earbud_Charging_Enable();

    earbudCur = (INTER_REF_VOL * Hal_Batt_Get_AdcVal(ADC_CHANNEL_0)) / Hal_Batt_Get_AdcVal(ADC_INTERREFVOLT);
	
    return earbudCur;
}

void Drv_Batt_Boost_Enable(void )
{
    Hal_Batt_Boost_Enable();
}

void Drv_Batt_Boost_Disable(void )
{
    Hal_Batt_Boost_Disable();
}
