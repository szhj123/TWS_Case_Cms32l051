#ifndef _APP_EVENT_H
#define _APP_EVENT_H

#include "drv_event.h"

#define SYS_STATE_WAKEUP        0
#define SYS_STATE_SLEEP         (!SYS_STATE_WAKEUP)

#define CMD_BATT                0x01
#define CMD_CASE                0x02
#define CMD_KEY                 0x03

void App_Event_Init(void );
uint8_t App_Key_Get_Hall_State(void );
void App_Sys_Sleep(void );
void App_Sys_Wakeup(void );
void App_Sys_Set_Sleep_State(uint8_t state );
uint8_t App_Sys_Get_Sleep_State(void );
void App_Sys_Delay_Count(void );
void App_Sys_Delay_Ms(uint16_t ms );

#endif 

