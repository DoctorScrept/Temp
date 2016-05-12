#include "DeviceRequest.h"


DeviceRequest::DeviceRequest()
{
	sendBuffer = new unsigned char[10];
	receiveBuffer = new unsigned char[10];

	sendBuffer[0] = 0x00;    // command
	sendBuffer[1] = 2;               // Expected length of the result
}


DeviceRequest::~DeviceRequest()
{
	delete[] sendBuffer;
	delete[] receiveBuffer;
}
