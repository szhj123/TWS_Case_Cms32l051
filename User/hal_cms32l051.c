/********************************************************
* @file       hal_cms32l051.c
* @author     szhj13
* @version    V1.0
* @date       2022-06-06
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hal_cms32l051.h"
/* Private typedef --------------------------------------*/
/* Private define ------------------ --------------------*/
#define INTP0_CHARGING_STATE                (1 << 0)
#define INTP1_USB_DETECT                    (1 << 1)
#define INTP2_HALL_DETECT                   (1 << 2)
#define INTP3_KEY_DETECT                    (1 << 3)
/* Private macro ----------------------------------------*/
/* Private function -------------------------------------*/
/* Private variables ------------------------------------*/

void Cms32l051_Gpio_Init(void )
{
    //p136, hall out, input
    PORT_Init(PORT13, PIN6, INPUT);
    
    //p124, led2, output
    //p123, led1, output
    //p122, uart_en, output
    //p121, 5v_detect, input 
    PORT_Init(PORT12, PIN4, OUTPUT);
    PORT_Init(PORT12, PIN3, OUTPUT);
    PORT_Init(PORT12, PIN2, OUTPUT);
    PORT_Init(PORT12, PIN1, INPUT);

    //p23, charging led, pull input
    //p22, ntc, adc input 
    //p21, ntc enable, output
    //p20, earbud current, adc input
    PORT_Init(PORT2, PIN3, PULLUP_INPUT);
    PORT_Init(PORT2, PIN1, OUTPUT);

    //p14, led3, output
    //p13, led4, output
    //p12, tx_rx, input
    //p11, key, pull input
    //p10, en boost, output
    PORT_Init(PORT1, PIN4, OUTPUT);
    PORT_Init(PORT1, PIN3, OUTPUT);
    PORT_Init(PORT1, PIN2, INPUT);
    PORT_Init(PORT1, PIN1, PULLUP_INPUT);
    PORT_Init(PORT1, PIN0, OUTPUT);    
}

void Cms32l051_Intp_Init(void )
{
    INTP_Init(INTP0_CHARGING_STATE, INTP_BOTH);
    INTP_Start(INTP0_CHARGING_STATE);
    
    INTP_Init(INTP1_USB_DETECT, INTP_BOTH);
    INTP_Start(INTP1_USB_DETECT);

    INTP_Init(INTP2_HALL_DETECT, INTP_BOTH);
    INTP_Start(INTP2_HALL_DETECT);

    INTP_Init(INTP3_KEY_DETECT, INTP_BOTH);
    INTP_Start(INTP3_KEY_DETECT);
}

void Cms32l051_Tim4_Channel0_Init(void )
{
    SystemCoreClockUpdate();
    //使能TM40外设时钟
    CGC->PER0 |= CGC_PER0_TM40EN_Msk; 
    //选择CKM0的时钟为Fclk 
    TM40->TPS0 = (TM40->TPS0 & ~TM40_TPS0_PRS00_Msk) | (0 << TM40_TPS0_PRS00_Pos);
    //选择CMK0 作为TM4通道0的时钟
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_CKS_Msk) | (0 << TM40_TMR00_CKS_Pos); 
    //TM4 通道0只有软件触发开始有效
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_STS_Msk) | (0 << TM40_TMR00_STS_Pos);
    //TM40通道0运行模式：间隔定时器
    TM40->TMR00 = (TM40->TMR00 & ~TM40_TMR00_MD_Msk) | (0 << TM40_TMR00_MD_Pos);

    TM40->TDR00 = (SystemCoreClock / 1000) - 1;

    INTC_ClearPendingIRQ(TM00_IRQn);
    NVIC_ClearPendingIRQ(TM00_IRQn);

    INTC_EnableIRQ(TM00_IRQn);

    TM40->TS0 |= TM40_TS0_TS00_Msk;
}

void Cms32l051_Adc_Init(void )
{
    ADC_Init();
}

