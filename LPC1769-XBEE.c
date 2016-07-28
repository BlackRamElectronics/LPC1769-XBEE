#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "XBEE_Driver.h"
#include "BR_SysTick.h"

// PORT and PIN numbers that LED pin assigned on
#define LED_PORT_NUM	1
#define LED_PIN_NUM		(1<<18)

//====================================================================================
int main()
{
	// Init SysTick
	BR_SysTickInit();

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
	BR_XBEE_Init();

	for(;;)
	{

	}

	return(0);
}
