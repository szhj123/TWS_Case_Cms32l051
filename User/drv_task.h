#ifndef _DRV_TASK_H
#define _DRV_TASK_H

#include "hal_task.h"

#define TASK_OK                0
#define TASK_ERR               (!TASK_OK)

typedef enum _task_state_t
{
    TASK_READY = 0,
    TASK_SUSPEND,
    TASK_SLEEP,
    TASK_IDLE
}task_state_t;

typedef struct _task_ctrl_block_t
{
    void (*handler)(void *);
    void *arg;

    task_state_t state;
    uint8_t id;
    uint16_t ticks;
    uint16_t period;

    struct _task_ctrl_block_t *prev;
    struct _task_ctrl_block_t *next;
}task_ctrl_block_t;

void Drv_Task_Init(void );
task_ctrl_block_t * Drv_Task_Regist_Oneshot(void (*handler)(void *), uint16_t ticks, void *arg );
task_ctrl_block_t * Drv_Task_Regist_Period(void (*handler)(void *), uint16_t ticks, uint16_t period, void *arg );
uint8_t Drv_Task_Delay(task_ctrl_block_t *task, uint16_t ticks );
uint8_t Drv_Task_Sleep(task_ctrl_block_t *task );
uint8_t Drv_Task_Wakeup(task_ctrl_block_t *task );
uint8_t Drv_Task_Delete(task_ctrl_block_t *task );
void Drv_Task_Scheduler(void );


#endif 

