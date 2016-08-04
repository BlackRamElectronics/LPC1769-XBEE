#ifndef _XBEE_DRIVER_H_
#define _XBEE_DRIVER_H_


typedef enum BR_XBEE_Result_t
{
	BR_XBEE_RESULT_OK,
	BR_XBEE_RESULT_BUSY,
	BR_XBEE_RESULT_WAITING,
	BR_XBEE_RESULT_ERROR,
	BR_XBEE_RESULT_TIMEOUT,
}BR_XBEE_Result_t;

// Setup the hardware interface and configure the module
BR_XBEE_Result_t BR_XBEE_Init(void);



#endif // _XBEE_DRIVER_H_
