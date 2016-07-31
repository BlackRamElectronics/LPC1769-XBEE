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
    BR_XBEE_InitIF();			// Setup hardware interface

    BR_XBEE_ResetAssert();		// Put module into reset
    BR_XBEE_MsDelay(50);		// Delay for 50ms

    BR_XBEE_ResetDeassert();	// Bring module out of reset
    BR_XBEE_MsDelay(50);		// Delay for 50ms

    // Initialization Sequence
    BR_XBEE_SendBuffer("+++", 3);
    
    BR_XBEE_SendBuffer("AT+HV", 5);
}
