#ifndef _DRV_COM_H
#define _DRV_COM_H

#include "hal_com.h"

#define CMD_CASE_OPEN                 150 //ms
#define CMD_CASE_CLOSE                300 //ms
#define CMD_CASE_TWS_PAIRING          450 //ms

#define COM_TX_IDLE                   0
#define COM_TX_BUSY                   (!COM_TX_IDLE)

typedef enum
{
    COM_STATE_TX_INIT = 0,
    COM_STATE_TX_HIGH,
    COM_STATE_TX_LOW,
    COM_STATE_TX_EXIT
}com_tx_state_t;

typedef struct _com_ctrl_block_t
{
    com_tx_state_t comTxState;
    uint16_t lowLevelTime;
    uint16_t delayCnt;
    uint8_t  txCnt;
    uint8_t  txEn;

    void (*tx_end_callback_t)(void );
}com_ctrl_block_t;

#include "hal_com.h"

void Drv_Com_Init(void );
void Drv_Com_Tx_Cmd(uint16_t lowLevelTime );
uint8_t Drv_Com_Get_Tx_State(void );
void Drv_COM_STATE_TX_HIGH(void );
void Drv_COM_STATE_TX_LOW(void );

#endif 

