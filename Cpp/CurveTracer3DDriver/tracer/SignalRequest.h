#ifndef SIGNAL_REQUEST_H
#define SIGNAL_REQUEST_H

#include "core/DeviceRequest.h"

class SignalRequest : public DeviceRequest
{
public:
	SignalRequest();
	~SignalRequest();

	virtual int GetExpectedSize();
	virtual int GetSendSize();
};

#endif // SIGNAL_REQUEST_H