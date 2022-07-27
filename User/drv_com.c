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
     
     com.comTxState = COM_STATE_TX_INIT;

     com.delayCnt = 0;
     
     com.txCnt = 0;

     com.txEn = COM_TX_BUSY;
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
        case COM_STATE_TX_INIT:
        {
            Drv_COM_STATE_TX_HIGH();
            
            com.delayCnt = 0;

            com.comTxState = COM_STATE_TX_HIGH;
            
            break;
        }
        case COM_STATE_TX_HIGH:
        {
            if(com.delayCnt > 50)
            {
                Drv_COM_STATE_TX_LOW();

                com.delayCnt = 0;

                com.comTxState = COM_STATE_TX_LOW;
            }
            
            break;
        }
        case COM_STATE_TX_LOW:
        {
            if(com.delayCnt > com.lowLevelTime)
            {
                com.delayCnt = 0;

                if(++com.txCnt >= 10)
                {
                    Hal_Batt_Boost_Enable();

                    Hal_Com_Tx_Disable();
                    
                    com.txCnt = 0;

                    com.comTxState = COM_STATE_TX_EXIT;
                }
                else
                {
                    com.comTxState = COM_STATE_TX_INIT;
                }
            }
            break;
        }
        case COM_STATE_TX_EXIT:
        {
            com.txEn = COM_TX_IDLE;
            
            break;
        }
        default: break;
    }
}

uint8_t Drv_Com_Get_Tx_State(void )
{
    return com.txEn;
}

void Drv_COM_STATE_TX_HIGH(void )
{
    Hal_Batt_Boost_Enable();

    Hal_Com_Tx_Enable();
}

void Drv_COM_STATE_TX_LOW(void )
{
    Hal_Batt_Boost_Disable();

    Hal_Com_Tx_Enable();
}

