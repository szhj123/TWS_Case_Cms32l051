/********************************************************
* @file       hal_battery.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_battery.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define INTP0_CHARGING_STATE                (1 << 0)
#define INTP1_USB_DETECT                    (1 << 1)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Batt_Init(void )
{
    ADC_Init();
    //initialize NTC power pin
    PORT_Init(PORT2, PIN1, OUTPUT);
    //initialize boost enbale pin
    PORT_Init(PORT1, PIN0, OUTPUT);    
    //initialize charging state pin
    PORT_Init(PORT2, PIN3, PULLUP_INPUT);
    INTP_Init(INTP0_CHARGING_STATE, INTP_BOTH);
    INTP_Start(INTP0_CHARGING_STATE);
    //initialize usb detect pin
    PORT_Init(PORT12, PIN1, INPUT);
    INTP_Init(INTP1_USB_DETECT, INTP_BOTH);
    INTP_Start(INTP1_USB_DETECT);

    //initialize UART_EN
    PORT_Init(PORT12, PIN2, OUTPUT);
    
    Hal_Batt_Boost_Enable();

    Hal_Batt_Earbud_Charging_Enable();
}

void Hal_Batt_Ntc_PwrOn(void )
{
    PORT_SetBit(PORT2, PIN1);
}

void Hal_Batt_Ntc_PwrOff(void )
{
    PORT_ClrBit(PORT2, PIN1);
}

void Hal_Batt_Boost_Enable(void )
{
    PORT_SetBit(PORT1, PIN0);
}

void Hal_Batt_Boost_Disable(void )
{
    PORT_ClrBit(PORT1, PIN0);
}

void Hal_Batt_Earbud_Charging_Enable(void )
{
    PORT_SetBit(PORT12, PIN2);
}

void Hal_Batt_Earbud_Charging_Disable(void )
{
    PORT_ClrBit(PORT12, PIN2);
}

uint8_t Hal_Batt_Get_Usb_State(void )
{
    return PORT_GetBit(PORT12, PIN1);
}

uint8_t Hal_Batt_Get_Charging_State(void )
{
    return PORT_GetBit(PORT2, PIN3);
}

uint16_t Hal_Batt_Get_AdcVal(adc_channel_t adcChannel)
{
    uint16_t tmpBuf[16];
    
    ADC_Converse(adcChannel, sizeof(tmpBuf)/sizeof(tmpBuf[0]), tmpBuf);

    return ADC_MidAvg_Filter(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]));
}

