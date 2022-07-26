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
            default: break;
        }
    }
}

