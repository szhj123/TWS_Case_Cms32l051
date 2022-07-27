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
#include "app_battery.h"

/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
static void App_Led_Handler(void *arg );
static void App_Led_Light_5S_End_Callback(void *arg );
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

App_Led_Flash_callback_t App_Led_Flash_callback = NULL;

static uint8_t ledSolidOnNum;
static uint8_t ledFlashIndex;
static uint8_t ledTimerId = TIMER_NULL;

void App_Led_Init(void )
{
    Drv_Led_Init(&battLed1);
    Drv_Led_Off(&battLed1);
    
    Drv_Led_Init(&battLed2);
    Drv_Led_Off(&battLed2);
    
    Drv_Led_Init(&battLed3);
    Drv_Led_Off(&battLed3);
    
    Drv_Led_Init(&battLed4);
    Drv_Led_Off(&battLed4);

    Drv_Timer_Regist_Period(App_Led_Handler, 0, 1, NULL);
}

static void App_Led_Handler(void *arg )
{
    if(App_Led_Flash_callback != NULL)
    {
        App_Led_Flash_callback();
    }
}

void App_Led_Light_5S(void )
{
    if(App_Batt_Get_Usb_State())
    {
        return ;
    }

    if(App_Earbud_Get_State() == EARBUD_CHARGING_DONE)
    {
         App_Led_Batt_Discharing();
    }

    Drv_Timer_Delete(ledTimerId);

    ledTimerId = Drv_Timer_Regist_Oneshot(App_Led_Light_5S_End_Callback, 5000, NULL);
}

static void App_Led_Light_5S_End_Callback(void *arg )
{
    App_Led_All_Off();
}

void App_Led_Batt_Discharing(void )
{
    uint8_t battLevel;
    
    battLevel = App_Batt_Get_Level();
    
    if(battLevel == BATT_LEVEL_75)
    {
        ledSolidOnNum = 4;

        App_Led_Flash_callback = App_Led_Solid_On;
    }
    else if(battLevel == BATT_LEVEL_50)
    {
        ledSolidOnNum = 3;
        App_Led_Flash_callback = App_Led_Solid_On;
    }
    else if(battLevel == BATT_LEVEL_25)
    {
        ledSolidOnNum = 2;
        App_Led_Flash_callback = App_Led_Solid_On;
    }
    else if(battLevel == BATT_LEVEL_10)
    {
        ledSolidOnNum = 1;
        App_Led_Flash_callback = App_Led_Solid_On;
    }
    else if(battLevel == BATT_LEVEL_5)
    {
        ledFlashIndex = 1;
        
        battLed1.ledOffTime = LED_FLASH_REGULAR_ON_TIME;
        battLed1.ledOnTime = LED_FLASH_REGULAR_ON_TIME;
        battLed1.ledDelayCnt = 0;
            
        App_Led_Flash_callback = App_Led_Flash_One;
    }
}

void App_Led_Batt_Charging(void )
{
    uint8_t battLevel;
    
    if(App_Batt_Get_Usb_State())
    {
        battLevel = App_Batt_Get_Level();
        
        if(battLevel == BATT_LEVEL_100)
        {
            ledSolidOnNum = 4;

            App_Led_Flash_callback = App_Led_Solid_On;
        }
        else if(battLevel == BATT_LEVEL_75)
        {
            ledFlashIndex = 4;
            battLed4.ledOffTime = LED_FLASH_REGULAR_ON_TIME;
            battLed4.ledOnTime = LED_FLASH_REGULAR_ON_TIME;
            App_Led_Flash_callback = App_Led_Flash_One;
        }
        else if(battLevel == BATT_LEVEL_50)
        {
            ledFlashIndex = 3;
            battLed3.ledOffTime = LED_FLASH_REGULAR_ON_TIME;
            battLed3.ledOnTime = LED_FLASH_REGULAR_ON_TIME;
            App_Led_Flash_callback = App_Led_Flash_One;
        }
        else if(battLevel == BATT_LEVEL_25)
        {
            ledFlashIndex = 2;
            battLed2.ledOffTime = LED_FLASH_REGULAR_ON_TIME;
            battLed2.ledOnTime = LED_FLASH_REGULAR_ON_TIME;
            App_Led_Flash_callback = App_Led_Flash_One;
        }
        else 
        {
            ledFlashIndex = 1;
            battLed1.ledOffTime = LED_FLASH_REGULAR_ON_TIME;
            battLed1.ledOnTime = LED_FLASH_REGULAR_ON_TIME;
            App_Led_Flash_callback = App_Led_Flash_One;
        }
    }
}

