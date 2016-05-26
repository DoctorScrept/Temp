#ifndef GET_VERSION_REQUEST_H
#define GET_VERSION_REQUEST_H

#include "core/DeviceRequest.h"

#define READ_VERSION 0x00

class GetVersionRequest : public DeviceRequest
{
public:
	GetVersionRequest();
	virtual ~GetVersionRequest();

	virtual int GetExpectedSize();
	virtual int GetSendSize();

	virtual bool OnAsyncResponse();

	int GetMajorVersion();
	int GetMinorVersion();

	bool IsVersionMatch(int major, int minor);
};

#endif // GET_VERSION_REQUEST_H