//====================================================================================
// Black Ram XBEE Hardware Interface
//
// This file should be edited by the user to provide a peripheral driver for
// interfacing to the XBEE module
//====================================================================================

// Function deceleration file
#include <stdint.h>			// Include standard types
#include "BR_XBEE_HWIF.h"
#include "string.h"

// User specific header files
#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include "BR_SysTick.h"

// PORT and PIN numbers that XBEE reset pin assigned on
#define RST_PORT_NUM	2
#define RST_PIN_NUM		(1<<13)

//====================================================================================
void BR_XBEE_InitIF(void)
{
	PINSEL_CFG_Type pinsel_cfg;
	UART_CFG_Type uart_cfg;

	// Initialize UART pin connect
	// 	P0.10 - TXD
	// 	P0.11 - RXD
	pinsel_cfg.Funcnum = PINSEL_FUNC_2;
	pinsel_cfg.OpenDrain = 0;
	pinsel_cfg.Pinmode = 0;
	pinsel_cfg.Portnum = PINSEL_PORT_0;
	pinsel_cfg.Pinnum = PINSEL_PIN_10;
	PINSEL_ConfigPin(&pinsel_cfg);
	pinsel_cfg.Pinnum = PINSEL_PIN_11;
	PINSEL_ConfigPin(&pinsel_cfg);

	// Initialize UART configuration structure to default
	UART_ConfigStructInit(&uart_cfg);

	uart_cfg.Baud_rate = 115200;
	uart_cfg.Parity = UART_PARITY_NONE;
	uart_cfg.Databits = UART_DATABIT_8;
	uart_cfg.Stopbits = UART_STOPBIT_1;

	// Initialize UART peripheral with parameter given in structure above
	UART_Init(LPC_UART2, &SSP_ConfigStruct);
	// Enable UART peripheral
	UART_Cmd(LPC_UART2, ENABLE);

	// Configure XBEE reset line and set high
	GPIO_SetDir(RST_PORT_NUM, RST_PIN_NUM, 1);
	GPIO_SetValue(RST_PORT_NUM, RST_PIN_NUM);
}

//====================================================================================
void BR_XBEE_MsDelay(uint32_t ms_delay)
{
	BR_MsDelay(ms_delay);
}

//====================================================================================
void BR_XBEE_SendByte(uint8_t data)
{
	UART_SendByte(LPC_UART2,data);
}

//====================================================================================
void BR_XBEE_SendBuffer(uint8_t *data, uint16_t length)
{
    uint8_t *data_ptr = data;
    uint16_t remaining_length = length;
    uint16_t current_tx_length;

	uint16_t i;

	for(i = 0; i < length; i++)
	{
	    while(UART_CheckBusy(LPC_UART2) == SET);
		UART_SendData(LPC_UART2, *data++);
	}
}

//====================================================================================
void BR_XBEE_ResetAssert(void)
{
	GPIO_ClearValue(RST_PORT_NUM, RST_PIN_NUM);
}

//====================================================================================
void BR_XBEE_ResetDeassert(void)
{
	GPIO_SetValue(RST_PORT_NUM, RST_PIN_NUM);
}
