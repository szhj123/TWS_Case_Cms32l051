/********************************************************
* @file       app_key.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
#include "drv_event.h"

#include "app_key.h"
#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Key_Detect_Callback(void );
static void App_Hall_Detect_Callback(void );
static void App_Key_Detect(void *arg);
static void App_Hall_Handler(void *arg );
/* Private variables ------------------------------------*/
static uint8_t keyTimerId = KEY_NULL;

void App_Key_Init(void )
{
    Drv_Key_Init();    

    Drv_Key_Regist_Isr_Callback(App_Key_Detect_Callback, App_Hall_Detect_Callback);
}

static void App_Key_Detect_Callback(void )
{
    if(keyTimerId == KEY_NULL)
    {
        Drv_Timer_Regist_Period(App_Key_Detect, 0, 1, NULL);
    }
}

static void App_Hall_Detect_Callback(void )
{
    static uint8_t timerId = TIMER_NULL;

    Drv_Timer_Delete(timerId);

    Drv_Timer_Regist_Oneshot(App_Hall_Handler, 500, NULL);
}

static void App_Key_Detect(void *arg )
{
    static uint8_t keyVal;
    
    Drv_Key_Detect(&keyVal);

    switch(keyVal)
    {
        case KEY_TX | KEY_DONW | KEY_UP:
        {
            break;
        }
        case KEY_TX | KEY_LONG:
        {
            break;
        }
        default: break;
    }

    if(keyVal & KEY_UP)
    {
        Drv_Timer_Delete(keyTimerId);
    }

    keyVal = KEY_NULL;
}

static void App_Hall_Handler(void *arg )
{    
    uint8_t caseState;
    
    if(Drv_Key_Get_Hall_State() == CASE_OPEN )
    {
        caseState = 1;
    }
    else
    {
        caseState = 0;
    }

    Drv_Msg_Put(CMD_CASE, &caseState, 1);
}

