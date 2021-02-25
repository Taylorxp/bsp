/***************************************************************************************************
* FILE: uart_gpio.h
*
* DESCRIPTION: GPIO¿ÚÄ£ÄâUART
*
* CREATED: 2020/3/14, by taylor
***************************************************************************************************/
#ifndef __UART_GPIO_H__
#define __UART_GPIO_H__

#include "stdint.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	uart_rx_start_bit = 0,
	uart_rx_stop_bit = 1
} UART_RX_STA_E;

typedef enum
{
	uart_tx_start_bit = -1,
	uart_tx_bit0,
	uart_tx_bit1,
	uart_tx_bit2,	
	uart_tx_bit3,
	uart_tx_bit4,
	uart_tx_bit5,
	uart_tx_bit6,
	uart_tx_bit7,
	uart_tx_bit8,
	uart_tx_bit9,
	uart_tx_bit10,	
	uart_tx_bit11,
	uart_tx_bit12,
	uart_tx_bit13,
	uart_tx_bit14,
	uart_tx_bit15,
	uart_tx_stop_bit
} UART_TX_STA_E;

extern UART_RX_STA_E e_uart_rx_sta;


#define BAUD_RATE_1000		10
#define DATA_BIT_LENGTH		16

void UART_GPIO_Init(void);
void UART_GPIO_TX_Data(uint8_t cmd, uint8_t data);
void UART_GPIO_RX_Data(void);

#ifdef __cplusplus
}
#endif

#endif
/****************************************** END OF FILE *******************************************/
