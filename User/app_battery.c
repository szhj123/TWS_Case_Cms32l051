/********************************************************
* @file       app_battery.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_task.h"
#include "app_battery.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/      
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Batt_Handler(void *arg );
/* Private variables ------------------------------------*/
batt_para_t battPara;

void App_Batt_Init(void )
{
    Drv_Batt_Init();

    Drv_Task_Regist_Period(App_Batt_Handler, 0, 1, NULL);
}

static void App_Batt_Get_Para(uint8_t *adcSampleEndFlag )
{
    static uint16_t adcSampleCnt;
    static uint32_t batVolSum;
    static uint32_t ntcVolSum;
    static uint32_t earbudCurSum;

    uint16_t tmpBatVol;
    uint16_t tmpNtcVol;
    uint16_t tmpEarbudCur;
	    
    tmpBatVol = Drv_Batt_Get_BatVol();

    tmpNtcVol = Drv_Batt_Get_NtcVol();
        
    tmpEarbudCur = Drv_Battery_Get_EarbudCur();

    if(adcSampleCnt < 100)
    {            
        *adcSampleEndFlag = 0;
        
        batVolSum += tmpBatVol;
        ntcVolSum += tmpNtcVol;
        earbudCurSum += tmpEarbudCur;
        
        adcSampleCnt++;
    }
    else
    {
        App_Batt_Set_BatVol(batVolSum / 100);
        App_Batt_Set_NtcVol(ntcVolSum / 100);
        App_Batt_Set_EarbudCur(earbudCurSum / 100);
     
        *adcSampleEndFlag = 1;

        adcSampleCnt = 0;
        batVolSum = 0;
        ntcVolSum = 0;
        earbudCurSum = 0;
    } 
}

static void App_Batt_Handler(void *arg )
{
    static uint8_t adcSampleEndFlag;
    
    App_Batt_Get_Para(&adcSampleEndFlag);

    if(Drv_Batt_Get_Usb_State() == USB_PLUG_OUT)
    {
        if(App_Batt_Get_Usb_State() == USB_PLUG_IN)
        {
            App_Batt_Set_Usb_State(USB_PLUG_OUT);

            Drv_Msg_Put(CMD_USB_PLUG_OUT, NULL, 0);
        }
    }
    else
    {
        
    }
}

void App_Batt_Set_Usb_State(uint8_t state )
{
    battPara.usbPluginState = state;
}

uint8_t App_Batt_Get_Usb_State(void )
{
    return battPara.usbPluginState;
}

