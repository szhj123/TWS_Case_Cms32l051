/********************************************************
* @file       app_led.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_timer.h"
#include "app_led.h"

/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Led_Handler(void *arg );
/* Private variables ------------------------------------*/
led_ctrl_block_t battLed1 = 
{
    .port = PORT12,
    .pin  = PIN3,
    .ledFlash = LED_FLASH_OFF,
    .ledOnTime = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

led_ctrl_block_t battLed2 = 
{
    .port = PORT12,
    .pin  = PIN4,
    .ledFlash = LED_FLASH_OFF,
    .ledOnTime = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

led_ctrl_block_t battLed3 = 
{
    .port = PORT1,
    .pin  = PIN4,
    .ledFlash = LED_FLASH_OFF,
    .ledOnTime = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

led_ctrl_block_t battLed4 = 
{
    .port = PORT1,
    .pin  = PIN3,
    .ledFlash = LED_FLASH_OFF,
    .ledOnTime = 0,
    .ledOffTime = 0,
    .ledDelayCnt = 0
};

app_led_flash_callback_t app_led_flash_callback = NULL;


void App_Led_Init(void )
{
    Drv_Led_Init(&battLed1);
    Drv_Led_Init(&battLed2);
    Drv_Led_Init(&battLed3);
    Drv_Led_Init(&battLed4);

    Drv_Timer_Regist_Period(App_Led_Handler, 0, 1, NULL);
}

static void App_Led_Handler(void *arg )
{
    if(app_led_flash_callback != NULL)
    {
        app_led_flash_callback();
    }
}

void App_Led_Charging(void )
{
    
}

