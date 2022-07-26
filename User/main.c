/********************************************************
* @file       main.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_task.h"
#include "drv_timer.h"

#include "app_battery.h"
#include "app_led.h"
#include "app_key.h"
#include "app_event.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

int main(void )
{
    Drv_Task_Init();

    Drv_Timer_Init();

    App_Event_Init();

    App_Batt_Init();

    App_Key_Init();

    App_Led_Init();
    
	while(1)
	{
        Drv_Task_Scheduler();
	}
	
}

