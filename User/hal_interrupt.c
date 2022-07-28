/********************************************************
* @file       hal_interrupt.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_task.h"
#include "hal_timer.h"
#include "hal_key.h"


#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
void IRQ17_Handler(void) __attribute__((alias("tm40_channel0_interrupt")));;
void IRQ01_Handler(void) __attribute__((alias("intp_0_interrupt")));
void IRQ02_Handler(void) __attribute__((alias("intp_1_interrupt")));
void IRQ03_Handler(void) __attribute__((alias("intp_2_interrupt")));
void IRQ04_Handler(void) __attribute__((alias("intp_3_interrupt")));

/* Private variables ------------------------------------*/

void SysTick_Handler(void )
{
    Hal_Task_IRQHandler();

    App_Sys_Delay_Count();
}

void tm40_channel0_interrupt(void )
{
    INTC_ClearPendingIRQ(TM00_IRQn);    /* clear INTTM00 interrupt flag */

    Hal_Timer_IRQHandler();
}

void intp_0_interrupt(void)
{
    INTC_ClearPendingIRQ(INTP0_IRQn);
}

void intp_1_interrupt(void )
{
    INTC_ClearPendingIRQ(INTP1_IRQn);
}

void intp_2_interrupt(void )
{
    INTC_ClearPendingIRQ(INTP2_IRQn);
    
    Hal_Hall_Isr_Handler();
}

void intp_3_interrupt(void )
{
    INTC_ClearPendingIRQ(INTP3_IRQn);
    
    Hal_Key_Isr_Handler();
}