void App_Led_Batt_Error(void )
{
    battLed1.ledOffTime = LED_FLASH_QUICK_ON_TIME;
    battLed1.ledOnTime = LED_FLASH_QUICK_ON_TIME;
    battLed2.ledOffTime = LED_FLASH_QUICK_ON_TIME;
    battLed2.ledOnTime = LED_FLASH_QUICK_ON_TIME;
    battLed3.ledOffTime = LED_FLASH_QUICK_ON_TIME;
    battLed3.ledOnTime = LED_FLASH_QUICK_ON_TIME;
    battLed4.ledOffTime = LED_FLASH_QUICK_ON_TIME;
    battLed4.ledOnTime = LED_FLASH_QUICK_ON_TIME;

    battLed1.ledDelayCnt = 0;
    battLed2.ledDelayCnt = 0;
    battLed3.ledDelayCnt = 0;
    battLed4.ledDelayCnt = 0;

    App_Led_Flash_callback = App_Led_Flash_All;
}

void App_Led_All_Off(void )
{
    Drv_Led_Off(&battLed1);
    Drv_Led_Off(&battLed2);
    Drv_Led_Off(&battLed3);
    Drv_Led_Off(&battLed4);

    App_Led_Flash_callback = NULL;
}

void App_Led_All_On(void )
{
    Drv_Led_On(&battLed1);
    Drv_Led_On(&battLed2);
    Drv_Led_On(&battLed3);
    Drv_Led_On(&battLed4);
}

void App_Led_Solid_On(void )
{
    if(ledSolidOnNum == 4)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_On(&battLed2);
        Drv_Led_On(&battLed3);
        Drv_Led_On(&battLed4);
    }
    else if(ledSolidOnNum == 3)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_On(&battLed2);
        Drv_Led_On(&battLed3);
        Drv_Led_Off(&battLed4);
    }
    else if(ledSolidOnNum == 2)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_On(&battLed2);
        Drv_Led_Off(&battLed3);
        Drv_Led_Off(&battLed4);
    }
    else if(ledSolidOnNum == 1)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_Off(&battLed2);
        Drv_Led_Off(&battLed3);
        Drv_Led_Off(&battLed4);
    }
}

void App_Led_Flash_One(void )
{
    if(ledFlashIndex == 4)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_On(&battLed2);
        Drv_Led_On(&battLed3);
        Drv_Led_Flash_Handler(&battLed4);
    }
    else if(ledFlashIndex == 3)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_On(&battLed2);
        Drv_Led_Flash_Handler(&battLed3);
        Drv_Led_Off(&battLed4);
    }
    else if(ledFlashIndex == 2)
    {
        Drv_Led_On(&battLed1);
        Drv_Led_Flash_Handler(&battLed2);
        Drv_Led_Off(&battLed3);
        Drv_Led_Off(&battLed4);
    }
    else if(ledFlashIndex == 1)
    {
        Drv_Led_Flash_Handler(&battLed1);
        Drv_Led_Off(&battLed2);
        Drv_Led_Off(&battLed3);
        Drv_Led_Off(&battLed4);
    }
}

void App_Led_Flash_All(void )
{
    Drv_Led_Flash_Handler(&battLed1);
    Drv_Led_Flash_Handler(&battLed2);
    Drv_Led_Flash_Handler(&battLed3);
    Drv_Led_Flash_Handler(&battLed4);
}


