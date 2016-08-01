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
	
	UART_IntConfig(LPC_UART2, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(LPC_UART2, UART_INTCFG_RLS, ENABLE);
	
	// Enable UART peripheral
	UART_TxCmd(LPC_UART2, ENABLE);

	// preemption = 1, sub-priority = 1
    NVIC_SetPriority(UART2_IRQn, ((0x01<<3)|0x01));
	// Enable Interrupt for UART0 channel
    NVIC_EnableIRQ(UART2_IRQn);


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

	/*if(UART_ReceiveByte(LPC_UART2) != 0)
	{
		return(1);
	}
	else
	{
		return(0);
	}*/
	
	uint8_t temp_rx = rx;
	rx = 0;
	
	return(temp_rx);
}

//====================================================================================
void UART2_IRQHandler(void)
{
	uint32_t intsrc, tmp, tmp1;

	// Determine the interrupt source
	intsrc = UART_GetIntId(LPC_UART2);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if(tmp == UART_IIR_INTID_RLS)
	{
		// Check line status
		tmp1 = UART_GetLineStatus(LPC_UART2);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if(tmp1)
		{
				UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
	{
		UART_IntReceive();
	}

	// Transmit Holding Empty
	if(tmp == UART_IIR_INTID_THRE)
	{
		UART_IntTransmit();
	}
}

//====================================================================================
void UART_IntReceive(void)
{
	uint8_t temp = UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART2);
	
	rx = 1;
	
	/*uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			// Check if buffer is more space If no more space, remaining character will be trimmed out
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}*/
}

//====================================================================================
void UART_IntTransmit(void)
{


/*
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);

	// Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
	// of data or break whenever ring buffers are empty
	// Wait until THR empty
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART0) == SET);

	while (!__BUF_IS_EMPTY(rb.tx_head,rb.tx_tail))
    {
        // Move a piece of data into the transmit FIFO
    	if (UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&rb.tx[rb.tx_tail], 1, NONE_BLOCKING)){
        // Update transmit ring FIFO tail pointer
        __BUF_INCR(rb.tx_tail);
    	} else {
    		break;
    	}
    }

    // If there is no more data to send, disable the transmit
    //   interrupt - else enable it or keep it enabled
	if (__BUF_IS_EMPTY(rb.tx_head, rb.tx_tail)) {
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
    	// Reset Tx Interrupt state
    	TxIntStat = RESET;
    }
    else{
      	// Set Tx Interrupt state
		TxIntStat = SET;
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, ENABLE);
    }*/
}

//====================================================================================
void UART_IntErr(uint8_t bLSErrType)
{
	while(1);
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
