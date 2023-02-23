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
#include "drv_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void Drv_Com_Tx_Handler(void *arg );
static void Drv_Earbud_Reset_Callback(void *arg );
/* Private variables ------------------------------------*/
com_ctrl_block_t com;

static uint8_t timerReset = TIMER_NULL;
static uint32_t earBudResetCmd;
static uint16_t earBudResetDelayCnt;
static uint8_t  earBudResetBitCnt;
static uint8_t  earBudResetState;

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

void Drv_Earbud_Reset(void )
{
    earBudResetCmd = 0x002acaa0; 
    
    earBudResetDelayCnt = 0;

    earBudResetBitCnt = 0;

    Drv_Timer_Delete(timerReset);

    timerReset = Drv_Timer_Regist_Period(Drv_Earbud_Reset_Callback, 0, 1, NULL);
}

static void Drv_Earbud_Reset_Callback(void *arg )
{
    if(earBudResetCmd & (uint32_t )0x01)
    {
        Drv_COM_STATE_TX_HIGH();
    }
    else
    {
        Drv_COM_STATE_TX_LOW();
    }

    if(++earBudResetDelayCnt >= 7)
    {
        earBudResetDelayCnt = 0;
        
        earBudResetCmd >>= 1;

        if(++earBudResetBitCnt >= 30)
        {
            earBudResetBitCnt = 0;

            Drv_COM_STATE_TX_HIGH();
            
            Drv_Timer_Delete(timerReset);

            timerReset = TIMER_NULL;
        }
    }
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

                if(++com.txCnt >= 3)
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

void Drv_EarbudRst_Set_State(uint8_t state )
{
    earBudResetState = state;
}

uint8_t Drv_EarbudRst_Get_State(void )
{
    return earBudResetState;
}

