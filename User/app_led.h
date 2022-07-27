#ifndef _APP_LED_H
#define _APP_LED_H

#include "drv_led.h"

#define LED_FLASH_REGULAR_ON_TIME               500 //ms
#define LED_FLASH_REGULAR_OFF_TIME              500 //ms
#define LED_FLASH_QUICK_ON_TIME                 250
#define LED_FLASH_QUICK_OFF_TIME                250

typedef void (*App_Led_Flash_callback_t)(void );

void App_Led_Init(void );
void App_Led_Batt_Charging(void );
void App_Led_All_Off(void );
void App_Led_All_On(void );

void App_Led_Solid_On(void );
void App_Led_Flash_One(void );
void App_Led_Flash_All(void );

void App_Led_Light_5S(void );
void App_Led_Batt_Charging(void );
void App_Led_Batt_Discharing(void );
void App_Led_Batt_Error(void );


#endif 

