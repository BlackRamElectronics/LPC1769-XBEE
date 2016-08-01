//====================================================================================
// XBEE Module Driver
// BlackRamElectronics.com
//====================================================================================

#include <stdint.h>         // Include standard types
#include <stdio.h>
#include "BR_XBEE_Driver.h"
#include "BR_XBEE_HWIF.h"

//====================================================================================

//====================================================================================
void BR_XBEE_Init(void)
{

	int i;

    BR_XBEE_InitIF();			// Setup hardware interface

    BR_XBEE_ResetAssert();		// Put module into reset
    BR_XBEE_MsDelay(50);		// Delay for 50ms

    BR_XBEE_ResetDeassert();	// Bring module out of reset
    BR_XBEE_MsDelay(50);		// Delay for 50ms


	BR_XBEE_MsDelay(1000);

	// Enter command mode
	printf("Sending +++\r\n");
	BR_XBEE_SendBuffer("+++", 3);
	BR_XBEE_MsDelay(2000);

	while(1)
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
	}
}
