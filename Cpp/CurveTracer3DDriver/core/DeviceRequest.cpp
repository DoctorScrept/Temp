#include "DeviceRequest.h"


DeviceRequest::DeviceRequest()
{
	sendBuffer = 0;
	receiveBuffer = 0;
}

DeviceRequest::~DeviceRequest()
{
	delete[] sendBuffer;
	delete[] receiveBuffer;
}

unsigned char * DeviceRequest::GetSendBuffer()
{
	return sendBuffer;
}

unsigned char * DeviceRequest::GetReceiveBuffer()
{
	return receiveBuffer;
}

bool DeviceRequest::OnResponse() {
	return false;
}