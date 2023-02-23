/********************************************************
* @file       drv_msg.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_key.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static uint8_t Drv_Key_Scan(void );
/* Private variables ------------------------------------*/

void Drv_Key_Init(void )
{
    Hal_Key_Init();    
}

void Drv_Key_Regist_Isr_Callback(hal_irq_handler_callback_t hallCallback )
{
    Hal_Key_Reigst_Isr_Callback(hallCallback);
}

void Drv_Key_Detect(uint8_t *keyVal )
{
    static uint16_t keyDelayCnt;
    static key_state_t keyState;
    static uint8_t keySave = KEY_NULL;
    uint8_t tmpKeyVal = KEY_NULL;

    if(keyDelayCnt < 0xffff)
    {
        keyDelayCnt++;
    }

    tmpKeyVal = Drv_Key_Scan();

    switch(keyState)
    {
        case KEY_STATE_INIT:
        {
            if(tmpKeyVal != KEY_NULL)
            {
                keyDelayCnt = 0;

                keyState = KEY_STATE_SHORT_PRESS;
            }
            
            *keyVal = KEY_NULL;

            break;
        }
        case KEY_STATE_SHORT_PRESS:
        {
            if(tmpKeyVal != NULL)
            {
                if(keyDelayCnt > 25)
                {
                    keySave = tmpKeyVal | KEY_DONW;

                    *keyVal = keySave;

                    keyDelayCnt = 0;
                    
                    keyState = KEY_STATE_LONG_PRESS;                    
                }
            }
            else
            {
                keyState = KEY_STATE_INIT;
            }
            break;
        }
        case KEY_STATE_LONG_PRESS:
        {
            if(tmpKeyVal != NULL)
            {
                if(keyDelayCnt > 5000)
                {
                    keySave = tmpKeyVal | KEY_LONG;

                    *keyVal = keySave;

                    keyDelayCnt = 0;
                    
                    keyState = KEY_STATE_CONT_PRESS;                    
                }
            }
            else
            {
                keyState = KEY_STATE_RELEASE;
            }
            break;
        }
        case KEY_STATE_CONT_PRESS:
        {
            if(tmpKeyVal != NULL)
            {
                if(keyDelayCnt > 50)
                {
                    keySave = tmpKeyVal | KEY_CONT;

                    *keyVal = keySave;
                    
                    keyDelayCnt = 0;
                }
            }
            else
            {
                keyState = KEY_STATE_RELEASE;
            }

            break;
        }
        case KEY_STATE_RELEASE:
        {
            keySave |= KEY_UP;
            
            *keyVal = keySave;

            keySave = KEY_NULL;

            keyState = KEY_STATE_INIT;
            break;
        }
        default: break;
    }
}

static uint8_t Drv_Key_Scan(void )
{
    if(Hal_Key_Get_Tx_State() == 0)
    {
        return KEY_TX;
    }

    return KEY_NULL;
}

uint8_t Drv_Key_Get_Hall_State(void )
{
    return Hal_Key_Get_Hall_State();
}

