/********************************************************
* @file       app_event.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_task.h"
#include "drv_com.h"

#include "app_event.h"
#include "app_key.h"
#include "app_battery.h"
#include "app_led.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Event_Handler(void *arg );
static void App_Event_Batt_Handler(uint8_t *buf, uint8_t length );
/* Private variables ------------------------------------*/

void App_Event_Init(void )
{
   Drv_Msg_Init();

   Drv_Task_Regist_Period(App_Event_Handler, 0, 1, NULL);
}

static void App_Event_Handler(void *arg )
{
    static msg_t msg;

    if(Drv_Msg_Get(&msg) == MSG_OK)
    {
        switch(msg.cmd)
        {
            case CMD_BATT:
            {
                App_Event_Batt_Handler(msg.buf, msg.length);
                break;
            }
            case CMD_CASE:
            {
                uint8_t caseState = msg.buf[0];

                if(caseState)
                {
                    Drv_Com_Tx_Cmd(CMD_CASE_OPEN);
                }
                else
                {
                    Drv_Com_Tx_Cmd(CMD_CASE_CLOSE);
                }

                App_Led_Light_5S();
                
                break;
            }
            case CMD_KEY:
            {
                uint8_t keyVal = msg.buf[0];

                if(keyVal == (KEY_TX | KEY_LONG))
                {
                    Drv_Com_Tx_Cmd(CMD_CASE_TWS_PAIRING);

                    App_Led_Earbud_Pair();
                }
            }
            default: break;
        }
    }
}

static void App_Event_Batt_Handler(uint8_t *buf, uint8_t length )
{
    uint8_t usbPlugState = buf[0];
    batt_level_t battLevel = (batt_level_t )buf[1];
    batt_ntc_state_t ntcState = (batt_ntc_state_t )buf[2];
    batt_cur_state_t curState = (batt_cur_state_t )buf[3];
    earbud_state_t   earbudState = (earbud_state_t )buf[4];

    if(usbPlugState)
    {
        if(ntcState == BATT_NTC_OVER_TEMPER || curState == BATT_CUR_OVER)
        {
            Drv_Batt_Boost_Disable();

            App_Led_Batt_Error();
        }
        else
        {
            if(Drv_Com_Get_Tx_State() == COM_TX_IDLE)
            {
                Drv_Batt_Boost_Enable();

                App_Led_Batt_Charging();
            }
        }
    }
    else
    {
        if(ntcState == BATT_NTC_OVER_TEMPER || curState == BATT_CUR_OVER)
        {
            Drv_Batt_Boost_Disable();

            App_Led_Batt_Error();
        }
        else
        {
            if(Drv_Com_Get_Tx_State() == COM_TX_IDLE)
            {
                Drv_Batt_Boost_Enable();

                if(earbudState == EARBUD_CHARGING_DONE)
                {
                    App_Led_All_Off();
                }
                else
                {
                    App_Led_Batt_Discharing();
                }

            }
        }
    }
}

