#ifndef _DRV_BATTERY_H
#define _DRV_BATTERY_H

#include "hal_battery.h"

#define USB_PLUG_OUT                 0
#define USB_PLUG_IN                  (!USB_PLUG_OUT)

void Drv_Batt_Init(void );
uint8_t Drv_Batt_Get_Usb_State(void );
uint16_t Drv_Batt_Get_BatVol(void );
uint16_t Drv_Batt_Get_NtcVol(void );
void Drv_Batt_Boost_Enable(void );
void Drv_Batt_Boost_Disable(void );

#endif 

