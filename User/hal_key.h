#ifndef _HAL_KEY_H
#define _HAL_KEY_H

#include "hal_cms32l051.h"

void Hal_Key_Init(void );
void Hal_Key_Reigst_Isr_Callback(hal_irq_handler_callback_t keyCallback, hal_irq_handler_callback_t hallCallback );
uint8_t Hal_Key_Get_Tx_State(void );
uint8_t Hal_Key_Get_Hall_State(void );
void Hal_Key_Isr_Handler(void );
void Hal_Hall_Isr_Handler(void );

#endif 

