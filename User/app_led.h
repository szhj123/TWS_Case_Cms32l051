#ifndef _APP_LED_H
#define _APP_LED_H

#include "drv_led.h"

typedef void (*app_led_flash_callback_t)(void );

void App_Led_Init(void );
void App_Led_Charging(void );
void App_Led_Discharging(void );


#endif 

