#ifndef DEVICE_REQUEST_H
#define DEVICE_REQUEST_H

class DeviceRequest
{
protected:
	unsigned char * sendBuffer;
	unsigned char * receiveBuffer;

public:
	DeviceRequest();
	~DeviceRequest();

	virtual int GetExpectedSize() = 0;
	virtual int GetSendSize() = 0;

	virtual unsigned char * GetSendBuffer();
	virtual unsigned char * GetReceiveBuffer();

	virtual bool OnResponse();
};

#endif // DEVICE_REQUEST_H

