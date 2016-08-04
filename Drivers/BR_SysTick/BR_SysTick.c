#include "lpc17xx_libcfg.h"
#include "lpc17xx_systick.h"
#include "BR_SysTick.h"

volatile uint32_t msTicks = 0; // counter for 1ms SysTicks

//====================================================================================
void SysTick_Handler(void)
{
	msTicks++;
}

//====================================================================================
void BR_SysTickInit(void)
{
	SYSTICK_InternalInit(1);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}

//====================================================================================
uint32_t BR_SysTickGet(void)
{
	return(msTicks);
}

//====================================================================================
void BR_MsDelay(uint32_t ms)
{
	uint32_t timer_mark;

	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + ms));	// Wait until ms has passed
}
