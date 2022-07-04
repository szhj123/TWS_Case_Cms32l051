#ifndef _DRV_LED_H
#define _DRV_LED_H

#include "hal_led.h"

typedef enum _led_flash_t
{
    LED_FLASH_OFF = 0,
    LED_FLASH_SLOW,
    LED_FLASH_ON,
    LED_FLASH_REGULAR,
    LED_FLASH_QUICK
}led_flash_t;

typedef struct _led_ctrl_block_t
{
    PORT_TypeDef port;
    PIN_TypeDef  pin;
    uint16_t     ledOnTime;
    uint16_t     ledOffTime;
    uint16_t     ledDelayCnt;
}led_ctrl_block_t;

void Drv_Led_Init(led_ctrl_block_t *led );
void Drv_Led_On(led_ctrl_block_t *led );
void Drv_Led_Off(led_ctrl_block_t *led );
void Drv_Led_Flash_Handler(led_ctrl_block_t *led );
uint8_t Drv_Led_Get_State(led_ctrl_block_t *led );


#endif 

