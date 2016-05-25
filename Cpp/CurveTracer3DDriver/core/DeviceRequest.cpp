#include "DeviceRequest.h"


DeviceRequest::DeviceRequest(int sendSize, int recvSize)
{
	if (sendSize > 0) {
		sendBuffer = new unsigned char[sendSize];
	} else {
		sendBuffer = 0;
	}

	if (recvSize > 0) {
		receiveBuffer = new unsigned char[recvSize];
	}
	else {
		receiveBuffer = 0;
	}
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

bool DeviceRequest::OnAsyncResponse() {
	return false;
}