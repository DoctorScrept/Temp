#ifndef MEASURE_REQUEST_H
#define MEASURE_REQUEST_H

#include "core/DeviceRequest.h"

class MeasureRequest : public DeviceRequest
{
public:
	MeasureRequest();
	~MeasureRequest();

	virtual int GetExpectedSize();
	virtual int GetSendSize();
};

#endif // MEASURE_REQUEST_H