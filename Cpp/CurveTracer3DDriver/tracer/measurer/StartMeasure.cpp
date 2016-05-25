#include "StartMeasure.h"


StartMeasure::StartMeasure(void) : DeviceRequest(10, 10)
{
	sendBuffer[0] = 0x02;
}

StartMeasure::~StartMeasure(void)
{
}

int StartMeasure::GetExpectedSize()
{
	return 8;
}

int StartMeasure::GetSendSize()
{
	return 1;
}