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
#include "lpc17xx_libcfg.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include "BR_SysTick.h"

// PORT and PIN numbers that XBEE reset pin assigned on
#define RST_PORT_NUM	2
#define RST_PIN_NUM		(1<<6)

//====================================================================================
void BR_XBEE_InitIF(void)
{
	PINSEL_CFG_Type pinsel_cfg;
	UART_CFG_Type uart_cfg;

	// Initialize UART pin connect
	// 	P0.10 - TXD
	// 	P0.11 - RXD
	pinsel_cfg.Funcnum = PINSEL_FUNC_1;
	pinsel_cfg.OpenDrain = 0;
	pinsel_cfg.Pinmode = 0;
	pinsel_cfg.Portnum = PINSEL_PORT_0;
	pinsel_cfg.Pinnum = PINSEL_PIN_10;
	PINSEL_ConfigPin(&pinsel_cfg);
	pinsel_cfg.Pinnum = PINSEL_PIN_11;
	PINSEL_ConfigPin(&pinsel_cfg);

	// Initialize UART configuration structure to default
	UART_ConfigStructInit(&uart_cfg);

	uart_cfg.Baud_rate = 9600;
	uart_cfg.Parity = UART_PARITY_NONE;
	uart_cfg.Databits = UART_DATABIT_8;
	uart_cfg.Stopbits = UART_STOPBIT_1;

	// Initialize UART peripheral with parameter given in structure above
	UART_Init(LPC_UART2, &uart_cfg);
	
	UART_IntConfig(LPC_UART2, UART_INTCFG_RBR | UART_INTCFG_RLS, ENABLE);
	
	// Enable UART peripheral
	UART_TxCmd(LPC_UART2, ENABLE);

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
		UART_SendByte(LPC_UART2, *data++);
	}
}

volatile uint8_t rx = 0;

//====================================================================================
uint8_t BR_XBEE_BytesRead()
{

	if(UART_ReceiveByte(LPC_UART2) != 0)
	{
		return(1);
	}
	else
	{
		return(0);
	}
	
	
	
	
	return(rx);
}

//====================================================================================
void UART2_IRQHandler(void)
{
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;
	
	uint8_t UART2Status;
	
	printf("!\r\n");
	
	IIRValue = LPC_UART2->IIR;
	
	IIRValue >>= 1;				// Skip pending bit in IIR
	IIRValue &= 0x07;			// Check bit 1~3, interrupt identification
	if(IIRValue == UART_INTCFG_RLS)		// Receive Line Status
	{
		LSRValue = LPC_UART0->LSR;
		// Receive Line Status
		if(LSRValue & (UART_LSR_OE|UART_LSR_PE|UART_LSR_FE|UART_LSR_RXFE|UART_LSR_BI))
		{
			// There are errors or break interrupt
			// Read LSR will clear the interrupt
			UART2Status = LSRValue;
			Dummy = LPC_UART2->RBR;		// Dummy read on RX to clear interrupt, then bail out
			return;
		}
		
		if(LSRValue & UART_LSR_RDR)	// Receive Data Ready
		{
			// If no error on RLS, normal ready, save into the data buffer.
			// Note: read RBR will clear the interrupt
			printf("RX1, %d\n", LPC_UART1->RBR);
			rx++;
			/*UART1Buffer[UART1Count] = LPC_UART1->RBR;
			UART1Count++;
			if(UART1Count == BUFSIZE)
			{
				UART1Count = 0;		// Buffer overflow
			}*/
		}
	}
	else if(IIRValue == UART_IIR_INTID_RDA)	// Receive Data Available
	{
		// Receive Data Available
		printf("RX2: %d\n", LPC_UART1->RBR);
		rx++;
		/*UART1Buffer[UART1Count] = LPC_UART1->RBR;
		UART1Count++;
		if(UART1Count == BUFSIZE)
		{
	  		UART1Count = 0;			// Buffer overflow
		}*/
  	}
	else if(IIRValue == UART_IIR_INTID_CTI)	// Character timeout indicator
	{
		printf("CTI\n");
		// Character Time-out indicator
		UART2Status |= 0x100;		// Bit 9 as the CTI error
	}
	else if(IIRValue == UART_IIR_INTID_THRE)	// THRE, transmit holding register empty
	{
		// THRE interrupt
		LSRValue = LPC_UART2->LSR;	// Check status in the LSR to see if valid data in U0THR or not
		/*if(LSRValue & LSR_THRE)
		{
			UART1TxEmpty = 1;
		}
		else
		{
			UART1TxEmpty = 0;
		}*/
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
