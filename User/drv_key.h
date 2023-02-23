#ifndef _DRV_KEY_H
#define _DRV_KEY_H

#include "hal_key.h"

#define KEY_DONW     0x10
#define KEY_LONG     0x20
#define KEY_CONT     0x40
#define KEY_UP       0x80

#define KEY_TX       0x01
#define KEY_NULL     0x00

typedef enum 
{
    KEY_STATE_INIT = 0,
    KEY_STATE_SHORT_PRESS,
    KEY_STATE_LONG_PRESS,
    KEY_STATE_CONT_PRESS,
    KEY_STATE_RELEASE
}key_state_t;

void Drv_Key_Init(void );
void Drv_Key_Regist_Isr_Callback(hal_irq_handler_callback_t hallCallback );
void Drv_Key_Detect(uint8_t *keyVal );
uint8_t Drv_Key_Get_Hall_State(void );

#endif 

