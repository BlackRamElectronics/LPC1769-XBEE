#ifndef _BLACKRAM_RINGBUFFER_H_
#define _BLACKRAM_RINGBUFFER_H_

#include <stdint.h>

// Define a data structure to hold info about out ring buffer instance
typedef struct
{
	uint8_t *Data;
	uint16_t Size;
	uint16_t In;
	uint16_t Out;
}BR_RingBuff_T;

/** ====================================================================================
 * @brief	Initialize ring buffer
 * @param	ring_buff	: Pointer to ring buffer structure to initialise
 * @param	buffer		: Pointer to an array to use for data storage
 * @param	size		: Size of ring buffer
 * @return	Nothing
 *  ==================================================================================== */
void BR_RingBuff_Init(BR_RingBuff_T *ring_buff, uint8_t *buffer, uint16_t size);


/** ====================================================================================
 * @brief	Clear ring buffer, set in pointer to out pointer 
 * @param	ring_buff	: Pointer to ring buffer structure to use
 * @return	Nothing
 *  ==================================================================================== */
void BR_RingBuff_Clear(BR_RingBuff_T *ring_buff);


/** ====================================================================================
 * @brief	Check if ring buffer is empty
 * @param	ring_buff	: Pointer to ring buffer structure to use
 * @return	true if ring buffer is empty, false otherwise
 *  ==================================================================================== */
bool BR_RingBuff_Empty(BR_RingBuff_T *ring_buff);


/** ====================================================================================
 * @brief	Check if ring buffer is full
 * @param	ring_buff	: Pointer to ring buffer structure to use
 * @return	true if ring buffer is full, false otherwise
 *  ==================================================================================== */
bool BR_RingBuff_Full(BR_RingBuff_T *ring_buff);


/** ====================================================================================
 * @brief	Add data byte to the buffer
 * @param	ring_buff	: Pointer to ring buffer structure to use
 * @param	data	    : Pointer to the data byte to add to the buffer
 * @return	false if ring buffer is full and failed to add data byte, true otherwise
 *  ==================================================================================== */
bool BR_RingBuff_Push(BR_RingBuff_T *ring_buff, const uint8_t *data);


/** ====================================================================================
 * @brief	Add data byte to the buffer
 * @param	ring_buff	: Pointer to ring buffer structure to use
 * @param	data	    : Pointer to the variable where we are to put the byte read
 * @return	false if ring buffer is empty and failed to read data byte, true otherwise
 *  ==================================================================================== */
bool BR_RingBuff_Pop(BR_RingBuff_T *ring_buff, uint8_t *data);


#endif	// _BLACKRAM_RINGBUFFER_H_
