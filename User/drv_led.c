/********************************************************
* @file       drv_led.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_led.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Drv_Led_Init(led_ctrl_block_t *led )
{
    Hal_Led_Init(led->port, led->pin);
}

void Drv_Led_On(led_ctrl_block_t *led )
{
    Hal_Led_On(led->port, led->pin);
}

void Drv_Led_Off(led_ctrl_block_t *led )
{
    Hal_Led_Off(led->port, led->pin);
}

void Drv_Led_Flash_Handler(led_ctrl_block_t *led )
{
    if(led->ledDelayCnt < 0xffff)
    {
        led->ledDelayCnt++;
    }

    if(Hal_Led_Get_State(led->port, led->pin) == LED_ON)
    {
        if(led->ledDelayCnt > led->ledOnTime)
        {
            led->ledDelayCnt = 0;

            Hal_Led_Off(led->port, led->pin);
        }
    }
    else
    {
        if(led->ledDelayCnt > led->ledOffTime)
        {
            led->ledDelayCnt = 0;

            Hal_Led_On(led->port, led->pin);
        }
    }
}

uint8_t Drv_Led_Get_State(led_ctrl_block_t *led )
{
    return Hal_Led_Get_State(led->port, led->pin);
}

