#ifndef _HAL_COM_H
#define _HAL_COM_H

#include "hal_cms32l051.h"
#include "hal_battery.h"

void Hal_Com_Init(void );
void Hal_Com_Tx_High(void );
void Hal_Com_Tx_Low(void );
void Hal_Com_Tx_Enable(void );
void Hal_Com_Tx_Disable(void );

#endif 

