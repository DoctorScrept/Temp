#ifndef GET_VERSION_REQUEST_H
#define GET_VERSION_REQUEST_H

#include "core/DeviceRequest.h"

class GetVersionRequest : public DeviceRequest
{
public:
	GetVersionRequest();
	virtual ~GetVersionRequest();
};

#endif // GET_VERSION_REQUEST_H