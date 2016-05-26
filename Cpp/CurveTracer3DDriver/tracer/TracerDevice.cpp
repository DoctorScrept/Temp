#include "TracerDevice.h"

TracerDevice::TracerDevice() : USBDevice("vid_04d8&pid_000c", "\\MCHP_EP1", "\\MCHP_EP1")
{
	getVersionRequest = new GetVersionRequest();
	signalRequest = new SignalRequest();
	measureRequest = new MeasureRequest();
}

TracerDevice::~TracerDevice()
{
	delete getVersionRequest;
	delete signalRequest;
	delete measureRequest;
}

int TracerDevice::Connect()
{
	int result = USBDevice::Connect();
	if (result != 0) {
		return result;
	}

	SendRequest(getVersionRequest);

	PBYTE recvBuffer = getVersionRequest->GetReceiveBuffer();
	if (recvBuffer[0] == READ_VERSION)
	{
		if (!getVersionRequest->IsVersionMatch(MAJOR_VERSION, MINOR_VERSION))
		{
			USBDevice::SetLastError(WRONG_VERSION);
			return WRONG_VERSION;
		}
	}

	return STATE_OK;
}

int TracerDevice::IsVersionConfirmed() {
	return getVersionRequest->IsVersionMatch(MAJOR_VERSION, MINOR_VERSION);
}