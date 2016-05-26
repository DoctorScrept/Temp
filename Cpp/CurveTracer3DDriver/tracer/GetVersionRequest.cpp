#include "GetVersionRequest.h"


GetVersionRequest::GetVersionRequest() : DeviceRequest(2, 4)
{
	sendBuffer[0] = READ_VERSION;    // command
	sendBuffer[1] = 2;               // Expected length of the result
}

GetVersionRequest::~GetVersionRequest()
{
}

int GetVersionRequest::GetExpectedSize()
{
	return 4;
}

int GetVersionRequest::GetSendSize()
{
	return 2;
}

bool GetVersionRequest::OnAsyncResponse() {
	return false;
}

int GetVersionRequest::GetMajorVersion() {
	return receiveBuffer[1];
}

int GetVersionRequest::GetMinorVersion() {
	return receiveBuffer[2];
}

bool GetVersionRequest::IsVersionMatch(int major, int minor) {
	return receiveBuffer[1] == major && receiveBuffer[2] == minor;
}