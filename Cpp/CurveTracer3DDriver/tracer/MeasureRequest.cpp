#include "MeasureRequest.h"


MeasureRequest::MeasureRequest(void) : DeviceRequest(10, 10)
{
	sendBuffer[0] = 0x02;
}

MeasureRequest::~MeasureRequest(void)
{
}

int MeasureRequest::GetExpectedSize()
{
	return 8;
}

int MeasureRequest::GetSendSize()
{
	return 1;
}