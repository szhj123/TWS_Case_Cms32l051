#ifndef _HAL_CMS32L051_H
#define _HAL_CMS32L051_H

#include "stdlib.h"
#include "string.h"
#include "CMS32L051.h"
#include "userdefine.h"
#include "clk.h"
#include "gpio.h"
#include "adc.h"
#include "intp.h"

typedef void (*hal_irq_handler_callback_t)(void );

void Cms32l051_Gpio_Init(void );
void Cms32l051_Intp_Init(void );
void Cms32l051_Tim4_Channel0_Init(void );
void Cms32l051_Adc_Init(void );


#endif 

