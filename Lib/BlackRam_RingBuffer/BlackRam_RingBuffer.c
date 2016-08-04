//====================================================================================
// Ring Buffer
// BlackRamElectronics.com
//====================================================================================

#incldue "BlackRam_RingBuffer.h"

//====================================================================================
void BR_RingBuff_Init(BR_RingBuff_T *ring_buff, uint8_t *buffer, uint16_t size)
{
	ring_buff->Data = buffer;			// Store the pointer to the data buffer
	ring_buff->Size = size;	
	ring_buff->In = 0;				// Reset the data in location pointer
	ring_buff->Out = 0;				// Reset the data out location pointer
}

//====================================================================================
void BR_RingBuff_Clear(BR_RingBuff_T *ring_buff)
{
	ring_buff->In = 0;	// Reset the data in location pointer
	ring_buff->Out = 0;	// Reset the data out location pointer
}

//====================================================================================
bool BR_RingBuff_Empty(BR_RingBuff_T *ring_buff)
{
	if(ring_buff->In == ring_buff->Out)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

//====================================================================================
bool BR_RingBuff_Full(BR_RingBuff_T *ring_buff)
{
	if(((ring_buff->In + 1) % ring_buff->Size) == ring_buff->Out)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

//====================================================================================
bool BR_RingBuff_Push(BR_RingBuff_T *ring_buff, const uint8_t *data)
{
	// Get a pointer to the next write address
	uint8_t *ptr = ring_buff->data + ring_buff->In;
	
	// Check that the buffer in not full
	if(BR_RingBuff_Full(ring_buff) == true)
	{
		return(false);
	}
	
	// Set the next data byte
	*ptr = *data;
	
	// Move the ring buffer input pointer on
	ring_buffer->In = (ring_buffer->In + 1) % ring_buffer->Size;
	
	return(true);
}

//====================================================================================
bool BR_RingBuff_Pop(BR_RingBuff_T *ring_buff, uint8_t *data)
{
	// Get a pointer to the next read address
	uint8_t *ptr = ring_buff->data + ring_buff->Out;
	
	// Check that the buffer in not empty
	if(BR_RingBuff_Empty(ring_buff) == true)
	{
		return(false);
	}
	
	// Set the next data byte
	*data = *ptr;
	
	// Move the ring buffer input pointer on
	ring_buffer->Out = (ring_buffer->Out + 1) % ring_buffer->Size;
	
	return(true);
}
