/********************************************************
* @file       hal_com.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_com.h"

/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Hal_Com_Init(void )
{
    Cms32l051_Gpio_Init();
}

void Hal_Com_Tx_Enable(void )
{
    PORT_ClrBit(PORT12, PIN2);
}

void Hal_Com_Tx_Disable(void )
{
    PORT_SetBit(PORT12, PIN2);
}

