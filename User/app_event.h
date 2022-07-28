#ifndef _APP_EVENT_H
#define _APP_EVENT_H

#include "drv_event.h"

#define CMD_BATT                0x01
#define CMD_CASE                0x02
#define CMD_KEY                 0x03

void App_Event_Init(void );
uint8_t App_Key_Get_Hall_State(void );

#endif 

