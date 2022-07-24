/********************************************************
* @file       drv_msg.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "drv_msg.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/
msg_queue_t msgQueue;

void Drv_Msg_Init(void )
{
    msgQueue.head = msgQueue.rear = 0;    
}

void Drv_Msg_Put(uint8_t cmd, uint8_t *buf, uint8_t length )
{
    uint8_t i;

    if(length > MSG_DATA_MAX_LENGTH)
    {
        return ;
    }
    
    msgQueue.qBuf[msgQueue.rear].cmd = cmd;

    for(i=0;i<length;i++)
    {
        msgQueue.qBuf[msgQueue.rear].buf[i] = buf[i];
    }

    msgQueue.qBuf[msgQueue.rear].length = length;

    msgQueue.rear = (msgQueue.rear + 1) % MSG_QUEUE_MAX_LENGT;
}

uint8_t Drv_Msg_Get(msg_t *msg )
{
    uint8_t ret = MSG_NULL;
        
    if(msgQueue.head != msgQueue)    
    {
        *msg = msgQueue.qBuf[msgQueue.head];

        msgQueue.head = (msgQueue.head + 1) % MSG_QUEUE_MAX_LENGTH;

        ret = MSG_OK;
    }

    return ret;
}

