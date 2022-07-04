/********************************************************
* @file       hal_timer.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_timer.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_irq_handler_callback_t timer_irq_handler_callback = NULL;

void Hal_Timer_Init(void )
{
    SystemCoreClockUpdate();
    //使能TM40外设时钟
    CGC->PER0 |= CGC_PER0_TM40EN_Msk; 
    //选择CKM0的时钟为Fclk 
    TM40->TPS0 = (TM40->TPS0 & ~TM40_TPS0_PRS00_Msk) | (0 << TM40_TPS0_PRS00_Pos);
    //选择CMK0 作为TM4通道0的时钟
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_CKS_Msk) | (0 << TM40_TMR00_CKS_Pos); 
    //TM4 通道0只有软件触发开始有效
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_STS_Msk) | (0 << TM40_TMR00_STS_Pos);
    //TM40通道0运行模式：间隔定时器
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_MD_Msk) | (0 << TM40_TMR00_MD_Pos);

    TM40->TDR00 = (SystemCoreClock / 1000) - 1;

    INTC_ClearPendingIRQ(TM00_IRQn);
    NVIC_ClearPendingIRQ(TM00_IRQn);

    INTC_EnableIRQ(TM00_IRQn);

    TM40->TS0 |= TM40_TS0_TS00_Msk;
}

void Hal_Timer_Regist_IRQHandler_Callback(hal_irq_handler_callback_t callback )
{
    timer_irq_handler_callback = callback;
}

void Hal_Timer_IRQHandler(void )
{
    if(timer_irq_handler_callback != NULL)
    {
        timer_irq_handler_callback();
    }
}

