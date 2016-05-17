#ifndef USB_DEVICE_ERROR
#define USB_DEVICE_ERROR

#define STATE_OK 0
#define NO_DLL 1
#define NO_FUNCTION 2
#define NO_DEVICE_CONNECTED 3
#define NOT_FULL_DATA 4
#define INVALID_PIPE_HANDLE 5
#define CALL_GET_LAST_ERROR 6
#define SESSION_ALREADY_OPEN 7

#define COMMUNICATION_ERROR 8

class USBDeviceError
{
public:
	USBDeviceError(void);
	~USBDeviceError(void);
};

#endif // USB_DEVICE_ERROR