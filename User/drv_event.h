#ifndef _DRV_MSG_H
#define _DRV_MSG_H

#define MSG_DATA_MAX_LENGTH                16
#define MSG_QUEUE_MAX_LENGTH               8

#define MSG_OK                             0
#define MSG_NULL                           (!MSG_OK)               

typedef struct _msg_t
{
    uint8_t cmd;
    uint8_t buf[MSG_DATA_MAX_LENGTH];
    uint8_t length;
}msg_t;

typedef struct _msg_queue_t
{
    msg_t qBuf[MSG_QUEUE_MAX_LENGTH];
    uint8_t head;
    uint8_t rear;
}msg_queue_t;

void Drv_Msg_Init(void );
void Drv_Msg_Put(uint8_t cmd, uint8_t *buf, uint8_t length );
uint8_t Drv_Msg_Get(msg_t *msg );



#endif 

