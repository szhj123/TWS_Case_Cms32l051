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
#include "app_event.h"
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
    const static uint16_t ADC_DET_COUNT = 200;
    
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

    if(adcSampleCnt < ADC_DET_COUNT)
    {            
        
        batVolSum += tmpBatVol;
        ntcVolSum += tmpNtcVol;
        earbudCurSum += tmpEarbudCur;
        
        adcSampleCnt++;
    }
    else
    {
        App_Batt_Set_BatVol(batVolSum / ADC_DET_COUNT);
        App_Batt_Set_NtcVol(ntcVolSum / ADC_DET_COUNT);
        App_Batt_Set_EarbudCur(earbudCurSum / ADC_DET_COUNT);
     
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
    static batt_ntc_state_t tmpBattNtcState;
    static earbud_state_t   tmpEarbudState;
    static batt_cur_state_t tmpCurState;
    
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

                    tmpBattNtcState = App_Batt_Get_Ntc_State();

                    tmpCurState = App_Batt_Get_Cur_State();

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

                    if(tmpBattNtcState != App_Batt_Get_Ntc_State())
                    {
                        tmpBattNtcState = App_Batt_Get_Ntc_State();
                        
                        App_Batt_Send_Para();
                    }

                    if(tmpCurState != App_Batt_Get_Cur_State())
                    {
                        tmpCurState = App_Batt_Get_Cur_State();
                        
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
            case FUNC_EXIT:
            {
                break;
            }
            default: break;
        }
    }
    else
    {
        static uint16_t battDelayCnt = 0;
        static uint16_t battVolSave = 0;
        
        battPara.dischargingState = FUNC_ENTRY;

        switch(battPara.chagingState)
        {
            case FUNC_ENTRY:
            {
                if(adcSampleEndFlag)
                {
                    if(battVolSave == 0)
                    {
                        Drv_Batt_Boost_Enable();

                        tmpBattLevel = App_Batt_Get_Level();

                        tmpBattNtcState = App_Batt_Get_Ntc_State();

                        tmpCurState = App_Batt_Get_Cur_State();

                        battVolSave = App_Batt_Get_BatVol();

                        battDelayCnt = 0;
                         
                        App_Batt_Send_Para();
                    }
                    else
                    {
                        if(++battDelayCnt > 15)
                        {
                            battPara.battErrVol =  battPara.battVol > battVolSave ? (battPara.battVol - battVolSave) : 0;
                            battDelayCnt = 0;
                            battVolSave = 0;
                            battPara.chagingState = FUNC_HANDLER;
                        }
                    }
                }
                break;
            }
            case FUNC_HANDLER:
            {
                if(adcSampleEndFlag)
                {
                    if(battPara.battVol >= 4200)
                    {
                        if(battPara.battErrVol > 0)
                        {
                            if(++battDelayCnt > 50)
                            {
                                battDelayCnt = 0;

                                battPara.battErrVol--;
                            }
                        }
                    }

                    
                    static uint16_t detectBattVol;
                    static uint16_t trueBattVol;
                    
                    detectBattVol = battPara.battVol;
                    battPara.battVol -= battPara.battErrVol;
                    trueBattVol = battPara.battVol;
                    
                    if(App_Batt_Get_Level() > tmpBattLevel)
                    {
                        tmpBattLevel = App_Batt_Get_Level();
                        
                        App_Batt_Send_Para();
                    }

                    if(tmpBattNtcState != App_Batt_Get_Ntc_State())
                    {
                        tmpBattNtcState = App_Batt_Get_Ntc_State();
                        
                        App_Batt_Send_Para();
                    }

                    if(tmpCurState != App_Batt_Get_Cur_State())
                    {
                        tmpEarbudState = App_Earbud_Get_State();
                        
                        App_Batt_Send_Para();
                    }
                }
                break;
            }
            default: break;
        }
    }
}

uint8_t App_Batt_Get_Level(void )
{
    const uint8_t battErrVol = 50;//mv

    if(Drv_Batt_Get_Usb_State())
    {
        if(Drv_Batt_Get_Charing_State())
        {
            battPara.battLevel = BATT_LEVEL_100;

            return battPara.battLevel;
        }
    }
    
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
                if(battPara.battVol > (BATT_VOL_5 + battErrVol))
                {
                    battPara.battLevel = BATT_LEVEL_10;
                }
            }
        }
        
    }

    return battPara.battLevel;
}

void App_Batt_Send_Para(void )
{
    uint8_t buf[5] = {0};

    buf[0] = App_Batt_Get_Usb_State();
    buf[1] = battPara.battLevel;
    buf[2] = (uint8_t )battPara.ntcState;
    buf[3] = (uint8_t )battPara.curState;
    buf[4] = (uint8_t )battPara.earbudState;    
    
    Drv_Msg_Put(CMD_BATT, buf, sizeof(buf));
}


batt_ntc_state_t App_Batt_Get_Ntc_State(void )
{
    uint32_t R_ntc = ((uint32_t )battPara.ntcVol * 10 * 1000) / (battPara.battVol - battPara.ntcVol);

    if(battPara.ntcState == BATT_NTC_NORMAL)
    {
        if(R_ntc < NTC_RES_45)
        {
            battPara.ntcState = BATT_NTC_OVER_TEMPER;
        }
    }
    else  if(battPara.ntcState == BATT_NTC_OVER_TEMPER) 
    {
        if(R_ntc > NTC_RES_42)
        {
            battPara.ntcState = BATT_NTC_NORMAL;
        }
    }
    
    return battPara.ntcState;
}

batt_cur_state_t App_Batt_Get_Cur_State(void )
{
    if(battPara.earbudCur > EARBUD_CUR_MAX_VALUE)
    {
        battPara.curState = BATT_CUR_OVER;
    }
    else
    {
        battPara.curState = BATT_CUR_NORMAL;
    }

    return battPara.curState;
}


earbud_state_t App_Earbud_Get_State(void )
{
    if(battPara.earbudCur <= EARBUD_CUR_MIN_VALUE)
    {
        battPara.earbudState = EARBUD_CHARGING_DONE;
    }
    else
    {
        battPara.earbudState = EARBUD_CHARGING_PROCESS;
    }
    
    return battPara.earbudState;
}

void App_Batt_Set_BatVol(uint16_t batVol )
{
    battPara.battVol = batVol;
}

uint16_t App_Batt_Get_BatVol(void )
{
    return battPara.battVol;
}

void App_Batt_Set_NtcVol(uint16_t ntcVol )
{
    battPara.ntcVol = ntcVol;
}

void App_Batt_Set_EarbudCur(uint16_t earbudCur )
{
    battPara.earbudCur = earbudCur;
}

uint8_t App_Batt_Get_Usb_State(void )
{
    return Drv_Batt_Get_Usb_State();
}

