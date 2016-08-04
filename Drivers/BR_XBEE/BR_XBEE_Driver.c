//====================================================================================
// XBEE Module Driver
// BlackRamElectronics.com
//====================================================================================

// System Includes
#include <stdint.h>         // Include standard types
#include <stdio.h>

// Project Includes
#include "BR_XBEE_Driver.h"
#include "BR_XBEE_HWIF.h"
#incldue "BR_SysTick.h"

// File Decelerations
#define AT_TIMEOUT		100	// in ms
#define AT_RETRY_COUNT	3

// Function Decelerations
BR_XBEE_Result_t BR_XBEE_EnterCommandMode(uint8_t retry_count);
BR_XBEE_Result_t BR_XBEE_SendAT(uint8_t const *cmd_string, uint8_t const cmd_length, uint8_t *rsp_buff, uint8_t *rsp_length, uint32_t timeout_ms, uint8_t retry_count); 

// Global Variables
uint8_t AT_Response[32];
uint8_t AT_ResponseLen = 0;
	
	
//====================================================================================

//====================================================================================
BR_XBEE_Result_t BR_XBEE_Init(void)
{
	int i;
	BR_XBEE_Result_t result;
	

    BR_XBEE_InitIF();			// Set up hardware interface

	// Enter command mode
	if(BR_XBEE_EnterCommandMode(3) == BR_XBEE_RESULT_OK)
	{
		printf("In CMD mode\r\n");
	}
	else
	{
		printf("Failed CMD mode\r\n");
		return(BR_XBEE_RESULT_TIMEOUT);
	}
	
	printf("Sending ATSL\r\n");
	result = BR_XBEE_SendAT("ATSL \r", 6, AT_Response, &AT_ResponseLen, AT_TIMEOUT, AT_RETRY_COUNT);
	if(result == BR_XBEE_RESULT_OK)
	{
		printf("Response OK\r\n");
	}
	else
	{
		printf("Response ERROR\r\n");
	}
	
	
	/*while(1)
	{
		printf("Sending ATSL\r\n");
		BR_XBEE_SendBuffer("ATSL \r", 6);
		
		for(i = 0; i < 100; i++)
		{
			BR_XBEE_MsDelay(10);
			if(BR_XBEE_BytesRead() != 0)
			{
				printf("Read\r\n");
			}
		}
		BR_XBEE_MsDelay(1000);
	}*/
	
	return(BR_XBEE_RESULT_OK);
}

//====================================================================================
BR_XBEE_Result_t BR_XBEE_EnterCommandMode(uint8_t retry_count)
{
	/// TODO: Check all the following timings
	
	uint8_t i;
	
	for(i = 0; i < retry_count; i++)
	{
		BR_XBEE_ResetAssert();		// Put module into reset
		BR_XBEE_MsDelay(50);		// Delay for 50ms

		BR_XBEE_ResetDeassert();	// Bring module out of reset
		BR_XBEE_MsDelay(50);		// Delay for 50ms

		BR_XBEE_MsDelay(1000);

		// Enter command mode
		printf("Sending +++\r\n");
		BR_XBEE_SendBuffer("+++", 3);
		BR_XBEE_MsDelay(2000);
		
		printf("Sending AT\r\n");
		result = BR_XBEE_SendAT("AT \r", 4, AT_Response, &AT_ResponseLen, AT_TIMEOUT, AT_RETRY_COUNT); 
		if(result == BR_XBEE_RESULT_OK)
		{
			if(strncmp(AT_Response, "OK" , 2) == 0)
			{
				printf("Response OK\r\n");
				return(BR_XBEE_RESULT_OK);
			}
			else
			{
				printf("Unexpected response: %s", AT_Response);
			}
		}
		else
		{
			printf("Response ERROR\r\n");
		}
	}
	return(BR_XBEE_RESULT_ERROR);
}

//====================================================================================
BR_XBEE_Result_t BR_XBEE_SendAT(uint8_t const *cmd_string, uint8_t const cmd_length, uint8_t *rsp_buff, uint8_t *rsp_length, uint32_t timeout_ms, uint8_t retry_count)
{
	uint32_t timeout_timer;
	uint8_t retrys;
	uint8_t *current_read;
	
	for(retrys = 0; retrys < retry_count; retrys++)
	{
		// Get the current time
		timeout_timer = BR_SysTickGet();
		
		// Reset the read pointer and length
		current_read = rsp_buff;
		*rsp_length = 0;
		
		// Send the command
		BR_XBEE_SendBuffer(cmd_string, cmd_length);
		
		// Wait until we have received the response or a timeout/error has occurred
		while((BR_SysTickGet() - timeout_timer) < timeout_ms)
		{
			// Try to read a byte, process it if we get one
			if(BR_XBEE_GetByte(current_read) == true)
			{
				// Check for end of response
				if(*current_read == '\n')
				{
					*current_read = 0;
					*rsp_length++;
					return(BR_XBEE_RESULT_OK);
				}
			}
		}
	}
	return(BR_XBEE_RESULT_TIMEOUT);
}

