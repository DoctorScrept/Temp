#include "SignalRequest.h"


SignalRequest::SignalRequest(void) : DeviceRequest(10, 10)
{
	sendBuffer[0] = 0x00;    // command
	sendBuffer[1] = 2;       // Expected length of the result
}

SignalRequest::~SignalRequest(void)
{
}

int SignalRequest::GetExpectedSize()
{
	return 4;
}

int SignalRequest::GetSendSize()
{
	return 2;
}
