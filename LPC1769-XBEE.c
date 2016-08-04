#include <stdio.h>

#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "BR_XBEE_Driver.h"
#include "BR_SysTick.h"

// PORT and PIN numbers that LED pin assigned on
#define LED_PORT_NUM	1
#define LED_PIN_NUM		(1<<18)

//====================================================================================
int main()
{
	uint8_t led = 0, j = 0;
	
	// Init SysTick
	BR_SysTickInit();

	HardwareInit();

	BR_MsDelay(500);
	printf("Black Ram XBEE\r\n");

	// Init on-board LED as output
	GPIO_SetDir(LED_PORT_NUM, LED_PIN_NUM, 1);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_ClearValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_ClearValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);

	// Init the XBEE module and required hardware
	if(BR_XBEE_Init() != BR_XBEE_RESULT_OK)
	{
		printf("Error in XBEE init\r\n");
		return(0);
	}
	else
	{
		printf("XBEE Init OK\r\n");
	}

	for(;;)
	{
		BR_MsDelay(200);
		
		if(led == 0)
		{
			led = 1;
			GPIO_ClearValue(LED_PORT_NUM, LED_PIN_NUM);
		}
		else
		{
			led = 0;
			GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);
		}
		
		
		//printf("%d\r\n", j++);
	}

	return(0);
}

//====================================================================================
void HardwareInit(void)
{
	// Config structures
	UART_CFG_Type uart_config;
	PINSEL_CFG_Type pinsel_config;
	
	// ----- Init SysTick -----
	SysTick_Config(SystemCoreClock / 1000);		// Generate interrupt every 1 ms

	// ----- Enable UART to PC for debug ----
	UART_ConfigStructInit(&uart_config);
	uart_config.Baud_rate = 115200;
	
	UART_Init(LPC_UART0, &uart_config);
	UART_TxCmd(LPC_UART0, ENABLE);
	
	pinsel_config.Portnum = PINSEL_PORT_0;
	pinsel_config.Funcnum = PINSEL_FUNC_1;
	pinsel_config.Pinmode = PINSEL_PINMODE_TRISTATE;
	pinsel_config.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	pinsel_config.Pinnum = PINSEL_PIN_2;
	PINSEL_ConfigPin(&pinsel_config);
	
	pinsel_config.Pinnum = PINSEL_PIN_3;
	PINSEL_ConfigPin(&pinsel_config);
	
}
