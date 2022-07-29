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
#include "hal_key.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define INTP2_HALL_DETECT                    (1 << 2)
#define INTP3_KEY_DETECT                     (1 << 3)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
hal_irq_handler_callback_t Hal_Key_Isr_Callback = NULL;
hal_irq_handler_callback_t Hal_Hall_Isr_Callback = NULL;

void Hal_Key_Init(void )
{
    PORT_Init(PORT13, PIN6, INPUT);
    INTP_Init(INTP2_HALL_DETECT, INTP_BOTH);
    INTP_Start(INTP2_HALL_DETECT);

    PORT_Init(PORT1, PIN1, PULLUP_INPUT);
    INTP_Init(INTP3_KEY_DETECT, INTP_BOTH);
    INTP_Start(INTP3_KEY_DETECT);
}

uint8_t Hal_Key_Get_Tx_State(void )
{
    if(PORT_GetBit(PORT1, PIN1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t Hal_Key_Get_Hall_State(void )
{
    if(PORT_GetBit(PORT13, PIN6))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Hal_Key_Reigst_Isr_Callback(hal_irq_handler_callback_t keyCallback, hal_irq_handler_callback_t hallCallback )
{
    Hal_Key_Isr_Callback = keyCallback;

    Hal_Hall_Isr_Callback = hallCallback;
}

void Hal_Key_Isr_Handler(void )
{
    if(Hal_Key_Isr_Callback != NULL)
    {
        Hal_Key_Isr_Callback();
    }
}

void Hal_Hall_Isr_Handler(void )
{
    if(Hal_Hall_Isr_Callback != NULL)
    {
        Hal_Hall_Isr_Callback();
    }
}

