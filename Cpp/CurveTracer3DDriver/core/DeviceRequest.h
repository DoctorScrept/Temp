#pragma once
class DeviceRequest
{
	unsigned char * sendBuffer;
	unsigned char * receiveBuffer;

public:
	DeviceRequest();
	~DeviceRequest();

	virtual int GetExpectedSize() {
		return 4;
	}
	virtual int GetSendSize() {
		return 2;
	}

	virtual unsigned char * GetSendBuffer() {
		return sendBuffer;
	}

	virtual unsigned char * GetReceiveBuffer() {
		return receiveBuffer;
	}
};

