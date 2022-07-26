#ifndef _APP_LED_H
#define _APP_LED_H

#include "drv_led.h"

typedef void (*app_led_flash_callback_t)(void );

void App_Led_Init(void );
void App_Led_Charging(void );
void App_Led_Discharging(void );
void App_Led_All_Off(void );
void App_Led_All_On(void );
void App_Led_Solid_On(void );
void App_Led_Flash(void );
void App_Led_Light_5S(void );
void App_Led_Charging(void );


#endif 

