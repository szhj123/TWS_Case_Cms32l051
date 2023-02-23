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
    Cms32l051_Tim4_Channel0_Init();
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

