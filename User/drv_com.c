/********************************************************
* @file       drv_com.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_com.h"
#include "drv_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx_Handler(void *arg );
/* Private variables ------------------------------------*/
com_ctrl_block_t com;

void Drv_Com_Init(void )
{
    Hal_Com_Init();

    Drv_Task_Regist_Period(Drv_Com_Tx_Handler, 0, 1, NULL);
}

void Drv_Com_Tx_Cmd(uint16_t lowLevelTime )
{
     com.lowLevelTime = lowLevelTime;
     
     com.comTxState = COM_TX_INIT;

     com.delayCnt = 0;
     
     com.txCnt = 0;

     com.txEn = 1;
}

static void Drv_Com_Tx_Handler(void *arg )
{
    if(com.txEn == 0)
    {
        return ;
    }

    if(com.delayCnt < 0xffff)
    {
        com.delayCnt++;
    }

    switch(com.comTxState)
    {
        case COM_TX_INIT:
        {
            Drv_Com_Tx_High();
            
            com.delayCnt = 0;

            com.comTxState = COM_TX_HIGH;
            
            break;
        }
        case COM_TX_HIGH:
        {
            if(com.delayCnt > 50)
            {
                Drv_Com_Tx_Low();

                com.delayCnt = 0;

                com.comTxState = COM_TX_LOW;
            }
            
            break;
        }
        case COM_TX_LOW:
        {
            if(com.delayCnt > com.lowLevelTime)
            {
                com.delayCnt = 0;

                if(++com.txCnt >= 10)
                {
                    com.txCnt = 0;

                    com.comTxState = COM_TX_EXIT;
                }
                else
                {
                    com.comTxState = COM_TX_INIT;
                }
            }
            break;
        }
        case COM_TX_EXIT:
        {
            com.txEn = 0;
            
            break;
        }
        default: break;
    }
}

uint8_t Drv_Com_Get_Tx_State(void )
{
    return com.txEn;
}

void Drv_Com_Tx_High(void )
{
    Hal_Batt_Boost_Disable();

    Hal_Com_Tx_Enable();

    Hal_Com_Tx_High();
}

void Drv_Com_Tx_Low(void )
{
    Hal_Batt_Boost_Disable();

    Hal_Com_Tx_Enable();

    Hal_Com_Tx_Low();
}

