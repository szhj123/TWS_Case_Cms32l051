/********************************************************
* @file       hal_task.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_task.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_irq_handler_callback_t systick_irq_handler_callback = NULL;

void Hal_Task_Init(void )
{
    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock/1000);
}

void Hal_Task_Regist_IRQHandler_Callback(hal_irq_handler_callback_t callback )
{
    systick_irq_handler_callback = callback;
}

void Hal_Task_IRQHandler(void )
{
    if(systick_irq_handler_callback != NULL)
    {
        systick_irq_handler_callback();
    }
}

