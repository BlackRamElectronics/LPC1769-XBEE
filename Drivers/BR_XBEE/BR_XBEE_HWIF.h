#ifndef _BR_XBEE_HARDWARE_INTERFACE_H_
#define _BR_XBEE_HARDWARE_INTERFACE_H_

void BR_XBEE_InitIF(void);
void BR_XBEE_MsDelay(uint32_t ms_delay);
void BR_XBEE_SendByte(const uint8_t data);
void BR_XBEE_SendBuffer(const uint8_t *data, const uint16_t length);
bool BR_XBEE_GetByte(uint8_t *data);
void BR_XBEE_ResetAssert(void);
void BR_XBEE_ResetDeassert(void);

#endif // _BR_XBEE_HARDWARE_INTERFACE_H_
