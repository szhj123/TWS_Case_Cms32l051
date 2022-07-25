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

    *adcSampleEndFlag = 0;

    if(adcSampleCnt < 100)
    {            
        
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
    static uint8_t tmpBattLevel;
    static batt_state_t tmpBattState;
    static earbud_state_t tmpEarbudState;
    
    App_Batt_Get_Para(&adcSampleEndFlag);

    if(Drv_Batt_Get_Usb_State() == USB_PLUG_OUT)
    {
        battPara.chagingState = FUNC_ENTRY;

        switch(battPara.dischargingState)
        {
            case FUNC_ENTRY:
            {
                if(adcSampleEndFlag)
                {
                    Drv_Batt_Boost_Enable();
                    
                    tmpBattLevel = App_Batt_Get_Level();

                    tmpBattState = App_Batt_Get_State();

                    tmpEarbudState = App_Earbud_Get_State();
                    
                    App_Batt_Send_Para();

                    battPara.dischargingState = FUNC_HANDLER;
                }
                break;
            }
            case FUNC_HANDLER:
            {
                if(adcSampleEndFlag)
                {
                    if(App_Batt_Get_Level() < tmpBattLevel)
                    {
                        tmpBattLevel = App_Batt_Get_Level();

                        App_Batt_Send_Para();
                    }

                    if(tmpBattState != App_Batt_Get_State())
                    {
                        tmpBattState = App_Batt_Get_State();
                        
                        App_Batt_Send_Para();
                    }

                    if(tmpEarbudState != App_Earbud_Get_State())
                    {
                        tmpEarbudState = App_Earbud_Get_State();

                        App_Batt_Send_Para();
                    }
                }
                break;
            }
        }
    }
    else
    {
        battPara.dischargingState = FUNC_ENTRY;

        switch(battPara.chagingState)
        {
            case FUNC_ENTRY:
            {
                if(adcSampleEndFlag)
                {
                    Drv_Batt_Boost_Enable();
                    
                    tmpBattLevel = App_Batt_Get_Level();

                    tmpBattState = App_Batt_Get_State();
                    
                    App_Batt_Send_Para();

                    battPara.chagingState = FUNC_HANDLER;
                }
                
                break;
            }
            case FUNC_HANDLER:
            {
                if(App_Batt_Get_Level() > tmpBattLevel)
                {
                    tmpBattLevel = App_Batt_Get_Level();
                    
                    App_Batt_Send_Para();
                }

                if(tmpBattState != App_Batt_Get_State())
                {
                    tmpBattState = App_Batt_Get_State();
                    
                    App_Batt_Send_Para();
                }
                
                break;
            }
            default: break;
        }
    }
}

uint8_t App_Batt_Get_Level(void )
{
    const uint8_t battErrVol = 25;//mv
    
    if(battPara.battLevel == 0)
    {
        if(battPara.battVol > BATT_VOL_75)
        {
            battPara.battLevel = BATT_LEVEL_75;
        }
        else if(battPara.battVol > BATT_VOL_50 && battPara.battVol <= BATT_VOL_75)
        {
            battPara.battLevel = BATT_LEVEL_50;
        }
        else if(battPara.battVol > BATT_VOL_25 && battPara.battVol <= BATT_VOL_50)
        {
            battPara.battLevel = BATT_LEVEL_25;
        }
        else if(battPara.battVol > BATT_VOL_10 && battPara.battVol <= BATT_VOL_25)
        {
            battPara.battLevel = BATT_LEVEL_10;
        } 
        else 
        {
            battPara.battLevel = BATT_LEVEL_5;
        }
    }
    else 
    {
        switch(battPara.battLevel)
        {
            case BATT_LEVEL_75:
            {
                if(battPara.battVol < (BATT_VOL_75 - battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_50;
                }
                break;
            }
            case BATT_LEVEL_50:
            {
                if(battPara.battVol > (BATT_VOL_75 + battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_75;
                }
                else if(battPara.battVol < (BATT_VOL_50 - battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_25;
                }
                break;
            }
            case BATT_LEVEL_25:
            {
                if(battPara.battVol > (BATT_VOL_50 + battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_50;
                }
                else if(battPara.battVol < (BATT_VOL_25 - battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_10;
                }
                break;
            }
            case BATT_LEVEL_10:
            {
                if(battPara.battVol > (BATT_VOL_25 + battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_25;
                }
                else if(battPara.battVol < (BATT_VOL_5 - battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_5;
                }
                break;
            }
            case BATT_LEVEL_5:
            {
                battPara.battVol > (BATT_VOL_5 + battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_10;
                }
            }
        }
        
    }
}

void App_Batt_Set_BatVol(uint16_t batVol )
{
    battPara.battVol = batVol;
}

uint16_t App_Batt_Get_BatVol(void )
{
    return battPara.battVol;
}

void App_Batt_Set_Usb_State(uint8_t state )
{
    battPara.usbPluginState = state;
}

uint8_t App_Batt_Get_Usb_State(void )
{
    return battPara.usbPluginState;
}

