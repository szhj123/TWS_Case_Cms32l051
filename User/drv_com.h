#ifndef _DRV_COM_H
#define _DRV_COM_H

#include "hal_com.h"

#define CMD_CASE_OPEN                 150 //ms
#define CMD_CASE_CLOSE                300 //ms
#define CMD_TWS_FACTORY_RESET         500 //ms
#define CMD_TWS_PARING                700 //ms
#define CMD_TWS_FACTORY_TEST          900 //ms

#define COM_TX_IDLE                   0
#define COM_TX_BUSY                   (!COM_TX_IDLE)

#define EARBUD_RESET_DONE             0
#define EARBUD_RESET_BUSY             (!EARBUD_RESET_DONE)

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

void Drv_Com_Init(void );
void Drv_Com_Tx_Cmd(uint16_t lowLevelTime );
uint8_t Drv_Com_Get_Tx_State(void );
void Drv_COM_STATE_TX_HIGH(void );
void Drv_COM_STATE_TX_LOW(void );

void Drv_Earbud_Reset(void );

void Drv_EarbudRst_Set_State(uint8_t state );
uint8_t Drv_EarbudRst_Get_State(void );

#endif 

